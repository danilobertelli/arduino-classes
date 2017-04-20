/**
Danilo Arthur Bertelli
channel: https://thingspeak.com/channels/244382 

The BLE SCAN uses BlueZ stack, the code is based on glock45 implementation:
https://github.com/glock45/intel-edison-playground/

This code will detect the defined iBeacons UUID (DEFINED_UUID or MY_UUID), when detected
it will turn the relay ON and add the state (1 == DETECTED/RELAY ON) with a timestamp to
a buffer.

If the beacon leaves the range of the Edison, it will wait for more 5 seconds and then,
add the state (0 == NOT DETECTED/RELAY OFF) with a timestamp of the event to the same buffer.

There is also a thread running together the main thread, this thread is responsible for:
- Check if the beacon is visible after 5 seconds that he leaves the range.
- Check if we have data in our circular buffer and send it to server.*

* Note that, as we are using a FREE license of ThingSpeak, we need to control the data posts,
this code is sending each request after 20 seconds.

ThingSpeakLinux libary -
This library is used to send the data on the ThingSpeak server, however, I've modified it to
be able to send the parameter "created_at" so we can post the exactly timestamp value of each
event on it.

For this case, I've added a new function:
char SendDataToThingSpeakWithDate(int FieldNo, float * FieldArray, char * Key, int SizeOfKey, char *date);

That accepts the same values from original function plus the date (formatted as e.g 2010-12-13T20:20:06-05:00)

// ------------------------------------------------------------------------------------------------
Instructions:
1) Upload all files (trabalho.c / ThingSpeakLinux.c / ThingSpeakLinux.h) to a directory on edison
2) Compile running:
gcc ThingSpeakLinux.c trabalho.c -o trabalho -lmraa -lbluetooth -lpthread

or if any compile error occurs:

g++ -std=c++11 ThingSpeakLinux.c trabalho.c -o trabalho -lmraa -lbluetooth -lpthread

3) Run it: ./trabalho
4) Play with your iBeacon.
5) If you need to change your iBeacon uuid, you can just modify the DEFINED_UUID or MY_UUID values and
compile it again.

In case of any erros running (bluetooth not working, please run)
rfkill unblock bluetooth
killall bluetoothd
hciconfig hci0 up

The code already does it, but you never know...
*/

#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <mraa.h> // lib to enable/disable/use pins
#include <pthread.h> // thread things
#include <time.h> // time things
#include "ThingSpeakLinux.h" // ThingSpeak Linux library

#define _POSIX_C_SOURCE 200809L
#define EIR_MANUFACTURE_SPECIFIC 0xFF
#define RELAY_PIN 5        /**< The digital pin where the RELAY is connected */
#define DEFINED_UUID "310064a25bd64d9780bfd99fa7e9b72f"
#define MY_UUID      "6d4a256c8def480d8e001d03d6df14e6"
#define UUID_LENGTH 32
#define CHANNEL_KEY "RUJZSC9RDYC6ZBEH"
#define MAX_ITEMS 100

struct hci_request ble_hci_request(uint16_t ocf, int clen, void * status, void * cparam)
{
	struct hci_request rq;
	memset(&rq, 0, sizeof(rq));
	rq.ogf = OGF_LE_CTL;
	rq.ocf = ocf;
	rq.cparam = cparam;
	rq.clen = clen;
	rq.rparam = status;
	rq.rlen = 1;
	return rq;
}

// My Circular buffer implementation
typedef struct circularBuffer
{
    int      first;
    int      last;
    int      validItems;
    int      state[MAX_ITEMS];
	time_t   date[MAX_ITEMS];
} circularBuffer_t;

// Buffer prototypes
void initializeBuffer(circularBuffer_t *buffer);
int isEmpty(circularBuffer_t *buffer);
int putItem(circularBuffer_t *buffer, int state, time_t date);
int getItem(circularBuffer_t *buffer, int *state, time_t *date);
void printBuffer(circularBuffer_t *buffer);

mraa_gpio_context relayPin;     // PIN that relay is connected
bool found = false;             // just to control if we found or not our beacon
long int startTime = 0;         // time the code started to run
long int lastSeenSeconds = -1;  // last time that beacon was seen
long int lastDataSent = -1;     // last time that data was sent
int lastItemAdded = -1;
circularBuffer_t   myServerBuffer;

char* format_time(time_t time) {
	// format 2010-12-13T20:20:06-05:00
	char *buf = (char *) malloc(sizeof(char) * 256);
	struct tm *ptm = localtime(&time);
	strftime(buf, 256, "%Y-%m-%dT%T%z", ptm);
	return buf;
}

time_t current_time_t() {
	time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
	return s;
}

long int current_time_in_s() {
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    s  = spec.tv_sec;
	return s;
}

// Send our data to server
void sendDataToServer(int state, time_t date) {
	lastDataSent = current_time_in_s();
	float FieldsArray[1];
	FieldsArray[0] = state; // ON or OFF attribute (state)
	char *formatted = format_time(date);
	printf("Will post %d state with %s as date", state, formatted);
	SendDataToThingSpeakWithDate(1, &FieldsArray[0],CHANNEL_KEY,sizeof(CHANNEL_KEY), formatted);
}

bool is_our_uuid(uint8_t *data, size_t data_len) {
	char uuid[UUID_LENGTH+1];
	uuid[0] = '\0';
	if (data[0] == EIR_MANUFACTURE_SPECIFIC) {
		int i;
		for(i = 5; i < data_len && i < 21; i++) {
			sprintf(uuid+strlen(uuid), "%02x", data[i]);        
		}
	}
    bool result = false;
	// Compare if it's the same UUID from my device or class teacher
    if (strcmp(uuid, MY_UUID) == 0 || strcmp(uuid, DEFINED_UUID) == 0) {
	    result = true;
    }
    return result;
}

void* doThreadWork(void *arg) {
	while (1) {
		long int currentTime = current_time_in_s();
		if (lastSeenSeconds > 0 && currentTime - lastSeenSeconds >= 5) {
			// printf("Turn off\n");
			/* Turn the RELAY off */
			mraa_gpio_write(relayPin, 0);
			lastSeenSeconds = -1;
		    putItem(&myServerBuffer, 0, current_time_t());
		}

		// This check if we are allowed to send data to server
		if (isEmpty(&myServerBuffer) == 0 && (lastDataSent == -1 || currentTime - lastDataSent > 20)) {
			// Buffer is not empty and time is good.
			// printf("Buffer before send: \n");
		    // printBuffer(&myServerBuffer);

			int state = -1;
			time_t date = 0;
			getItem(&myServerBuffer, &state, &date);
			if (state > -1 && date > 0) {
				sendDataToServer(state, date);
			}
			// printf("Buffer after send: \n");
			// printBuffer(&myServerBuffer);
		}
	}

    return NULL;
}

int main() {
	printf("Welcome to jungle!\n");
	startTime = current_time_in_s(); // get our V0
    mraa_init();
    
    // Set the relayPin as port D5
    relayPin = mraa_gpio_init(RELAY_PIN);
     
    // Set the said pin as output
    mraa_gpio_dir(relayPin, MRAA_GPIO_OUT);
    mraa_gpio_write(relayPin, 0);

	int ret, status;

	system("rfkill unblock bluetooth");
	system("killall bluetoothd");
	system("hciconfig hci0 up");
	
	// Initialize the buffer
	initializeBuffer(&myServerBuffer);

	// Get HCI device.
	const int device = hci_open_dev(hci_get_route(NULL));
	if ( device < 0 ) { 
		perror("Failed to open HCI device.\n");
		return 0; 
	}

	// Set BLE scan parameters.
	le_set_scan_parameters_cp scan_params_cp;
	memset(&scan_params_cp, 0, sizeof(scan_params_cp));
	scan_params_cp.type 			= 0x00; 
	scan_params_cp.interval 		= htobs(0x0010);
	scan_params_cp.window 			= htobs(0x0010);
	scan_params_cp.own_bdaddr_type 	= 0x00; // Public Device Address (default).
	scan_params_cp.filter 			= 0x00; // Accept all.

	struct hci_request scan_params_rq = ble_hci_request(OCF_LE_SET_SCAN_PARAMETERS, LE_SET_SCAN_PARAMETERS_CP_SIZE, &status, &scan_params_cp);
	
	ret = hci_send_req(device, &scan_params_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to set scan parameters data.");
		return 0;
	}

	// Set BLE events report mask.
	le_set_event_mask_cp event_mask_cp;
	memset(&event_mask_cp, 0, sizeof(le_set_event_mask_cp));
	int i = 0;
	for ( i = 0 ; i < 8 ; i++ ) event_mask_cp.mask[i] = 0xFF;

	struct hci_request set_mask_rq = ble_hci_request(OCF_LE_SET_EVENT_MASK, LE_SET_EVENT_MASK_CP_SIZE, &status, &event_mask_cp);
	ret = hci_send_req(device, &set_mask_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to set event mask.");
		return 0;
	}

	// Enable scanning.
	le_set_scan_enable_cp scan_cp;
	memset(&scan_cp, 0, sizeof(scan_cp));
	scan_cp.enable 		= 0x01;	// Enable flag.
	scan_cp.filter_dup 	= 0x00; // Filtering disabled.

	struct hci_request enable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);

	ret = hci_send_req(device, &enable_adv_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to enable scan.");
		return 0;
	}

	// Get Results.
	struct hci_filter nf;
	hci_filter_clear(&nf);
	hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
	hci_filter_set_event(EVT_LE_META_EVENT, &nf);
	if ( setsockopt(device, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0 ) {
		hci_close_dev(device);
		perror("Could not set socket options\n");
		return 0;
	}

	// Creates the thread stuff
	pthread_t inc_x_thread;
	if (pthread_create(&inc_x_thread, NULL, &doThreadWork, NULL)) {
		perror("Could not create a thread\n");
		return 0;
	}	
	
	printf("Scanning....\n");
	uint8_t buf[HCI_MAX_EVENT_SIZE];
	evt_le_meta_event * meta_event;
	le_advertising_info * infoPacket;
	int len;

	while ( 1 ) {
		len = read(device, buf, sizeof(buf));
		if ( len >= HCI_EVENT_HDR_SIZE ) {
			meta_event = (evt_le_meta_event*)(buf+HCI_EVENT_HDR_SIZE+1);
			if ( meta_event->subevent == EVT_LE_ADVERTISING_REPORT ) {
				uint8_t reports_count = meta_event->data[0];
				void * offset = meta_event->data + 1;
				while ( reports_count-- ) {
					infoPacket = (le_advertising_info *)offset;
                    int current_index = 0;
					int data_error = 0;

					while(!data_error && current_index < infoPacket->length) {
						size_t data_len = infoPacket->data[current_index];
						if(data_len + 1 > infoPacket->length) {
							data_error = 1;
						} else {
							found = is_our_uuid(infoPacket->data + current_index + 1, data_len);
							current_index += data_len + 1;

							if (found) {
								//printf("found!!!\n");
								lastSeenSeconds = current_time_in_s();
								/* Turn RELAY on */
								mraa_gpio_write(relayPin, 1);
								found = false;
								putItem(&myServerBuffer, 1, current_time_t());
							} else {
								//printf("NOT found \n");
							}
						}
					}
				}
			}
		}
	}

	// Disable scanning.
	memset(&scan_cp, 0, sizeof(scan_cp));
	scan_cp.enable = 0x00;	// Disable flag.

	struct hci_request disable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);
	ret = hci_send_req(device, &disable_adv_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to disable scan.");
		return 0;
	}

	hci_close_dev(device);
	
	return 0;
}

void initializeBuffer(circularBuffer_t *buffer) {
    int i;
    buffer->validItems  =  0;
    buffer->first       =  0;
    buffer->last        =  0;
    for(i=0; i<MAX_ITEMS; i++) {
        buffer->state[i] = -1;
		buffer->date[i] = 0;
    }        
    return;
}

int isEmpty(circularBuffer_t *buffer) {
    if(buffer->validItems==0)
        return(1);
    else
        return(0);
}

int putItem(circularBuffer_t *buffer, int state, time_t date) {
    if(buffer->validItems>=MAX_ITEMS || lastItemAdded == state) {
        //printf("Cannot add more items, buffer is full or item is already added \n");
        return(-1);
    } else {
        buffer->validItems++;
        buffer->state[buffer->last] = state;
		buffer->date[buffer->last] = date;
        buffer->last = (buffer->last+1)%MAX_ITEMS;
		lastItemAdded = state;
    }
}

int getItem(circularBuffer_t *buffer, int *state, time_t *date) {
    if(isEmpty(buffer)) {
        printf("getItem: isempty\n");
        return(-1);
    } else {
        *state=buffer->state[buffer->first];
		*date=buffer->date[buffer->first];
        buffer->first=(buffer->first+1)%MAX_ITEMS;
        buffer->validItems--;
        return(0);
    }
}

void printBuffer(circularBuffer_t *buffer) {
    int aux, aux1;
    aux  = buffer->first;
    aux1 = buffer->validItems;
    while(aux1 > 0) {
        printf("Element #%d = %d\n", aux, buffer->state[aux]);
		char *formatted = format_time(buffer->date[aux]);
		printf(formatted);
		printf("\n");
        aux=(aux+1)%MAX_ITEMS;
        aux1--;
    }
    return;
}
