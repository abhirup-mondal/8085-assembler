; calculates 1! through 10!, dynamically allocating bytes for the result
; maximum 3-bytes
; result stored at the end of memory
INIT:   MVI C 02H       ; start with 2!
        LXI H FFEFH     ; starting location of output
        MVI M 01H       ; 1! = 1
        INX H
        MVI M 00H
        INX H
        MVI M 00H
        INX H
        MVI M 00H
        XCHG

        LXI H FFEBH     ; starting location of working space
        MVI M 01H       ; 1! = 1
        INX H
        MVI M 00H
        INX H
        MVI M 00H
        DCX H           ; re-align pointer
        DCX H

FACT:   MOV B C         ; begin factorial calculation

ADDT:   MOV A M         ; begin addition of 3 byte numbers
        XCHG
        DCX H           ; re-align
        DCX H
        ADD M           ; add lower BYTE
        MOV M A         ; update
        INX H
        XCHG
        INX H

        MOV A M
        XCHG
        ADC M           ; add middle BYTE
        MOV M A
        INX H
        XCHG
        INX H

        MOV A M
        XCHG
        ADC M           ; add upper BYTE
        MOV M A
        XCHG

        DCX H
        DCX H
        DCR B
        JNZ ADDT        ; add N times, i.e. multiply by N

        XCHG
        DCX H           ; re-align pointer
        DCX H

COPY:   MOV A M         ; copy latest result from working area...
        XCHG            ; ...to output area
        MOV M A         ; lower BYTE
        INX H
        XCHG
        INX H

        MOV A M         ; middle BYTE
        XCHG
        MOV M A
        INX H
        XCHG
        INX H

        MOV A M         ; upper BYTE
        XCHG
        MOV M A
        DCX H
        DCX H

        XCHG
TRUNK:  MOV A M         ; truncate bytes with value 0
        ANA A
        JNZ NEXT
        DCX H           ; starting with upper BYTE
        JMP TRUNK

NEXT:   XCHG
        INR C
        MVI A 0AH       ; if 10! calculated
        CMP C
        JC END          ; program finished, jump to end
        XCHG
        INX H
        MVI M 00H       ; initialize space for next value
        INX H
        MVI M 00H
        INX H
        MVI M 00H
        XCHG
        JMP FACT        ; calculate next factorial


END:    LXI H FFEBH     ; clear working space
        MVI M 00H
        INX H
        MVI M 00H
        INX H
        MVI M 00H
        HLT
        
