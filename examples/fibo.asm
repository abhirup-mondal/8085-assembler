; generates Fibonacci numbers iteratively
; the Nth such number is stored at 2050h
; the value of N is provided at 2060h 
        LDA 2060h	; Load the Accumulator with 2060h
        MOV C A		; Move the content of Accumulator to C register
        MVI A 00h	; Initialize A register with immediate data 00h
        MVI B 01h	; Initialize B register with immediate data 01h
        DCR C		; Decrement the content of C register
        JZ LOOP1	; Jump on zero to Loop1
LOOP:   MOV D B		; Move the content of B register to D register
        ADD B		; Add the content of B with the Accumulator
        MOV B A		; Move the content of Accumulator to B register
        MOV A D		; Move the content of D register to Accumulator
        DCR C		; Decrement the content of C register
        JNZ LOOP	; Jump on no zero to Loop
LOOP1:  STA 2050h	; Store the content of Accumulator in 2050h
        RST 0		; Control to Monitor

