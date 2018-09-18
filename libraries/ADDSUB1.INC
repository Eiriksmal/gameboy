;*
;* 8 & 16-bit Add / Subtract Macros
;*
;* V1.0 - Original Release - Jeff Frohwein
;* V1.1 - Fixed bug in ADD16R Addr,rp macro reported by Sam Nova
;* V1.2 - Added more efficient ADD8IR macro from Sam Nova
;*
;* Library Macros:
;*
;* add8 ADDR1,ADDR2
;*    Add ADDR2 to ADDR1.
;*
;* add8r ADDR1,ADDR2
;*    Add ADDR2 to ADDR1.
;*
;* add8i ADDR,VALUE
;*    Add 8-bit immediate VALUE to ADDR.
;*
;* add8ir ADDR,VALUE
;*    Add 8-bit immediate VALUE to ADDR.
;*
;*
;* add16 RP1,RP2
;* add16 RP1,ADDR2
;* add16 ADDR1,RP2
;* add16 ADDR1,ADDR2
;*    Add RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* add16r RP1,RP2
;* add16r RP1,ADDR2
;* add16r ADDR1,RP2
;* add16r ADDR1,ADDR2
;*    Add RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* add16i RP,VALUE
;*   Add 16-bit immediate VALUE to register contents of RP.
;*
;* add16i ADDR,VALUE
;*   Add 16-bit immediate VALUE to register contents of ADDR.
;*
;* add16ir RP,VALUE
;*   Add 16-bit immediate VALUE to register contents of RP.
;*
;* add16ir ADDR,VALUE
;*   Add 16-bit immediate VALUE to register contents of ADDR.
;*
;*
;*
;* sub8 ADDR1,ADDR2
;*    Subtract ADDR2 from ADDR1.
;*
;* sub8r ADDR1,ADDR2
;*    Subtract ADDR2 from ADDR1.
;*
;* sub8i ADDR,VALUE
;*    Subtract 8-bit immediate VALUE from ADDR.
;*
;* sub8ir ADDR,VALUE
;*    Subtract 8-bit immediate VALUE from ADDR.
;*
;*
;* sub16 RP1,RP2
;* sub16 RP1,ADDR2
;* sub16 ADDR1,RP2
;* sub16 ADDR1,ADDR2
;*    Subtract from RP1 (bc,de, or hl) or ADDR1 the
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* sub16r RP1,RP2
;* sub16r RP1,ADDR2
;* sub16r ADDR1,RP2
;* sub16r ADDR1,ADDR2
;*    Subtract from RP1 (bc,de, or hl) or ADDR1 the
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* sub16i  RP,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of RP.
;*
;* sub16i  ADDR,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of ADDR.
;*
;* sub16ir RP,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of RP.
;*
;* sub16ir ADDR,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of ADDR.
;*
;*   The small 'r' at the end of some macros above
;*  indicates a raw add and register A is destroyed.
;*  Raw adds are slightly faster/smaller macros.

; Add & subtract values less than 6 use INC (example):
;
;   add16i  bc,4   -> inc bc/inc bc/inc bc inc bc
;   add16i  de,-2  -> dec de/dec de
;   sub16i  hl,2   -> dec hl/dec hl
;
; Add / subtract values less than 256 use the following code:
;
;   add16ir bc,40  ->  ld a,c/add 40/ld c,a/jr nc,.skip/inc b/skip:
;   add16ir bc,40  ->  ld a,c/add 40/ld c,a/jr nc,.skip/inc b/skip:
;   sub16ir bc,40  ->  ld a,c/sub 40/ld c,a/jr nc,.skip/dec b/skip:
;
; Add / subtract values of 256, 512, 768, 1024 use this:
;
;   add16i bc,256   ->  inc b
;   sub16i de,512   ->  dec d/dec d
;
; Add / subtract values of larger 256 multiples use:
;
;   add16ir hl,2048  -> ld a,h/add 8/ld h,a
;   sub16ir de,2304  -> ld a,d/sub 9/ld d,a
;
; All others use the unoptimized code:
;
;   add16ir de,258  -> ld a,e/add 2/ld e,a/ld a,d/adc 1/ld d,a
;   add16i  de,258  -> push af/ld a,e/add 2/ld e,a/ld a,d/adc 1/ld d,a/pop af
;   sub16ir bc,258  -> ld a,c/sub 2/ld c,a/ld a,b/sbc 1/ld b,a

rev_Check_addsub1_inc: MACRO
;NOTE: REVISION NUMBER CHANGES MUST BE ADDED
;TO SECOND PARAMETER IN FOLLOWING LINE.
        IF      \1 > 1      ; <---- NOTE!!! PUT FILE REVISION NUMBER HERE
        WARN    "Version \1 or later of 'addsub1.inc' is required."
        ENDC
        ENDM

;* add8 ADDR1,ADDR2
;*    Add ADDR2 to ADDR1.

add8:  MACRO
        push    af
        push    hl
        ld      hl,\1
        ld      a,[\2]
        add     [hl]
        ld      [hl],a
        pop     hl
        pop     af
        ENDM

;* add8r ADDR1,ADDR2
;*    Add ADDR2 to ADDR1.

add8r:  MACRO
        push    hl
        ld      hl,\1
        ld      a,[\2]
        add     [hl]
        ld      [hl],a
        pop     hl
        ENDM

;* add8i ADDR,VALUE
;*    Add 8-bit immediate VALUE to ADDR.

add8i:  MACRO

        IF      (\2 != 0)

          IF      (\2 == 1) || (\2 == 2)
            push    hl
            ld      hl,\1

            REPT    \2
            inc     [hl]
            ENDR

            pop     hl
          ELSE
            IF      (\2 == -1) || (\2 == -2)
              push    hl
              ld      hl,\1

              REPT    (-(\2))
              dec     [hl]
              ENDR

              pop     hl

            ELSE
              push    af
              ld      a,[\1]
              add     \2
              ld      [\1],a
              pop     af
            ENDC
          ENDC
        ENDC

        ENDM

;* add8ir ADDR,VALUE
;*    Add 8-bit immediate VALUE to ADDR.


add8ir: MACRO

         IF      (\2 != 0)
           ld      a,[\1]
           IF  (\2 == 1)
             inc   a
           ELSE
             IF (\2 == -1)
               dec   a
             ELSE
               add     \2
             ENDC
           ENDC
           ld      [\1],a
         ENDC

         ENDM

;* add16 RP1,RP2
;* add16 RP1,ADDR2
;* add16 ADDR1,RP2
;* add16 ADDR1,ADDR2
;*    Add RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

add16:   MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")

__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; ADD16 rp,?

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; ADD16 rp,rp

            push    af
            ld      a,__d2
            add     __s2
            ld      __d2,a

            ld      a,__d1
            adc     __s1
            ld      __d1,a
            pop     af

          ELSE
; ADD16 rp,Addr

            push    af
            ld      a,[\2]
            add     __d2
            ld      __d2,a

            ld      a,[\2+1]
            adc     __d1
            ld      __d1,a
            pop     af

          ENDC
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; ADD16 Addr,rp

            push    af
            ld      a,[\1]
            add     __s2
            ld      [\1],a

            ld      a,[\1+1],a
            adc     __s1
            ld      [\1+1],a
            pop     af

          ELSE
; ADD16 Addr,Addr

            push    af
            push    hl
            ld      hl,\2
            ld      a,[hl+]
            ld      h,[hl]
            ld      l,a

            ld      a,[\1]
            add     l
            ld      [\1],a

            ld      a,[\1+1]
            adc     h
            ld      [\1+1],a
            pop     hl
            pop     af

          ENDC
        ENDC

        PURGE   __p1,__p2,__s1,__s2,__d1,__d2

        ENDM


;* add16r RP1,RP2
;* add16r RP1,ADDR2
;* add16r ADDR1,RP2
;* add16r ADDR1,ADDR2
;*    Add RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

add16r:   MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")

__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; ADD16R rp,?

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; ADD16R rp,rp

            ld      a,__d2
            add     __s2
            ld      __d2,a

            ld      a,__d1
            adc     __s1
            ld      __d1,a

          ELSE
; ADD16R rp,Addr

            ld      a,[\2]
            add     __d2
            ld      __d2,a

            ld      a,[\2+1]
            adc     __d1
            ld      __d1,a

          ENDC
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; ADD16R Addr,rp

            ld      a,[\1]
            add     __s2
            ld      [\1],a

            ld      a,[\1+1]
            adc     __s1
            ld      [\1+1],a

          ELSE
; ADD16R Addr,Addr

            push    hl
            ld      hl,\2
            ld      a,[hl+]
            ld      h,[hl]
            ld      l,a

            ld      a,[\1]
            add     l
            ld      [\1],a

            ld      a,[\1+1]
            adc     h
            ld      [\1+1],a
            pop     hl

          ENDC
        ENDC

        PURGE   __p1,__p2,__s1,__s2,__d1,__d2

        ENDM

;* add16i RP,VALUE
;*   Add 16-bit immediate VALUE to register contents of RP.

add16i: MACRO
__p1    EQUS    STRLWR("\1")

__INCDEC EQU    6

        IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)

; add16i RP,VALUE

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF (\2 < -255) || (\2 > 255)

            IF ( ((\2 > 255) && ((\2 & $ff) > 0) ) || ((\2 < -255) && (-(\2) & $ff) > 0))

; 16 bit immediate value

              IF (STRCMP("{__p1}","hl")==0)
                push    de
                ld      de,\2
                add     hl,de
                pop     de
              ELSE
                push    af
                ld      a,__d2
                add     (\2) & $ff
                ld      __d2,a

                ld      a,__d1
                adc     (\2) >> 8
                ld      __d1,a
                pop     af
              ENDC

            ELSE

              IF (\2 > 0)

                IF ((\2 >> 8) > 4)

; 16 bit immediate value of $xx00

                  push    af
                  ld      a,__d1
                  add     (\2) >> 8
                  ld      __d1,a
                  pop     af

                ELSE

; Use INC x instead of addition.

                  REPT    (\2 >> 8)
                  inc     __d1
                  ENDR

                ENDC

              ELSE

                IF ((-(\2) >> 8) > 4)

; 16 bit immediate value of -$xx00

                  push    af
                  ld      a,__d1
                  sub     ((-(\2)) >> 8)
                  ld      __d1,a
                  pop     af

                ELSE

; Use DEC x instead of addition.

                  REPT    (-(\2) >> 8)
                  dec     __d1
                  ENDR

                ENDC

              ENDC
            ENDC
          ELSE

            IF (\2 > 0)

              IF (\2 > __INCDEC)

; positive 8 bit immediate value

            push    af
            ld      a,__d2
            add     (\2) & $ff
            ld      __d2,a

            jr      nc,.nocarry\@
            inc     __d1
.nocarry\@: pop     af

              ELSE

; Use INC xx instead of addition.

              REPT    \2
              inc     \1
              ENDR

              ENDC

            ELSE
              IF (\2 < 0)

                IF (\2 < -(__INCDEC))

; negative 8 bit immediate value

                push    af
                ld      a,__d2
                sub     (-(\2))
                ld      __d2,a

                jr      nc,.nocarry\@
                dec     __d1
.nocarry\@:     pop     af

                ELSE

; Use DEC xx instead of addition.

                REPT    -(\2)
                dec     \1
                ENDR

                ENDC
              ENDC
            ENDC
          ENDC

          PURGE   __d1,__d2

        ELSE

; add16i ADDR,VALUE

            IF ( ((\2 > 0) && ((\2 & $ff) > 0) ) || ((\2 < 0) && (-(\2) & $ff) > 0) || (\2 == 0) )

            IF (\2 != 0)

; 16 bit immediate value

              push    af
              ld      a,[\1]
              add     (\2) & $ff
              ld      [\1],a

              ld      a,[\1+1]
              adc     (\2) >> 8
              ld      [\1+1],a
              pop     af

            ENDC

          ELSE

            IF (\2 > 0)

              IF (\2 == 256)

; 16 bit immediate value of $0100

                push    af
                ld      a,[\1+1]
                inc     a
                ld      [\1+1],a
                pop     af

              ELSE

; 16 bit immediate value of $xx00

                push    af
                ld      a,[\1+1]
                add     (\2) >> 8
                ld      [\1+1],a
                pop     af

              ENDC

            ELSE

              IF (\2 == -256)

; 16 bit immediate value of -$xx00

                push    af
                ld      a,[\1+1]
                dec     a
                ld      [\1+1],a
                pop     af

              ELSE

; 16 bit immediate value of -$xx00

                push    af
                ld      a,[\1+1]
                sub     ((-(\2)) >> 8)
                ld      [\1+1],a
                pop     af

              ENDC
            ENDC
          ENDC
        ENDC

        PURGE __p1,__INCDEC

        ENDM

;* add16ir RP,VALUE
;*   Add 16-bit immediate VALUE to register contents of RP.

add16ir: MACRO
__p1    EQUS    STRLWR("\1")

__INCDEC EQU    6

        IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)

; add16ir RP,VALUE

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF (\2 < -255) || (\2 > 255)

            IF ( ((\2 > 255) && ((\2 & $ff) > 0) ) || ((\2 < -255) && (-(\2) & $ff) > 0))

; 16 bit immediate value

              ld      a,__d2
              add     (\2) & $ff
              ld      __d2,a

              ld      a,__d1
              adc     (\2) >> 8
              ld      __d1,a

            ELSE

              IF (\2 > 0)

                IF ((\2 >> 8) > 4)

; 16 bit immediate value of $xx00

                  ld      a,__d1
                  add     (\2) >> 8
                  ld      __d1,a

                ELSE

; Use INC x instead of addition.

                  REPT    (\2 >> 8)
                  inc     __d1
                  ENDR

                ENDC

              ELSE

                IF ((-(\2) >> 8) > 4)

; 16 bit immediate value of -$xx00

                  ld      a,__d1
                  sub     ((-(\2)) >> 8)
                  ld      __d1,a

                ELSE

; Use DEC x instead of addition.

                  REPT    (-(\2) >> 8)
                  dec     __d1
                  ENDR

                ENDC

              ENDC
            ENDC
          ELSE

            IF (\2 > 0)

              IF (\2 > __INCDEC)

; positive 8 bit immediate value

            ld      a,__d2
            add     (\2) & $ff
            ld      __d2,a

            jr      nc,.nocarry\@
            inc     __d1
.nocarry\@
              ELSE

; Use INC xx instead of addition.

              REPT    \2
              inc     \1
              ENDR

              ENDC

            ELSE
              IF (\2 < 0)

                IF (\2 < -(__INCDEC))

; negative 8 bit immediate value

                ld      a,__d2
                sub     (-(\2))
                ld      __d2,a

                jr      nc,.nocarry\@
                dec     __d1
.nocarry\@
                ELSE

; Use DEC xx instead of addition.

                REPT    -(\2)
                dec     \1
                ENDR

                ENDC
              ENDC
            ENDC
          ENDC

          PURGE   __d1,__d2

        ELSE

; add16ir ADDR,VALUE

          IF ( ((\2 > 0) && ((\2 & $ff) > 0) ) || ((\2 < 0) && (-(\2) & $ff) > 0) || (\2 == 0) )

            IF (\2 != 0)

; 16 bit immediate value

              ld      a,[\1]
              add     (\2) & $ff
              ld      [\1],a

              ld      a,[\1+1]
              adc     (\2) >> 8
              ld      [\1+1],a

            ENDC

          ELSE

            IF (\2 > 0)

              IF (\2 == 256)

; 16 bit immediate value of $0100

                ld      a,[\1+1]
                inc     a
                ld      [\1+1],a

              ELSE

; 16 bit immediate value of $xx00

                ld      a,[\1+1]
                add     (\2) >> 8
                ld      [\1+1],a

              ENDC

            ELSE

              IF (\2 == -256)

; 16 bit immediate value of -$xx00

                ld      a,[\1+1]
                dec     a
                ld      [\1+1],a

              ELSE

; 16 bit immediate value of -$xx00

                ld      a,[\1+1]
                sub     ((-(\2)) >> 8)
                ld      [\1+1],a

              ENDC
            ENDC
          ENDC
        ENDC

        PURGE __p1,__INCDEC

        ENDM

;* sub8 ADDR1,ADDR2
;*    Subtract ADDR2 from ADDR1.

sub8:  MACRO
        push    af
        push    hl
        ld      hl,\2
        ld      a,[\1]
        sub     [hl]
        ld      [\1],a
        pop     hl
        pop     af
        ENDM

;* sub8r ADDR1,ADDR2
;*    Subtract ADDR2 from ADDR1.

sub8r:  MACRO
        push    hl
        ld      hl,\2
        ld      a,[\1]
        sub     [hl]
        ld      [\1],a
        pop     hl
        ENDM

;* sub8i ADDR,VALUE
;*    Subtract 8-bit immediate VALUE from ADDR.

sub8i:  MACRO

        IF      (\2 != 0)

          IF      (\2 == 1) || (\2 == 2)
            push    hl
            ld      hl,\1

            REPT    \2
            dec     [hl]
            ENDR

            pop     hl
          ELSE
            IF      (\2 == -1) || (\2 == -2)
              push    hl
              ld      hl,\1

              REPT    (-(\2))
              inc     [hl]
              ENDR

              pop     hl

            ELSE
              push    af
              ld      a,[\1]
              sub     \2
              ld      [\1],a
              pop     af
            ENDC
          ENDC
        ENDC

        ENDM

;* sub8ir ADDR,VALUE
;*    Subtract 8-bit immediate VALUE from ADDR.

sub8ir: MACRO

        IF      (\2 != 0)
          ld      a,[\1]
          sub     \2
          ld      [\1],a
        ENDC

        ENDM

;* sub16 RP1,RP2
;* sub16 RP1,ADDR2
;* sub16 ADDR1,RP2
;* sub16 ADDR1,ADDR2
;*    Subtract from RP1 (bc,de, or hl) or ADDR1 the
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

sub16:   MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")

__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; SUB16 rp,?

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; SUB16 rp,rp

            push    af
            ld      a,__d2
            sub     __s2
            ld      __d2,a

            ld      a,__d1
            sbc     __s1
            ld      __d1,a
            pop     af

          ELSE
; SUB16 rp,Addr

            IF      (STRCMP("{__p1}","hl")==0)
              push    af
              push    bc
              ld      a,[\2]
              ld      c,a
              ld      a,[\2+1]
              ld      b,a

              ld      a,__d2
              sub     c
              ld      __d2,a

              ld      a,__d1
              sbc     b
              ld      __d1,a
              pop     bc
              pop     af
            ELSE
              push    af
              push    hl
              ld      hl,\2
              ld      a,[hl+]
              ld      h,[hl]
              ld      l,a

              ld      a,__d2
              sub     l
              ld      __d2,a

              ld      a,__d1
              sbc     h
              ld      __d1,a
              pop     hl
              pop     af
            ENDC

          ENDC
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; SUB16 Addr,rp
            push    af
            ld      a,[\1]
            sub     __s2
            ld      [\1],a

            ld      a,[\1+1]
            sbc     __s1
            ld      [\1+1],a
            pop     af
          ELSE
; SUB16 Addr,Addr
            push    af
            push    hl
            ld      hl,\2
            ld      a,[hl+]
            ld      h,[hl]
            ld      l,a

            ld      a,[\1]
            sub     l
            ld      [\1],a

            ld      a,[\1+1]
            sbc     h
            ld      [\1+1],a
            pop     hl
            pop     af
          ENDC
        ENDC

        PURGE   __p1,__p2,__s1,__s2,__d1,__d2

        ENDM

;* sub16r RP1,RP2
;* sub16r RP1,ADDR2
;* sub16r ADDR1,RP2
;* sub16r ADDR1,ADDR2
;*    Subtract from RP1 (bc,de, or hl) or ADDR1 the
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

sub16r:   MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")

__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; SUB16R rp,?

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; SUB16R rp,rp

            ld      a,__d2
            sub     __s2
            ld      __d2,a

            ld      a,__d1
            sbc     __s1
            ld      __d1,a

          ELSE
; SUB16R rp,Addr

            IF      (STRCMP("{__p1}","hl")==0)
              push    bc
              ld      a,[\2]
              ld      c,a
              ld      a,[\2+1]
              ld      b,a

              ld      a,__d2
              sub     c
              ld      __d2,a

              ld      a,__d1
              sbc     b
              ld      __d1,a
              pop     bc
            ELSE
              push    hl
              ld      hl,\2
              ld      a,[hl+]
              ld      h,[hl]
              ld      l,a

              ld      a,__d2
              sub     l
              ld      __d2,a

              ld      a,__d1
              sbc     h
              ld      __d1,a
              pop     hl
            ENDC

          ENDC
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; SUB16R Addr,rp

            ld      a,[\1]
            sub     __s2
            ld      [\1],a

            ld      a,[\1+1]
            sbc     __s1
            ld      [\1+1],a

          ELSE
; SUB16R Addr,Addr

            push    hl
            ld      hl,\2
            ld      a,[hl+]
            ld      h,[hl]
            ld      l,a

            ld      a,[\1]
            sub     l
            ld      [\1],a

            ld      a,[\1+1]
            sbc     h
            ld      [\1+1],a
            pop     hl

          ENDC
        ENDC

        PURGE   __p1,__p2,__s1,__s2,__d1,__d2

        ENDM

;* sub16i  RP,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of RP.

sub16i: MACRO
        add16i   \1,-(\2)
        ENDM

;* sub16ir RP,VALUE
;*   Subtract 16-bit immediate VALUE from register contents of RP.

sub16ir: MACRO
        add16ir  \1,-(\2)
        ENDM
