	.file	"prog.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	print_message
	.type	print_message, @function
print_message:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r30,r24
	0:
	ld __tmp_reg__,Z+
	tst __tmp_reg__
	brne 0b
	sbiw r30,1
	sub r30,r24
	sbc r31,r25
	movw r26,r24
.L2:
	movw r18,r26
	sub r18,r24
	sbc r19,r25
	cp r18,r30
	cpc r19,r31
	brge .L8
.L5:
	lds r18,192
	sbrs r18,5
	rjmp .L5
	ld r18,X+
	sts 198,r18
	rjmp .L2
.L8:
/* epilogue start */
	ret
	.size	print_message, .-print_message
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
.global	reset_players
	.type	reset_players, @function
reset_players:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,0
	ldi r25,0
.L11:
	lds r18,nro_players
	lds r19,nro_players+1
	cp r24,r18
	cpc r25,r19
	brge .L14
	movw r30,r24
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ld r18,Z
	ldd r19,Z+1
	cpi r18,1
	cpc r19,__zero_reg__
	brne .L12
	std Z+1,__zero_reg__
	st Z,__zero_reg__
.L12:
	adiw r24,1
	rjmp .L11
.L14:
/* epilogue start */
	ret
	.size	reset_players, .-reset_players
.global	get_not_played_players
	.type	get_not_played_players, @function
get_not_played_players:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r18,0
	ldi r19,0
	ldi r24,0
	ldi r25,0
.L16:
	lds r20,nro_players
	lds r21,nro_players+1
	cp r18,r20
	cpc r19,r21
	brge .L19
	movw r30,r18
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ld r20,Z
	ldd r21,Z+1
	or r20,r21
	brne .L17
	adiw r24,1
.L17:
	subi r18,-1
	sbci r19,-1
	rjmp .L16
.L19:
/* epilogue start */
	ret
	.size	get_not_played_players, .-get_not_played_players
.global	get_active_players
	.type	get_active_players, @function
get_active_players:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r18,0
	ldi r19,0
	ldi r24,0
	ldi r25,0
.L21:
	lds r20,nro_players
	lds r21,nro_players+1
	cp r18,r20
	cpc r19,r21
	brge .L27
	movw r30,r18
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ld r20,Z
	ldd r21,Z+1
	cpi r20,-1
	sbci r21,-1
	breq .L22
	adiw r24,1
.L22:
	subi r18,-1
	sbci r19,-1
	rjmp .L21
.L27:
/* epilogue start */
	ret
	.size	get_active_players, .-get_active_players
.global	find_winner
	.type	find_winner, @function
find_winner:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,0
	ldi r25,0
.L29:
	lds r18,nro_players
	lds r19,nro_players+1
	cp r24,r18
	cpc r25,r19
	brge .L33
	movw r30,r24
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ld r18,Z
	ldd r19,Z+1
	adiw r24,1
	cpi r18,-1
	sbci r19,-1
	breq .L29
	rjmp .L31
.L33:
	ldi r24,0
	ldi r25,0
.L31:
	ret
	.size	find_winner, .-find_winner
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Player %d\n"
	.text
.global	round_turn
	.type	round_turn, @function
round_turn:
	push r16
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,15
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 15 */
/* stack size = 19 */
.L__stack_usage = 19
	ldi r24,0
	ldi r25,0
.L35:
	lds r18,nro_players
	lds r19,nro_players+1
	cp r24,r18
	cpc r25,r19
	brge .L37
	movw r30,r24
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ld r18,Z
	ldd r19,Z+1
	or r18,r19
	brne .L36
	ldi r18,lo8(1)
	ldi r19,0
	std Z+1,r19
	st Z,r18
	sts current_player+1,r25
	sts current_player,r24
	rjmp .L37
.L36:
	adiw r24,1
	rjmp .L35
.L37:
	lds r24,current_player
	lds r25,current_player+1
	adiw r24,1
	push r25
	push r24
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	push r25
	push r24
	movw r16,r28
	subi r16,-1
	sbci r17,-1
	push r17
	push r16
	call sprintf
	movw r24,r16
	call print_message
	sts current_player_note+1,__zero_reg__
	sts current_player_note,__zero_reg__
	call get_millis
	sts player_time,r22
	sts player_time+1,r23
	sts player_time+2,r24
	sts player_time+3,r25
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
/* epilogue start */
	adiw r28,15
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	round_turn, .-round_turn
	.section	.rodata.str1.1
.LC1:
	.string	"\nWelcome to the follow the leader game! Please enter the number of players: [2-9]\n"
.LC2:
	.string	"Enter a valid number of players\n"
	.text
.global	round_reset
	.type	round_reset, @function
round_reset:
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	subi r28,116
	sbc r29,__zero_reg__
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 116 */
/* stack size = 119 */
.L__stack_usage = 119
	sts round_count+1,__zero_reg__
	sts round_count,__zero_reg__
	sts N+1,__zero_reg__
	sts N,__zero_reg__
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	sts current_player+1,r25
	sts current_player,r24
	sts current_player_note+1,__zero_reg__
	sts current_player_note,__zero_reg__
	sts nro_players+1,r25
	sts nro_players,r24
	sts interruptions,__zero_reg__
	sts interruptions+1,__zero_reg__
	sts interruptions+2,__zero_reg__
	sts interruptions+3,__zero_reg__
	ldi r24,0
	ldi r25,0
	ldi r18,lo8(-1)
	ldi r19,lo8(-1)
.L40:
	movw r30,r24
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	std Z+1,r19
	st Z,r18
	adiw r24,1
	cpi r24,9
	cpc r25,__zero_reg__
	brne .L40
	ldi r24,0
	ldi r25,0
	ldi r18,lo8(48)
.L41:
	movw r30,r24
	subi r30,lo8(-(notes))
	sbci r31,hi8(-(notes))
	st Z,r18
	adiw r24,1
	cpi r24,100
	cpc r25,__zero_reg__
	brne .L41
	ldi r24,lo8(83)
	ldi r30,lo8(.LC1)
	ldi r31,hi8(.LC1)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,1
	call print_message
	ldi r17,lo8(33)
.L42:
	lds r24,192
	sbrs r24,7
	rjmp .L42
	lds r24,198
	ldi r25,0
	sbiw r24,48
	sts nro_players+1,r25
	sts nro_players,r24
	lds r24,nro_players
	lds r25,nro_players+1
	sbiw r24,10
	brlt .L43
	lds r24,nro_players
	lds r25,nro_players+1
	sbiw r24,2
	brge .L43
	ldi r30,lo8(.LC2)
	ldi r31,hi8(.LC2)
	movw r26,r28
	subi r26,-84
	sbci r27,-1
	mov r24,r17
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	subi r24,-84
	sbci r25,-1
	call print_message
.L43:
	lds r24,nro_players
	lds r25,nro_players+1
	sbiw r24,2
	brlt .L42
	lds r24,nro_players
	lds r25,nro_players+1
	sbiw r24,10
	brge .L42
	ldi r24,0
	ldi r25,0
.L45:
	lds r18,nro_players
	lds r19,nro_players+1
	cp r24,r18
	cpc r25,r19
	brge .L50
	movw r30,r24
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	std Z+1,__zero_reg__
	st Z,__zero_reg__
	adiw r24,1
	rjmp .L45
.L50:
	call round_start
/* epilogue start */
	subi r28,-116
	sbci r29,-1
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	ret
	.size	round_reset, .-round_reset
	.section	.rodata.str1.1
.LC3:
	.string	"\nGame over! Player %d won\n"
.LC4:
	.string	"\nROUND = %d \n"
	.text
.global	round_start
	.type	round_start, @function
round_start:
	push r16
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,50
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 50 */
/* stack size = 54 */
.L__stack_usage = 54
	call get_active_players
	movw r16,r28
	subi r16,-1
	sbci r17,-1
	sbiw r24,1
	brne .L52
	call find_winner
	push r25
	push r24
	ldi r24,lo8(.LC3)
	ldi r25,hi8(.LC3)
	push r25
	push r24
	push r17
	push r16
	call sprintf
	movw r24,r16
	call print_message
	call round_reset
	rjmp .L55
.L52:
	call get_not_played_players
	or r24,r25
	brne .L54
	call reset_players
.L54:
	lds r24,round_count
	lds r25,round_count+1
	adiw r24,1
	sts round_count+1,r25
	sts round_count,r24
	lds r24,round_count
	lds r25,round_count+1
	push r25
	push r24
	ldi r24,lo8(.LC4)
	ldi r25,hi8(.LC4)
	push r25
	push r24
	push r17
	push r16
	call sprintf
	movw r24,r16
	call print_message
	call round_turn
.L55:
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
/* epilogue start */
	adiw r28,50
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	round_start, .-round_start
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cbi 0xa,2
	sbi 0xb,2
	lds r24,105
	ori r24,lo8(1)
	sts 105,r24
	sbi 0x1d,0
	cbi 0xa,3
	sbi 0xb,3
	lds r24,105
	ori r24,lo8(4)
	sts 105,r24
	sbi 0x1d,1
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
	lds r24,193
	ori r24,lo8(24)
	sts 193,r24
	lds r24,194
	ori r24,lo8(6)
	sts 194,r24
	sts 197,__zero_reg__
	ldi r24,lo8(103)
	sts 196,r24
/* #APP */
 ;  224 "prog.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	call round_reset
.L57:
	rjmp .L57
	.size	main, .-main
	.section	.rodata.str1.1
.LC5:
	.string	"Incorrect sequence! You have been eliminated.\n"
	.text
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
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
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,47
	out __SP_H__,r29
	out __SP_L__,r28
/* prologue: Signal */
/* frame size = 47 */
/* stack size = 66 */
.L__stack_usage = 66
	call get_millis
	lds r16,bounceTime
	lds r17,bounceTime+1
	lds r18,bounceTime+2
	lds r19,bounceTime+3
	or r16,r17
	or r16,r18
	or r16,r19
	breq .+2
	rjmp .L58
	sts player_time,r22
	sts player_time+1,r23
	sts player_time+2,r24
	sts player_time+3,r25
	sts bounceTime,r22
	sts bounceTime+1,r23
	sts bounceTime+2,r24
	sts bounceTime+3,r25
	lds r18,current_player_note
	lds r19,current_player_note+1
	lds r24,N
	lds r25,N+1
	cp r18,r24
	cpc r19,r25
	brne .L61
	lds r30,N
	lds r31,N+1
	subi r30,lo8(-(notes))
	sbci r31,hi8(-(notes))
	ldi r24,lo8(67)
	st Z,r24
	lds r24,N
	lds r25,N+1
	adiw r24,1
	sts N+1,r25
	sts N,r24
	lds r24,current_player_note
	lds r25,current_player_note+1
	adiw r24,1
	sts current_player_note+1,r25
	sts current_player_note,r24
	rjmp .L64
.L61:
	lds r18,current_player_note
	lds r19,current_player_note+1
	lds r24,N
	lds r25,N+1
	cp r18,r24
	cpc r19,r25
	brge .L58
	lds r30,current_player_note
	lds r31,current_player_note+1
	subi r30,lo8(-(notes))
	sbci r31,hi8(-(notes))
	ld r24,Z
	cpi r24,lo8(67)
	brne .L63
	lds r24,current_player_note
	lds r25,current_player_note+1
	adiw r24,1
	sts current_player_note+1,r25
	sts current_player_note,r24
	rjmp .L58
.L63:
	lds r30,current_player
	lds r31,current_player+1
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	std Z+1,r25
	st Z,r24
	ldi r24,lo8(47)
	ldi r30,lo8(.LC5)
	ldi r31,hi8(.LC5)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,1
	call print_message
.L64:
	call round_start
.L58:
/* epilogue start */
	adiw r28,47
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
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
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_1, .-__vector_1
.global	__vector_2
	.type	__vector_2, @function
__vector_2:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
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
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,47
	out __SP_H__,r29
	out __SP_L__,r28
/* prologue: Signal */
/* frame size = 47 */
/* stack size = 66 */
.L__stack_usage = 66
	call get_millis
	lds r16,bounceTime2
	lds r17,bounceTime2+1
	lds r18,bounceTime2+2
	lds r19,bounceTime2+3
	or r16,r17
	or r16,r18
	or r16,r19
	breq .+2
	rjmp .L65
	sts player_time,r22
	sts player_time+1,r23
	sts player_time+2,r24
	sts player_time+3,r25
	sts bounceTime2,r22
	sts bounceTime2+1,r23
	sts bounceTime2+2,r24
	sts bounceTime2+3,r25
	lds r18,current_player_note
	lds r19,current_player_note+1
	lds r24,N
	lds r25,N+1
	cp r18,r24
	cpc r19,r25
	brne .L68
	lds r30,N
	lds r31,N+1
	subi r30,lo8(-(notes))
	sbci r31,hi8(-(notes))
	ldi r24,lo8(70)
	st Z,r24
	lds r24,N
	lds r25,N+1
	adiw r24,1
	sts N+1,r25
	sts N,r24
	lds r24,current_player_note
	lds r25,current_player_note+1
	adiw r24,1
	sts current_player_note+1,r25
	sts current_player_note,r24
	rjmp .L71
.L68:
	lds r18,current_player_note
	lds r19,current_player_note+1
	lds r24,N
	lds r25,N+1
	cp r18,r24
	cpc r19,r25
	brge .L65
	lds r30,current_player_note
	lds r31,current_player_note+1
	subi r30,lo8(-(notes))
	sbci r31,hi8(-(notes))
	ld r24,Z
	cpi r24,lo8(70)
	brne .L70
	lds r24,current_player_note
	lds r25,current_player_note+1
	adiw r24,1
	sts current_player_note+1,r25
	sts current_player_note,r24
	rjmp .L65
.L70:
	lds r30,current_player
	lds r31,current_player+1
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	std Z+1,r25
	st Z,r24
	ldi r24,lo8(47)
	ldi r30,lo8(.LC5)
	ldi r31,hi8(.LC5)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,1
	call print_message
.L71:
	call round_start
.L65:
/* epilogue start */
	adiw r28,47
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
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
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_2, .-__vector_2
	.section	.rodata.str1.1
.LC6:
	.string	"Time expired. You have been eliminated!\n"
	.text
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
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,41
	out __SP_H__,r29
	out __SP_L__,r28
/* prologue: Signal */
/* frame size = 41 */
/* stack size = 62 */
.L__stack_usage = 62
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
	lds r24,bounceTime
	lds r25,bounceTime+1
	lds r26,bounceTime+2
	lds r27,bounceTime+3
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brge .L73
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
	cpi r24,45
	sbci r25,1
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlt .L73
	sts bounceTime,__zero_reg__
	sts bounceTime+1,__zero_reg__
	sts bounceTime+2,__zero_reg__
	sts bounceTime+3,__zero_reg__
.L73:
	lds r24,bounceTime2
	lds r25,bounceTime2+1
	lds r26,bounceTime2+2
	lds r27,bounceTime2+3
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brge .L74
	lds r24,bounceTime2
	lds r25,bounceTime2+1
	lds r26,bounceTime2+2
	lds r27,bounceTime2+3
	movw r14,r18
	movw r16,r20
	sub r14,r24
	sbc r15,r25
	sbc r16,r26
	sbc r17,r27
	movw r26,r16
	movw r24,r14
	cpi r24,45
	sbci r25,1
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlt .L74
	sts bounceTime2,__zero_reg__
	sts bounceTime2+1,__zero_reg__
	sts bounceTime2+2,__zero_reg__
	sts bounceTime2+3,__zero_reg__
.L74:
	lds r24,player_time
	lds r25,player_time+1
	lds r26,player_time+2
	lds r27,player_time+3
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	cpc __zero_reg__,r26
	cpc __zero_reg__,r27
	brge .L72
	lds r24,player_time
	lds r25,player_time+1
	lds r26,player_time+2
	lds r27,player_time+3
	movw r14,r18
	movw r16,r20
	sub r14,r24
	sbc r15,r25
	sbc r16,r26
	sbc r17,r27
	movw r26,r16
	movw r24,r14
	cpi r24,-119
	sbci r25,19
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brlt .L72
	sts player_time,__zero_reg__
	sts player_time+1,__zero_reg__
	sts player_time+2,__zero_reg__
	sts player_time+3,__zero_reg__
	lds r30,current_player
	lds r31,current_player+1
	lsl r30
	rol r31
	subi r30,lo8(-(players))
	sbci r31,hi8(-(players))
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	std Z+1,r25
	st Z,r24
	ldi r24,lo8(41)
	ldi r30,lo8(.LC6)
	ldi r31,hi8(.LC6)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	movw r24,r28
	adiw r24,1
	call print_message
	call round_start
.L72:
/* epilogue start */
	adiw r28,41
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
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
.global	current_player_note
	.section .bss
	.type	current_player_note, @object
	.size	current_player_note, 2
current_player_note:
	.zero	2
.global	current_player
	.data
	.type	current_player, @object
	.size	current_player, 2
current_player:
	.word	-1
.global	round_count
	.section .bss
	.type	round_count, @object
	.size	round_count, 2
round_count:
	.zero	2
.global	N
	.type	N, @object
	.size	N, 2
N:
	.zero	2
	.comm	notes,100,1
.global	player_time
	.type	player_time, @object
	.size	player_time, 4
player_time:
	.zero	4
.global	nro_players
	.type	nro_players, @object
	.size	nro_players, 2
nro_players:
	.zero	2
	.comm	players,18,1
.global	bounceTime2
	.type	bounceTime2, @object
	.size	bounceTime2, 4
bounceTime2:
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
	.ident	"GCC: (GNU) 4.9.2"
.global __do_copy_data
.global __do_clear_bss
