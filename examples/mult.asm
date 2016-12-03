; multiplies two numbers, located at 3000h and 3001h
; result is stored at 3002h and 3003h
		LXI H 3000H
		MOV B M
		INX H
		MOV C M
		MVI A 00H
		MVI D 00H
LOOP: 	ADD B
		JNC SKIP
		INR D
SKIP: 	DCR C
		JNZ LOOP
		INX H
		MOV M A
		INX H
		MOV M D
		HLT
		
