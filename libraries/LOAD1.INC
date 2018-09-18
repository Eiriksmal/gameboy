;*
;* 8 & 16-bit Load Macros
;*
;*   Started 17-Jan-98
;*
;* Initials: JF = Jeff Frohwein
;*
;* V1.0 - 12-Mar-98 : Original Release - JF
;*
;* Library Macros:
;*
;* ld8 ADDR1,ADDR2 - 8 bytes, 60 cycles
;*   Copy 8-bit value from ADDR2 to ADDR1.
;*
;* ld8r ADDR1,ADDR2 - 6 bytes, 32 cycles
;*    Copy 8-bit value from ADDR2 to ADDR1.
;*
;* ld8i ADDR,VALUE - 7 bytes, 52 cycles
;*   Store 8-bit immediate VALUE to memory at ADDR.
;*
;* ld8ir ADDR,VALUE - 5 bytes, 24 cycles
;*   Store 8-bit immediate VALUE to memory at ADDR.
;*
;* ld16 RP1,RP2 - 2 bytes, 8 cycles
;* ld16 RP1,ADDR2 - 10 bytes, 68 cycles
;* ld16 ADDR1,RP2 - 10 bytes, 68 cycles
;* ld16 ADDR1,ADDR2 - 14 bytes, 92 cycles
;*    Load RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* ld16r RP1,ADDR2 - 8 bytes, 40 cycles
;* ld16r ADDR1,RP2 - 8 bytes, 40 cycles
;* ld16r ADDR1,ADDR2 - 12 bytes, 64 cycles
;*    Load RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.
;*
;* ld16i ADDR,VALUE - 10 bytes, 72 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.
;*
;* ld16ir ADDR,VALUE - 10 bytes, 48 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.
;*
;* ld16irr ADDR,VALUE - 8 bytes, 44 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.
;*
;* Note:
;*   The functions above have several macros that basically
;*  do the same thing but each has it's own advantages/disadvantages.
;*
;*   The small 'r' at the end of some macros above
;*  indicates a raw move and register A is destroyed.
;*  'rr' indicates register HL is destroyed.
;*

; If all of these are already defined, don't do it again.

        IF      !DEF(LOAD1_INC)
LOAD1_INC  SET  1

rev_Check_load1_inc: MACRO
;NOTE: REVISION NUMBER CHANGES MUST BE ADDED
;TO SECOND PARAMETER IN FOLLOWING LINE.
        IF      \1 > 1.0      ; <--- PUT REVISION NUMBER HERE
        WARN    "Version \1 or later of 'load1.inc' is required."
        ENDC
        ENDM

;* ld8 ADDR1,ADDR2 - 8 bytes, 60 cycles
;*    Copy 8-bit value from ADDR2 to ADDR1.

ld8:    MACRO
        push    af
        ld      a,[\2]
        ld      [\1],a
        pop     af
        ENDM

;* ld8r ADDR1,ADDR2 - 6 bytes, 32 cycles
;*    Copy 8-bit value from ADDR2 to ADDR1.

ld8r:   MACRO
        ld      a,[\2]
        ld      [\1],a
        ENDM

;* ld8i ADDR,VALUE - 7 bytes, 52 cycles
;*   Store 8-bit immediate VALUE to memory at ADDR.

ld8i:   MACRO
        push    af
        ld      a,\2
        ld      [\1],a
        pop     af
        ENDM

;* ld8ir ADDR,VALUE - 5 bytes, 24 cycles
;*   Store 8-bit immediate VALUE to memory at ADDR.

ld8ir:  MACRO
        ld      a,\2
        ld      [\1],a
        ENDM

;* ld16 RP1,RP2 - 2 bytes, 8 cycles
;* ld16 RP1,ADDR2 - 10 bytes, 68 cycles
;* ld16 ADDR1,RP2 - 10 bytes, 68 cycles
;* ld16 ADDR1,ADDR2 - 14 bytes, 92 cycles
;*    Load RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

ld16:   MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")
        IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; LD16 rp,?
__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; LD16 rp,rp
__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

            ld      __d2,__s2
            ld      __d1,__s1

            PURGE   __s1,__s2
          ELSE
; LD16 rp,Addr
            push    af
            ld      a,[\2]
            ld      __d2,a
            ld      a,[\2+1]
            ld      __d1,a
            pop     af
          ENDC

          PURGE   __d1,__d2
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; LD16 Addr,rp
__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

            push    af
            ld      a,__s2
            ld      [\1],a
            ld      a,__s1
            ld      [\1+1],a
            pop     af

            PURGE   __s1,__s2
          ELSE
; LD16 Addr,Addr
            push    af
            ld      a,[\2]
            ld      [\1],a
            ld      a,[\2+1]
            ld      [\1+1],a
            pop     af
          ENDC
        ENDC

        PURGE   __p1,__p2

        ENDM

;* ld16r RP1,ADDR2 - 8 bytes, 40 cycles
;* ld16r ADDR1,RP2 - 8 bytes, 40 cycles
;* ld16r ADDR1,ADDR2 - 12 bytes, 64 cycles
;*    Load RP1 (bc,de, or hl) or ADDR1 with
;*   16-bit contents of RP2 (bc,de, or hl) or ADDR2.

ld16r:  MACRO
__p1    EQUS    STRLWR("\1")
__p2    EQUS    STRLWR("\2")
        IF      (STRCMP("{__p1}","bc")==0) || (STRCMP("{__p1}","de")==0) || (STRCMP("{__p1}","hl")==0)
; LD16r rp,?
__d1    EQUS    STRSUB("\1",1,1)
__d2    EQUS    STRSUB("\1",2,1)

          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; LD16r rp,rp

            FAIL   "Error: Use 'ld16 rp,rp' instead."

          ELSE
; LD16r rp,Addr
            ld      a,[\2]
            ld      __d2,a
            ld      a,[\2+1]
            ld      __d1,a
          ENDC

          PURGE   __d1,__d2
        ELSE
          IF      (STRCMP("{__p2}","bc")==0) || (STRCMP("{__p2}","de")==0) || (STRCMP("{__p2}","hl")==0)
; LD16r Addr,rp
__s1    EQUS    STRSUB("\2",1,1)
__s2    EQUS    STRSUB("\2",2,1)

            ld      a,__s2
            ld      [\1],a
            ld      a,__s1
            ld      [\1+1],a

            PURGE   __s1,__s2
          ELSE
; LD16r Addr,Addr
            ld      a,[\2]
            ld      [\1],a
            ld      a,[\2+1]
            ld      [\1+1],a
          ENDC
        ENDC

        PURGE   __p1,__p2

        ENDM

;* ld16i ADDR,VALUE - 10 bytes, 72 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.

ld16i:  MACRO
        push    hl
        ld      hl,\1
        ld      [hl],(\2) & $ff
        inc     hl
        ld      [hl],(\2) >> 8
        pop     hl
        ENDM

;* ld16ir ADDR,VALUE - 10 bytes, 48 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.

ld16ir: MACRO
        ld      a,(\2) & $ff
        ld      [\1],a
        ld      a,(\2) >> 8
        ld      [\1+1],a
        ENDM

;* ld16irr ADDR,VALUE - 8 bytes, 44 cycles
;*   Store 16-bit immediate VALUE to memory at ADDR.

ld16irr: MACRO
        ld      hl,\1
        ld      [hl],(\2) & $ff
        inc     hl
        ld      [hl],(\2) >> 8
        ENDM

        ENDC    ;LOAD1_INC

