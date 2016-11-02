	.file	"main.cpp"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	_Z9getMillisv
	.type	_Z9getMillisv, @function
_Z9getMillisv:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r22,timeInMillis
	lds r23,timeInMillis+1
	lds r24,timeInMillis+2
	lds r25,timeInMillis+3
	ret
	.size	_Z9getMillisv, .-_Z9getMillisv
.global	_Z12turnLedStatei
	.type	_Z12turnLedStatei, @function
_Z12turnLedStatei:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	out 0x5,r24
	in r24,0x5
	cpi r24,lo8(-1)
	brne .L3
	lds r24,timeInMillis
	lds r25,timeInMillis+1
	lds r26,timeInMillis+2
	lds r27,timeInMillis+3
	sts ledTime,r24
	sts ledTime+1,r25
	sts ledTime+2,r26
	sts ledTime+3,r27
	ret
.L3:
	sts ledTime,__zero_reg__
	sts ledTime+1,__zero_reg__
	sts ledTime+2,__zero_reg__
	sts ledTime+3,__zero_reg__
	ret
	.size	_Z12turnLedStatei, .-_Z12turnLedStatei
.global	_Z9delayTimem
	.type	_Z9delayTimem, @function
_Z9delayTimem:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
.L7:
	lds r16,timeInMillis
	lds r17,timeInMillis+1
	lds r18,timeInMillis+2
	lds r19,timeInMillis+3
	cp r16,r22
	cpc r17,r23
	cpc r18,r24
	cpc r19,r25
	brlo .L7
/* epilogue start */
	pop r17
	pop r16
	ret
	.size	_Z9delayTimem, .-_Z9delayTimem
.global	_Z20turnLedOffAfterDelaym
	.type	_Z20turnLedOffAfterDelaym, @function
_Z20turnLedOffAfterDelaym:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	lds r16,ledTime
	lds r17,ledTime+1
	lds r18,ledTime+2
	lds r19,ledTime+3
	add r22,r16
	adc r23,r17
	adc r24,r18
	adc r25,r19
	call _Z9delayTimem
	ldi r24,0
	ldi r25,0
/* epilogue start */
	pop r17
	pop r16
	jmp _Z12turnLedStatei
	.size	_Z20turnLedOffAfterDelaym, .-_Z20turnLedOffAfterDelaym
.global	_Z3dotv
	.type	_Z3dotv, @function
_Z3dotv:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(-1)
	ldi r25,0
	call _Z12turnLedStatei
	ldi r22,lo8(-106)
	ldi r23,0
	ldi r24,0
	ldi r25,0
	jmp _Z20turnLedOffAfterDelaym
	.size	_Z3dotv, .-_Z3dotv
.global	_Z4dashv
	.type	_Z4dashv, @function
_Z4dashv:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(-1)
	ldi r25,0
	call _Z12turnLedStatei
	ldi r22,lo8(-62)
	ldi r23,lo8(1)
	ldi r24,0
	ldi r25,0
	jmp _Z20turnLedOffAfterDelaym
	.size	_Z4dashv, .-_Z4dashv
.global	_Z15decode_as_blinkPc
	.type	_Z15decode_as_blinkPc, @function
_Z15decode_as_blinkPc:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 8 */
.L__stack_usage = 8
	movw r16,r24
	movw r28,r24
	0:
	ld __tmp_reg__,Y+
	tst __tmp_reg__
	brne 0b
	sbiw r28,1
	sub r28,r24
	sbc r29,r25
	movw r14,r24
	ldi r24,lo8(45)
	mov r13,r24
	ldi r25,lo8(46)
	mov r12,r25
.L13:
	movw r24,r14
	sub r24,r16
	sbc r25,r17
	cp r24,r28
	cpc r25,r29
	brge .L23
	movw r30,r14
	ld r24,Z+
	movw r14,r30
	cpi r24,lo8(46)
	brne .L20
.L15:
	lds r24,192
	sbrs r24,5
	rjmp .L15
	sts 198,r12
	call _Z3dotv
	rjmp .L16
.L20:
	lds r24,192
	sbrs r24,5
	rjmp .L20
	sts 198,r13
	call _Z4dashv
.L16:
	lds r24,timeInMillis
	lds r25,timeInMillis+1
	lds r26,timeInMillis+2
	lds r27,timeInMillis+3
	movw r22,r24
	movw r24,r26
	subi r22,106
	sbci r23,-1
	sbci r24,-1
	sbci r25,-1
	call _Z9delayTimem
	rjmp .L13
.L23:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	_Z15decode_as_blinkPc, .-_Z15decode_as_blinkPc
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	".-"
.LC1:
	.string	"-..."
.LC2:
	.string	"-.-."
.LC3:
	.string	"-.."
.LC4:
	.string	"."
.LC5:
	.string	"..-."
.LC6:
	.string	"--."
.LC7:
	.string	"...."
.LC8:
	.string	".."
.LC9:
	.string	".---"
.LC10:
	.string	"-.-"
.LC11:
	.string	".-.."
.LC12:
	.string	"--"
.LC13:
	.string	"-."
.LC14:
	.string	"---"
.LC15:
	.string	".--."
.LC16:
	.string	"--.-"
.LC17:
	.string	".-."
.LC18:
	.string	"..."
.LC19:
	.string	"-"
.LC20:
	.string	"..-"
.LC21:
	.string	"...-"
.LC22:
	.string	".--"
.LC23:
	.string	"-..-"
.LC24:
	.string	"-.--"
.LC25:
	.string	"--.."
.LC26:
	.string	"-----"
.LC27:
	.string	".----"
.LC28:
	.string	"..---"
.LC29:
	.string	"...--"
.LC30:
	.string	"....-"
.LC31:
	.string	"....."
.LC32:
	.string	"-...."
.LC33:
	.string	"--..."
.LC34:
	.string	"---.."
.LC35:
	.string	"----."
	.text
.global	_Z10find_morsev
	.type	_Z10find_morsev, @function
_Z10find_morsev:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 8 */
.L__stack_usage = 8
	ldi r28,0
	ldi r29,0
	ldi r16,lo8(buffer)
	ldi r17,hi8(buffer)
.L25:
	lds r24,buffer+16
	lds r25,buffer+16+1
	cp r28,r24
	cpc r29,r25
	brlt .+2
	rjmp .L70
	lds r30,buffer+12
	lds r31,buffer+12+1
	movw r18,r30
	subi r18,-1
	sbci r19,-1
	sts buffer+12+1,r19
	sts buffer+12,r18
	ld r30,Z
	sbiw r24,1
	sts buffer+16+1,r25
	sts buffer+16,r24
	lds r24,buffer+14
	lds r25,buffer+14+1
	cp r18,r24
	cpc r19,r25
	brne .L26
	sts buffer+12+1,r17
	sts buffer+12,r16
.L26:
	mov r24,r30
	lsl r30
	sbc r25,r25
	sbc r26,r26
	sbc r27,r27
	movw r30,r24
	sbiw r30,32
	cpi r30,59
	cpc r31,__zero_reg__
	brlo .+2
	rjmp .L27
	subi r30,lo8(-(gs(.L29)))
	sbci r31,hi8(-(gs(.L29)))
	jmp __tablejump2__
	.section	.progmem.gcc_sw_table,"a",@progbits
	.p2align	1
.L29:
	.word gs(.L67)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L30)
	.word gs(.L31)
	.word gs(.L32)
	.word gs(.L33)
	.word gs(.L34)
	.word gs(.L35)
	.word gs(.L36)
	.word gs(.L37)
	.word gs(.L38)
	.word gs(.L39)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L27)
	.word gs(.L40)
	.word gs(.L41)
	.word gs(.L42)
	.word gs(.L43)
	.word gs(.L44)
	.word gs(.L45)
	.word gs(.L46)
	.word gs(.L47)
	.word gs(.L48)
	.word gs(.L49)
	.word gs(.L50)
	.word gs(.L51)
	.word gs(.L52)
	.word gs(.L53)
	.word gs(.L54)
	.word gs(.L55)
	.word gs(.L56)
	.word gs(.L57)
	.word gs(.L58)
	.word gs(.L59)
	.word gs(.L60)
	.word gs(.L61)
	.word gs(.L62)
	.word gs(.L63)
	.word gs(.L64)
	.word gs(.L65)
	.text
.L40:
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	rjmp .L69
.L41:
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	rjmp .L69
.L42:
	ldi r24,lo8(.LC2)
	ldi r25,hi8(.LC2)
	rjmp .L69
.L43:
	ldi r24,lo8(.LC3)
	ldi r25,hi8(.LC3)
	rjmp .L69
.L44:
	ldi r24,lo8(.LC4)
	ldi r25,hi8(.LC4)
	rjmp .L69
.L45:
	ldi r24,lo8(.LC5)
	ldi r25,hi8(.LC5)
	rjmp .L69
.L46:
	ldi r24,lo8(.LC6)
	ldi r25,hi8(.LC6)
	rjmp .L69
.L47:
	ldi r24,lo8(.LC7)
	ldi r25,hi8(.LC7)
	rjmp .L69
.L48:
	ldi r24,lo8(.LC8)
	ldi r25,hi8(.LC8)
	rjmp .L69
.L49:
	ldi r24,lo8(.LC9)
	ldi r25,hi8(.LC9)
	rjmp .L69
.L50:
	ldi r24,lo8(.LC10)
	ldi r25,hi8(.LC10)
	rjmp .L69
.L51:
	ldi r24,lo8(.LC11)
	ldi r25,hi8(.LC11)
	rjmp .L69
.L52:
	ldi r24,lo8(.LC12)
	ldi r25,hi8(.LC12)
	rjmp .L69
.L53:
	ldi r24,lo8(.LC13)
	ldi r25,hi8(.LC13)
	rjmp .L69
.L54:
	ldi r24,lo8(.LC14)
	ldi r25,hi8(.LC14)
	rjmp .L69
.L55:
	ldi r24,lo8(.LC15)
	ldi r25,hi8(.LC15)
	rjmp .L69
.L56:
	ldi r24,lo8(.LC16)
	ldi r25,hi8(.LC16)
	rjmp .L69
.L57:
	ldi r24,lo8(.LC17)
	ldi r25,hi8(.LC17)
	rjmp .L69
.L58:
	ldi r24,lo8(.LC18)
	ldi r25,hi8(.LC18)
	rjmp .L69
.L59:
	ldi r24,lo8(.LC19)
	ldi r25,hi8(.LC19)
	rjmp .L69
.L60:
	ldi r24,lo8(.LC20)
	ldi r25,hi8(.LC20)
	rjmp .L69
.L61:
	ldi r24,lo8(.LC21)
	ldi r25,hi8(.LC21)
	rjmp .L69
.L62:
	ldi r24,lo8(.LC22)
	ldi r25,hi8(.LC22)
	rjmp .L69
.L63:
	ldi r24,lo8(.LC23)
	ldi r25,hi8(.LC23)
	rjmp .L69
.L64:
	ldi r24,lo8(.LC24)
	ldi r25,hi8(.LC24)
	rjmp .L69
.L65:
	ldi r24,lo8(.LC25)
	ldi r25,hi8(.LC25)
	rjmp .L69
.L30:
	ldi r24,lo8(.LC26)
	ldi r25,hi8(.LC26)
	rjmp .L69
.L31:
	ldi r24,lo8(.LC27)
	ldi r25,hi8(.LC27)
	rjmp .L69
.L32:
	ldi r24,lo8(.LC28)
	ldi r25,hi8(.LC28)
	rjmp .L69
.L33:
	ldi r24,lo8(.LC29)
	ldi r25,hi8(.LC29)
	rjmp .L69
.L34:
	ldi r24,lo8(.LC30)
	ldi r25,hi8(.LC30)
	rjmp .L69
.L35:
	ldi r24,lo8(.LC31)
	ldi r25,hi8(.LC31)
	rjmp .L69
.L36:
	ldi r24,lo8(.LC32)
	ldi r25,hi8(.LC32)
	rjmp .L69
.L37:
	ldi r24,lo8(.LC33)
	ldi r25,hi8(.LC33)
	rjmp .L69
.L38:
	ldi r24,lo8(.LC34)
	ldi r25,hi8(.LC34)
	rjmp .L69
.L39:
	ldi r24,lo8(.LC35)
	ldi r25,hi8(.LC35)
.L69:
	call _Z15decode_as_blinkPc
	rjmp .L27
.L67:
	ldi r24,lo8(88)
	ldi r25,lo8(2)
.L28:
	lds r20,timeInMillis
	lds r21,timeInMillis+1
	lds r22,timeInMillis+2
	lds r23,timeInMillis+3
	mov __tmp_reg__,r25
	lsl r0
	sbc r26,r26
	sbc r27,r27
	movw r12,r24
	movw r14,r26
	add r12,r20
	adc r13,r21
	adc r14,r22
	adc r15,r23
	movw r24,r14
	movw r22,r12
	call _Z9delayTimem
	adiw r28,1
	rjmp .L25
.L27:
	ldi r24,lo8(44)
	ldi r25,lo8(1)
	rjmp .L28
.L70:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	_Z10find_morsev, .-_Z10find_morsev
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,193
	ori r24,lo8(24)
	sts 193,r24
	lds r24,194
	ori r24,lo8(6)
	sts 194,r24
	sts 197,__zero_reg__
	ldi r24,lo8(9)
	sts 196,r24
	lds r24,193
	ori r24,lo8(-112)
	sts 193,r24
	sts 177,__zero_reg__
	ldi r24,lo8(-126)
	sts 178,r24
	out 0x17,__zero_reg__
	ldi r24,lo8(1)
	sts 112,r24
	sts 176,__zero_reg__
	ldi r24,lo8(5)
	sts 177,r24
	ldi r24,lo8(-1)
	out 0x4,r24
	out 0x5,__zero_reg__
/* #APP */
 ;  271 "main.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r28,lo8(buffer)
	ldi r29,hi8(buffer)
	ldi r17,lo8(-1)
.L74:
	lds r24,received_char
	cpi r24,lo8(-1)
	breq .L72
	lds r24,received_char
	lds r30,buffer+10
	lds r31,buffer+10+1
	movw r18,r30
	subi r18,-1
	sbci r19,-1
	sts buffer+10+1,r19
	sts buffer+10,r18
	st Z,r24
	lds r24,buffer+16
	lds r25,buffer+16+1
	adiw r24,1
	sts buffer+16+1,r25
	sts buffer+16,r24
	lds r18,buffer+10
	lds r19,buffer+10+1
	lds r24,buffer+14
	lds r25,buffer+14+1
	cp r18,r24
	cpc r19,r25
	brne .L73
	sts buffer+10+1,r29
	sts buffer+10,r28
.L73:
	sts received_char,r17
.L72:
	lds r24,buffer+16
	lds r25,buffer+16+1
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	brge .L74
	call _Z10find_morsev
	rjmp .L74
	.size	main, .-main
	.text
.global	__vector_18
	.type	__vector_18, @function
__vector_18:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r24
	push r25
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 9 */
.L__stack_usage = 9
	lds r24,198
	lds r30,buffer+10
	lds r31,buffer+10+1
	movw r18,r30
	subi r18,-1
	sbci r19,-1
	sts buffer+10+1,r19
	sts buffer+10,r18
	st Z,r24
	lds r24,buffer+16
	lds r25,buffer+16+1
	adiw r24,1
	sts buffer+16+1,r25
	sts buffer+16,r24
	lds r18,buffer+10
	lds r19,buffer+10+1
	lds r24,buffer+14
	lds r25,buffer+14+1
	cp r18,r24
	cpc r19,r25
	brne .L80
	ldi r24,lo8(buffer)
	ldi r25,hi8(buffer)
	sts buffer+10+1,r25
	sts buffer+10,r24
.L80:
/* epilogue start */
	pop r31
	pop r30
	pop r25
	pop r24
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_18, .-__vector_18
.global	__vector_9
	.type	__vector_9, @function
__vector_9:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
	push r25
	push r26
	push r27
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 7 */
.L__stack_usage = 7
	lds r24,timeInMillis
	lds r25,timeInMillis+1
	lds r26,timeInMillis+2
	lds r27,timeInMillis+3
	adiw r24,1
	adc r26,__zero_reg__
	adc r27,__zero_reg__
	sts timeInMillis,r24
	sts timeInMillis+1,r25
	sts timeInMillis+2,r26
	sts timeInMillis+3,r27
/* epilogue start */
	pop r27
	pop r26
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_9, .-__vector_9
	.section	.text.startup
	.type	_GLOBAL__sub_I_timeInMillis, @function
_GLOBAL__sub_I_timeInMillis:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(buffer)
	ldi r31,hi8(buffer)
	std Z+11,r31
	std Z+10,r30
	std Z+13,r31
	std Z+12,r30
	ldi r24,lo8(buffer+10)
	ldi r25,hi8(buffer+10)
	std Z+15,r25
	std Z+14,r24
	std Z+17,__zero_reg__
	std Z+16,__zero_reg__
	ret
	.size	_GLOBAL__sub_I_timeInMillis, .-_GLOBAL__sub_I_timeInMillis
	.global __do_global_ctors
	.section .ctors,"a",@progbits
	.p2align	1
	.word	gs(_GLOBAL__sub_I_timeInMillis)
	.section	.text.exit,"ax",@progbits
	.type	_GLOBAL__sub_D_timeInMillis, @function
_GLOBAL__sub_D_timeInMillis:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ret
	.size	_GLOBAL__sub_D_timeInMillis, .-_GLOBAL__sub_D_timeInMillis
.global	ledTime
	.section .bss
	.type	ledTime, @object
	.size	ledTime, 4
ledTime:
	.zero	4
.global	buffer
	.type	buffer, @object
	.size	buffer, 18
buffer:
	.zero	18
.global	received_char
	.data
	.type	received_char, @object
	.size	received_char, 1
received_char:
	.byte	-1
.global	timeInMillis
	.section .bss
	.type	timeInMillis, @object
	.size	timeInMillis, 4
timeInMillis:
	.zero	4
	.ident	"GCC: (GNU) 4.9.2"
.global __do_copy_data
.global __do_clear_bss
