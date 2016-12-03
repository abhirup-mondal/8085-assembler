		lxi sp fffeh	; initialize stack pointer
		mvi a 01		; put some values in the registers
		mvi b 02
		mvi c 03
		mvi d 04
		mvi e 05
		lxi h 0607h
		call routine
		sta 1			; register data not changed
		hlt

routine: mvi a 08		; change data in registers
         mvi b 09
         mvi c 0ah
         mvi d 0bh
         mvi e 0ch
         lxi h 0e0dh
         sta 0
         ret

