	.file	"prog.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	get_millis
	.type	get_millis, @function
get_millis:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	lds r20,interruptions
	lds r21,interruptions+1
	lds r22,interruptions+2
	lds r23,interruptions+3
	lds r24,178
	ldi r25,14
	1:
	lsl r20
	rol r21
	rol r22
	rol r23
	dec r25
	brne 1b
	ldi r16,lo8(64)
	mul r24,r16
	movw r24,r0
	clr __zero_reg__
	mov __tmp_reg__,r25
	lsl r0
	sbc r26,r26
	sbc r27,r27
	movw r16,r20
	movw r18,r22
	add r16,r24
	adc r17,r25
	adc r18,r26
	adc r19,r27
	movw r24,r18
	movw r22,r16
	ldi r18,lo8(-24)
	ldi r19,lo8(3)
	ldi r20,0
	ldi r21,0
	call __divmodsi4
	movw r24,r20
	movw r22,r18
/* epilogue start */
	pop r17
	pop r16
	ret
	.size	get_millis, .-get_millis
.global	sleep_cpu
	.type	sleep_cpu, @function
sleep_cpu:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x33
	ori r24,lo8(1)
	out 0x33,r24
	call get_millis
	sts sleepTime,r22
	sts sleepTime+1,r23
	sts sleepTime+2,r24
	sts sleepTime+3,r25
/* #APP */
 ;  32 "prog.c" 1
	sleep
 ;  0 "" 2
/* #NOAPP */
	ret
	.size	sleep_cpu, .-sleep_cpu
.global	set_sleep_mode
	.type	set_sleep_mode, @function
set_sleep_mode:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cpi r24,lo8(2)
	breq .L5
	brge .L6
	tst r24
	breq .L7
	cpi r24,lo8(1)
	breq .L8
	ret
.L6:
	cpi r24,lo8(3)
	breq .L9
	cpi r24,lo8(4)
	breq .L10
	ret
.L7:
	in r24,0x33
	andi r24,lo8(-3)
	out 0x33,r24
	in r24,0x33
	andi r24,lo8(-5)
	out 0x33,r24
	in r24,0x33
	andi r24,lo8(-9)
	rjmp .L11
.L8:
	in r24,0x33
	andi r24,lo8(-3)
	out 0x33,r24
	in r24,0x33
	ori r24,lo8(4)
	rjmp .L11
.L5:
	in r24,0x33
	ori r24,lo8(6)
	rjmp .L11
.L9:
	in r24,0x33
	andi r24,lo8(-3)
	out 0x33,r24
	in r24,0x33
	ori r24,lo8(12)
	rjmp .L11
.L10:
	in r24,0x33
	ori r24,lo8(14)
.L11:
	out 0x33,r24
	ret
	.size	set_sleep_mode, .-set_sleep_mode
.global	increment_sleep_time
	.type	increment_sleep_time, @function
increment_sleep_time:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	lds r16,sleepTime
	lds r17,sleepTime+1
	lds r18,sleepTime+2
	lds r19,sleepTime+3
	cp __zero_reg__,r16
	cpc __zero_reg__,r17
	cpc __zero_reg__,r18
	cpc __zero_reg__,r19
	brge .L14
	lds r16,sleepTime
	lds r17,sleepTime+1
	lds r18,sleepTime+2
	lds r19,sleepTime+3
	lds r12,totalSleepTime
	lds r13,totalSleepTime+1
	lds r14,totalSleepTime+2
	lds r15,totalSleepTime+3
	movw r20,r22
	movw r22,r24
	sub r20,r16
	sbc r21,r17
	sbc r22,r18
	sbc r23,r19
	movw r26,r22
	movw r24,r20
	add r24,r12
	adc r25,r13
	adc r26,r14
	adc r27,r15
	sts totalSleepTime,r24
	sts totalSleepTime+1,r25
	sts totalSleepTime+2,r26
	sts totalSleepTime+3,r27
.L14:
	sts sleepTime,__zero_reg__
	sts sleepTime+1,__zero_reg__
	sts sleepTime+2,__zero_reg__
	sts sleepTime+3,__zero_reg__
/* epilogue start */
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	increment_sleep_time, .-increment_sleep_time
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call uart_init
	ldi r24,lo8(uart_output)
	ldi r25,hi8(uart_output)
	sts __iob+2+1,r25
	sts __iob+2,r24
	ldi r24,lo8(uart_input)
	ldi r25,hi8(uart_input)
	sts __iob+1,r25
	sts __iob,r24
	cbi 0xa,2
	sbi 0xb,2
	lds r24,105
	andi r24,lo8(-2)
	sts 105,r24
	lds r24,105
	ori r24,lo8(2)
	sts 105,r24
	sbi 0x1d,0
	ldi r24,lo8(-1)
	out 0x4,r24
	out 0x5,__zero_reg__
	sts 177,__zero_reg__
	sts 178,__zero_reg__
	out 0x17,__zero_reg__
	ldi r24,lo8(1)
	sts 112,r24
	sts 176,__zero_reg__
	ldi r24,lo8(7)
	sts 177,r24
/* #APP */
 ;  93 "prog.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L16:
	lds r24,shouldSleep
	tst r24
	breq .L16
	ldi r24,lo8(2)
	call set_sleep_mode
	call sleep_cpu
	rjmp .L16
	.size	main, .-main
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Time since system is booted: %ld \n"
.LC1:
	.string	"Duty cycle: %d%% \n"
	.text
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r12
	push r13
	push r14
	push r15
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 19 */
.L__stack_usage = 19
	call get_millis
	movw r12,r22
	movw r14,r24
	call increment_sleep_time
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	or r24,r25
	or r24,r26
	or r24,r27
	breq .+2
	rjmp .L20
	sts delayTime,r12
	sts delayTime+1,r13
	sts delayTime+2,r14
	sts delayTime+3,r15
	sts shouldSleep,__zero_reg__
	sts bounceTime,r12
	sts bounceTime+1,r13
	sts bounceTime+2,r14
	sts bounceTime+3,r15
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	push r27
	push r26
	push r25
	push r24
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	push r25
	push r24
	call printf
	lds r20,bounceTime
	lds r21,bounceTime+1
	lds r22,bounceTime+2
	lds r23,bounceTime+3
	lds r24,totalSleepTime
	lds r25,totalSleepTime+1
	lds r26,totalSleepTime+2
	lds r27,totalSleepTime+3
	lds r12,bounceTime
	lds r13,bounceTime+1
	lds r14,bounceTime+2
	lds r15,bounceTime+3
	movw r18,r20
	movw r20,r22
	sub r18,r24
	sbc r19,r25
	sbc r20,r26
	sbc r21,r27
	ldi r26,lo8(100)
	ldi r27,0
	call __muluhisi3
	movw r20,r14
	movw r18,r12
	call __divmodsi4
	push r21
	push r20
	push r19
	push r18
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	push r25
	push r24
	call printf
	in r24,__SP_L__
	in r25,__SP_H__
	adiw r24,12
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r25
	out __SREG__,__tmp_reg__
	out __SP_L__,r24
.L20:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r15
	pop r14
	pop r13
	pop r12
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_1, .-__vector_1
.global	__vector_9
	.type	__vector_9, @function
__vector_9:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
	push r28
	push r29
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: Signal */
/* frame size = 4 */
/* stack size = 25 */
.L__stack_usage = 25
	lds r24,interruptions
	lds r25,interruptions+1
	lds r26,interruptions+2
	lds r27,interruptions+3
	adiw r24,1
	adc r26,__zero_reg__
	adc r27,__zero_reg__
	sts interruptions,r24
	sts interruptions+1,r25
	sts interruptions+2,r26
	sts interruptions+3,r27
	lds r20,interruptions
	lds r21,interruptions+1
	lds r22,interruptions+2
	lds r23,interruptions+3
	lds r24,178
	ldi r25,14
	1:
	lsl r20
	rol r21
	rol r22
	rol r23
	dec r25
	brne 1b
	ldi r16,lo8(64)
	mul r24,r16
	movw r24,r0
	clr __zero_reg__
	mov __tmp_reg__,r25
	lsl r0
	sbc r26,r26
	sbc r27,r27
	movw r14,r20
	movw r16,r22
	add r14,r24
	adc r15,r25
	adc r16,r26
	adc r17,r27
	movw r24,r16
	movw r22,r14
	ldi r18,lo8(-24)
	ldi r19,lo8(3)
	ldi r20,0
	ldi r21,0
	call __divmodsi4
	movw r24,r20
	movw r22,r18
	std Y+1,r18
	std Y+2,r19
	std Y+3,r20
	std Y+4,r21
	call increment_sleep_time
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	ldd r18,Y+1
	ldd r19,Y+2
	ldd r20,Y+3
	ldd r21,Y+4
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brge .L23
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	movw r14,r18
	movw r16,r20
	sub r14,r24
	sbc r15,r25
	sbc r16,r26
	sbc r17,r27
	movw r26,r16
	movw r24,r14
	cpi r24,101
	cpc r25,__zero_reg__
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlt .L23
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
.L23:
	lds r24,delayTime
	lds r25,delayTime+1
	lds r26,delayTime+2
	lds r27,delayTime+3
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brge .L22
	lds r24,delayTime
	lds r25,delayTime+1
	lds r26,delayTime+2
	lds r27,delayTime+3
	movw r14,r18
	movw r16,r20
	sub r14,r24
	sbc r15,r25
	sbc r16,r26
	sbc r17,r27
	movw r26,r16
	movw r24,r14
	sbiw r24,31
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlt .L22
	sts delayTime,__zero_reg__
	sts delayTime+1,__zero_reg__
	sts delayTime+2,__zero_reg__
	sts delayTime+3,__zero_reg__
	ldi r24,lo8(1)
	sts shouldSleep,r24
.L22:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_9, .-__vector_9
.global	shouldSleep
	.data
	.type	shouldSleep, @object
	.size	shouldSleep, 1
shouldSleep:
	.byte	1
.global	delayTime
	.section .bss
	.type	delayTime, @object
	.size	delayTime, 4
delayTime:
	.zero	4
.global	totalSleepTime
	.type	totalSleepTime, @object
	.size	totalSleepTime, 4
totalSleepTime:
	.zero	4
.global	sleepTime
	.type	sleepTime, @object
	.size	sleepTime, 4
sleepTime:
	.zero	4
.global	bounceTime
	.type	bounceTime, @object
	.size	bounceTime, 4
bounceTime:
	.zero	4
.global	interruptions
	.type	interruptions, @object
	.size	interruptions, 4
interruptions:
	.zero	4
.global	uart_input
	.data
	.type	uart_input, @object
	.size	uart_input, 14
uart_input:
	.zero	3
	.byte	1
	.zero	4
	.word	0
	.word	gs(uart_getchar)
	.word	0
.global	uart_output
	.type	uart_output, @object
	.size	uart_output, 14
uart_output:
	.zero	3
	.byte	2
	.zero	4
	.word	gs(uart_putchar)
	.word	0
	.word	0
	.ident	"GCC: (GNU) 4.9.2"
.global __do_copy_data
.global __do_clear_bss
