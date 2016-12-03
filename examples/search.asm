; N numbers are Stored in consecutive m/m location 
; starting from 2050h. The value of N is stored in 204Fh.
; Write a program to test whether a number is stored
; in 204Eh is present in the list. If present, 
; store it’s position in the list at 204Dh otherwise store FFh. 
 
        MVI E 00h	; Initialize the content of E register with 00h
        LXI H 204Eh	; Load (H)(L) with memory location 204Eh
        MOV B M		; Move the content of M register to B register
        INX H		; Increment the content of (H)(L) register pair 
        MOV C M		; Move the content of M register to C register
        INX H		; Increment the content of (H)(L) register pair
LOOP:   MOV A M		; Move the content of M register to Accumulator
        INR E		; Increment the content of E register 
        CMP B		; Compare content of B register with A register
        JNZ LOOP1	; Jump on no zero to Loop1
        MOV A E		; Move the content of E register to A register
        STA 204Dh	; Store the content of Accumulator in 204Dh
        JMP END		; Unconditional Jump to End
LOOP1:  INX H		; Increment the content of (H)(L) register pair
        DCR C		; Decrement the content of C register
        JNZ LOOP	; Jump on no zero to Loop
        MVI A FFh	; Move immediate data FFh to Accumulator
        STA 204Dh	; Store the content of Accumulator in 204Dh
END:    RST 0		; Control to Monitor
