;*
;* 16/16/32 Bit Unsigned Math, Version A
;*
;* Initials: JF = Jeff Frohwein
;*
;* V1.0 - 15-Jul-97 : Original Release - JF
;* V1.1 - 27-Jul-97 : Modified for new subroutine prefixes - JF
;*

; If all of these are already defined, don't do it again.

        IF      !DEF(U161632A_ASM)
U161632A_ASM  SET  1

rev_Check_u161632a_asm: MACRO
;NOTE: REVISION NUMBER CHANGES MUST BE ADDED
;TO SECOND PARAMETER IN FOLLOWING LINE.
        IF      \1 > 1.1      ; <--- PUT REVISION NUMBER HERE
        WARN    "Version \1 or later of 'u161632a.asm' is required."
        ENDC
        ENDM

        section "U161632a Code",home

;*
;*  QMUL  -  Quick 16 x 16 Unsigned Multiply
;*
;*
;*         By:           Jon Tara
;*                       MicroControl Software
;*                       1300 E. Lafayette #2808
;*                       Detroit, Michigan  48207
;*                       (313) 393-2916
;*
;*        This software is placed in the public domain.
;*
;*
;*  On entry:  DE = Multiplicand
;*             HL = Multiplier
;*
;*  On exit:   BCDE = Product
;*
;*  AF destroyed
;*
;*  Revision 1.00  01/21/82   JT  Initial Release
;*  Revision 1.10  25/Apr/97  Bug fixes - Jeff Frohwein
;*

mul_DE_HL_BCDEu::
        push    hl              ; Save multiplier.
        ld      c,h             ; Save MSBs of multiplier.
        ld      a,l             ; LSBs to A for an 8 x 16 multiply.

        ld      b,0             ; Handy 0 to B for carry propagation.
        ld      h,b             ; Init LSBs of product.
        ld      l,b

        add     a,a             ; Test multiplier bit.
        jr      nc,.mul2
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul2   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul3
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul3   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul4
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul4   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul5
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul5   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul6
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul6   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul7
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul7   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul8
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)
.mul8   add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul9
        add     hl,de           ; Add multiplicand to product.
        adc     a,b             ; (Product in AHL)

.mul9   push    hl              ; Save LSBs in stack.
        ld      h,b             ; Zero second product.
        ld      l,b             ; .
        ld      b,a             ; Save MSBs of first product in B
        ld      a,c             ; Get MSBs of multiplier.
        ld      c,h             ; Handy 0 in C this time.

        add     a,a             ; Test multiplier bit.
        jr      nc,.mul20
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul20  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul21
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul21  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul22
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul22  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul23
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul23  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul24
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul24  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul25
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul25  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul26
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)
.mul26  add     hl,hl           ; Shift product left.
        adc     a,a             ; Test multiplier bit.
        jr      nc,.mul27
        add     hl,de           ; Add multiplicand to product.
        adc     a,c             ; (Product in AHL)

.mul27  pop     de              ; Fetch LSBs of 1st product.
        ld      c,a             ; Add partial products.
        ld      a,d             ; .
        add     a,l             ; .
        ld      d,a             ; .
        ld      a,b             ; .
        adc     a,h             ; .
        ld      h,a             ; .
        ld      a,c             ; .
        adc     a,0             ; .
        ld      b,a             ; .
        ld      c,h             ; .
        pop     hl              ; Restore multiplier.
        ret

        ENDC    ;u161632a_asm
