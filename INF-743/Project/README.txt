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