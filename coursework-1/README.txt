# Course Work 1 - INF-741 Embedded systems programming for IoT
# Nome do aluno: Danilo Arthur Bertelli

# Instruções para compilação:
avr-gcc -Os -mmcu=atmega328 -S prog.c -o prog.s

avr-as -mmcu=atmega328 prog.s -o prog.o

avr-gcc -Os -mmcu=atmega328 prog.o simple_usart\uart.o -o prog.x

avr-objcopy -j .text -j .data -O ihex prog.x prog.hex

# Instruções para instalação no Arduino ATMega328 (NOTE: The path may change according to your system folders)
avrdude -p atmega328p -c arduino -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -P com3 -U flash:w:"C:\Users\vntdabe\Documents\Arduino\Aula 8\coursework-1\prog.hex":i -D -v -b 115200