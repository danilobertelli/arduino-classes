	.file	"prog.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	enableTimer
	.type	enableTimer, @function
enableTimer:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(-79)
	ldi r31,0
	st Z,__zero_reg__
	ldi r24,lo8(-126)
	sts 178,r24
	out 0x17,__zero_reg__
	ldi r24,lo8(1)
	sts 112,r24
	sts 176,__zero_reg__
	ldi r24,lo8(5)
	st Z,r24
	sts count,__zero_reg__
	sts count+1,__zero_reg__
	sts count+2,__zero_reg__
	sts count+3,__zero_reg__
	ret
	.size	enableTimer, .-enableTimer
.global	disableTimer
	.type	disableTimer, @function
disableTimer:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sts 177,__zero_reg__
	ldi r24,lo8(-126)
	sts 178,r24
	out 0x17,__zero_reg__
	sts count,__zero_reg__
	sts count+1,__zero_reg__
	sts count+2,__zero_reg__
	sts count+3,__zero_reg__
	ret
	.size	disableTimer, .-disableTimer
.global	get_millis
	.type	get_millis, @function
get_millis:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r22,count
	lds r23,count+1
	lds r24,count+2
	lds r25,count+3
	ret
	.size	get_millis, .-get_millis
.global	get_micros
	.type	get_micros, @function
get_micros:
	push r12
	push r13
	push r14
	push r15
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	lds r18,count
	lds r19,count+1
	lds r20,count+2
	lds r21,count+3
	lds r12,178
	ldi r24,lo8(8)
	mul r12,r24
	movw r12,r0
	clr __zero_reg__
	mov __tmp_reg__,r13
	lsl r0
	sbc r14,r14
	sbc r15,r15
	ldi r26,lo8(-24)
	ldi r27,lo8(3)
	call __muluhisi3
	add r22,r12
	adc r23,r13
	adc r24,r14
	adc r25,r15
/* epilogue start */
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	get_micros, .-get_micros
.global	get_random
	.type	get_random, @function
get_random:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call rand
	ldi r22,lo8(3)
	ldi r23,0
	call __divmodhi4
	adiw r24,1
	ret
	.size	get_random, .-get_random
.global	turnLedStatus
	.type	turnLedStatus, @function
turnLedStatus:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	out 0x5,r24
	in r24,0x5
	cpi r24,lo8(-1)
	brne .L7
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	sts ledTime,r24
	sts ledTime+1,r25
	sts ledTime+2,r26
	sts ledTime+3,r27
	ret
.L7:
	sts ledTime,__zero_reg__
	sts ledTime+1,__zero_reg__
	sts ledTime+2,__zero_reg__
	sts ledTime+3,__zero_reg__
	ret
	.size	turnLedStatus, .-turnLedStatus
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Press button to start"
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
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	call puts
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
	call enableTimer
/* #APP */
 ;  99 "prog.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L11:
	rjmp .L11
	.size	main, .-main
	.section	.rodata.str1.1
.LC1:
	.string	"Congrats, reaction time = %lu ms, new round! \n"
.LC2:
	.string	"Beeeh, wrong time, game Over!\n"
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
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	or r24,r25
	or r24,r26
	or r24,r27
	breq .+2
	rjmp .L12
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	sts bounceTime,r24
	sts bounceTime+1,r25
	sts bounceTime+2,r26
	sts bounceTime+3,r27
	lds r24,reseted
	tst r24
	breq .L14
	sts reseted,__zero_reg__
	lds r12,count
	lds r13,count+1
	lds r14,count+2
	lds r15,count+3
	call get_random
	ldi r18,lo8(-24)
	ldi r19,lo8(3)
	movw r20,r24
	mul r20,r18
	movw r24,r0
	mul r20,r19
	add r25,r0
	mul r21,r18
	add r25,r0
	clr r1
	add r12,r24
	adc r13,r25
	adc r14,__zero_reg__
	adc r15,__zero_reg__
	sts targetTime,r12
	sts targetTime+1,r13
	sts targetTime+2,r14
	sts targetTime+3,r15
	rjmp .L12
.L14:
	in r24,0x5
	cpi r24,lo8(-1)
	breq .+2
	rjmp .L15
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	lds r20,ledTime
	lds r21,ledTime+1
	lds r22,ledTime+2
	lds r23,ledTime+3
	sub r24,r20
	sbc r25,r21
	sbc r26,r22
	sbc r27,r23
	push r27
	push r26
	push r25
	push r24
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	push r25
	push r24
	call printf
	ldi r24,0
	ldi r25,0
	call turnLedStatus
	lds r12,count
	lds r13,count+1
	lds r14,count+2
	lds r15,count+3
	call get_random
	ldi r18,lo8(-24)
	ldi r19,lo8(3)
	movw r20,r24
	mul r20,r18
	movw r24,r0
	mul r20,r19
	add r25,r0
	mul r21,r18
	add r25,r0
	clr r1
	add r12,r24
	adc r13,r25
	adc r14,__zero_reg__
	adc r15,__zero_reg__
	sts targetTime,r12
	sts targetTime+1,r13
	sts targetTime+2,r14
	sts targetTime+3,r15
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	rjmp .L12
.L15:
	ldi r24,lo8(.LC2)
	ldi r25,hi8(.LC2)
	call puts
	ldi r24,lo8(1)
	sts reseted,r24
	ldi r24,0
	ldi r25,0
	call turnLedStatus
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
	sts targetTime,__zero_reg__
	sts targetTime+1,__zero_reg__
	sts targetTime+2,__zero_reg__
	sts targetTime+3,__zero_reg__
	sts count,__zero_reg__
	sts count+1,__zero_reg__
	sts count+2,__zero_reg__
	sts count+3,__zero_reg__
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	call puts
.L12:
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
	.section	.rodata.str1.1
.LC3:
	.string	"Game Over! Time out\n\nPress a button to start"
	.text
.global	__vector_9
	.type	__vector_9, @function
__vector_9:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
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
/* stack size = 15 */
.L__stack_usage = 15
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	adiw r24,1
	adc r26,__zero_reg__
	adc r27,__zero_reg__
	sts count,r24
	sts count+1,r25
	sts count+2,r26
	sts count+3,r27
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	or r24,r25
	or r24,r26
	or r24,r27
	breq .L17
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	lds r20,bounceTime
	lds r21,bounceTime+1
	lds r22,bounceTime+2
	lds r23,bounceTime+3
	sub r24,r20
	sbc r25,r21
	sbc r26,r22
	sbc r27,r23
	cpi r24,-55
	cpc r25,__zero_reg__
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlo .L17
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
.L17:
	lds r24,reseted
	cpse r24,__zero_reg__
	rjmp .L18
	in r24,0x5
	cpse r24,__zero_reg__
	rjmp .L18
	lds r20,count
	lds r21,count+1
	lds r22,count+2
	lds r23,count+3
	lds r24,targetTime
	lds r25,targetTime+1
	lds r26,targetTime+2
	lds r27,targetTime+3
	cp r20,r24
	cpc r21,r25
	cpc r22,r26
	cpc r23,r27
	brlo .L18
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
	sts generatedRand+1,__zero_reg__
	sts generatedRand,__zero_reg__
	ldi r24,lo8(-1)
	ldi r25,0
	call turnLedStatus
	lds r24,count
	lds r25,count+1
	lds r26,count+2
	lds r27,count+3
	subi r24,120
	sbci r25,-20
	sbci r26,-1
	sbci r27,-1
	sts targetTime,r24
	sts targetTime+1,r25
	sts targetTime+2,r26
	sts targetTime+3,r27
.L18:
	lds r24,reseted
	cpse r24,__zero_reg__
	rjmp .L19
	in r24,0x5
	cpi r24,lo8(-1)
	brne .L19
	lds r20,count
	lds r21,count+1
	lds r22,count+2
	lds r23,count+3
	lds r24,targetTime
	lds r25,targetTime+1
	lds r26,targetTime+2
	lds r27,targetTime+3
	cp r20,r24
	cpc r21,r25
	cpc r22,r26
	cpc r23,r27
	brlo .L19
	ldi r24,lo8(.LC3)
	ldi r25,hi8(.LC3)
	push r25
	push r24
	call printf
	ldi r24,lo8(1)
	sts reseted,r24
	ldi r24,0
	ldi r25,0
	call turnLedStatus
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
	sts targetTime,__zero_reg__
	sts targetTime+1,__zero_reg__
	sts targetTime+2,__zero_reg__
	sts targetTime+3,__zero_reg__
	sts count,__zero_reg__
	sts count+1,__zero_reg__
	sts count+2,__zero_reg__
	sts count+3,__zero_reg__
	pop __tmp_reg__
	pop __tmp_reg__
.L19:
	ldi r24,lo8(-126)
	sts 178,r24
	out 0x17,__zero_reg__
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
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_9, .-__vector_9
.global	ledTime
	.section .bss
	.type	ledTime, @object
	.size	ledTime, 4
ledTime:
	.zero	4
.global	targetTime
	.type	targetTime, @object
	.size	targetTime, 4
targetTime:
	.zero	4
.global	bounceTime
	.type	bounceTime, @object
	.size	bounceTime, 4
bounceTime:
	.zero	4
.global	generatedRand
	.type	generatedRand, @object
	.size	generatedRand, 2
generatedRand:
	.zero	2
.global	reseted
	.data
	.type	reseted, @object
	.size	reseted, 1
reseted:
	.byte	1
.global	count
	.section .bss
	.type	count, @object
	.size	count, 4
count:
	.zero	4
.global	LED_ON
	.section	.rodata
	.type	LED_ON, @object
	.size	LED_ON, 2
LED_ON:
	.word	255
.global	LED_OFF
	.type	LED_OFF, @object
	.size	LED_OFF, 2
LED_OFF:
	.zero	2
.global	DEBUG
	.type	DEBUG, @object
	.size	DEBUG, 1
DEBUG:
	.zero	1
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
