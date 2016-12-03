; recursively loads 10 9 8 7 6 5 4 3 2 1 1 2 3 4 5 6 7 8 9 10 to memory
; note the handling of global variables
		lxi sp ffffh ; initialize stack pointer
		mvi a 10 ; put some value in Accumulator
		lxi h 5
		call rout
		sta 0
		hlt

rout:   mov m a ; store value to memory
        inx h
        dcr a
        rz
        shld 0	; save global variable
        call rout ; recursive call
        lhld 0 ; load global variable
        mov m a
        inx h
        shld 0
        ret
