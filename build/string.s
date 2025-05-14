	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 4
	.globl	_strLen                         ; -- Begin function strLen
	.p2align	2
_strLen:                                ; @strLen
; %bb.0:
	sub	sp, sp, #48
	str	x0, [sp, #32]
	str	w1, [sp, #28]
	ldr	x8, [sp, #32]
	subs	x8, x8, #0
	cset	w8, eq
	tbnz	w8, #0, LBB0_2
	b	LBB0_1
LBB0_1:
	ldr	w8, [sp, #28]
	subs	w8, w8, #0
	cset	w8, gt
	tbnz	w8, #0, LBB0_3
	b	LBB0_2
LBB0_2:
	mov	w8, #3                          ; =0x3
	str	w8, [sp, #44]
	b	LBB0_11
LBB0_3:
	str	wzr, [sp, #24]
	ldr	x8, [sp, #32]
	str	x8, [sp, #16]
	b	LBB0_4
LBB0_4:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #16]
	ldrsb	w8, [x8]
	subs	w8, w8, #0
	cset	w8, eq
	mov	w9, #0                          ; =0x0
	str	w9, [sp, #12]                   ; 4-byte Folded Spill
	tbnz	w8, #0, LBB0_6
	b	LBB0_5
LBB0_5:                                 ;   in Loop: Header=BB0_4 Depth=1
	ldr	w8, [sp, #24]
	ldr	w9, [sp, #28]
	subs	w8, w8, w9
	cset	w8, lt
	str	w8, [sp, #12]                   ; 4-byte Folded Spill
	b	LBB0_6
LBB0_6:                                 ;   in Loop: Header=BB0_4 Depth=1
	ldr	w8, [sp, #12]                   ; 4-byte Folded Reload
	tbz	w8, #0, LBB0_8
	b	LBB0_7
LBB0_7:                                 ;   in Loop: Header=BB0_4 Depth=1
	ldr	w8, [sp, #24]
	add	w8, w8, #1
	str	w8, [sp, #24]
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	b	LBB0_4
LBB0_8:
	ldr	w8, [sp, #24]
	ldr	w9, [sp, #28]
	subs	w8, w8, w9
	cset	w8, ne
	tbnz	w8, #0, LBB0_10
	b	LBB0_9
LBB0_9:
	mov	w8, #5                          ; =0x5
	str	w8, [sp, #44]
	b	LBB0_11
LBB0_10:
	ldr	w8, [sp, #24]
	str	w8, [sp, #44]
	b	LBB0_11
LBB0_11:
	ldr	w0, [sp, #44]
	add	sp, sp, #48
	ret
                                        ; -- End function
	.globl	_strCpy                         ; -- Begin function strCpy
	.p2align	2
_strCpy:                                ; @strCpy
; %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	stur	x0, [x29, #-16]
	stur	w1, [x29, #-20]
	str	x2, [sp, #32]
	ldur	x8, [x29, #-16]
	subs	x8, x8, #0
	cset	w8, eq
	tbnz	w8, #0, LBB1_2
	b	LBB1_1
LBB1_1:
	ldr	x8, [sp, #32]
	subs	x8, x8, #0
	cset	w8, ne
	tbnz	w8, #0, LBB1_3
	b	LBB1_2
LBB1_2:
	mov	w8, #2                          ; =0x2
	stur	w8, [x29, #-4]
	b	LBB1_13
LBB1_3:
	ldur	x8, [x29, #-16]
	str	x8, [sp, #24]
	ldr	x8, [sp, #32]
	str	x8, [sp, #16]
	str	wzr, [sp, #12]
	b	LBB1_4
LBB1_4:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #12]
	ldur	w9, [x29, #-20]
	subs	w8, w8, w9
	cset	w8, hs
	mov	w9, #0                          ; =0x0
	str	w9, [sp, #8]                    ; 4-byte Folded Spill
	tbnz	w8, #0, LBB1_6
	b	LBB1_5
LBB1_5:                                 ;   in Loop: Header=BB1_4 Depth=1
	ldr	x8, [sp, #16]
	ldrsb	w8, [x8]
	subs	w8, w8, #0
	cset	w8, ne
	str	w8, [sp, #8]                    ; 4-byte Folded Spill
	b	LBB1_6
LBB1_6:                                 ;   in Loop: Header=BB1_4 Depth=1
	ldr	w8, [sp, #8]                    ; 4-byte Folded Reload
	tbz	w8, #0, LBB1_8
	b	LBB1_7
LBB1_7:                                 ;   in Loop: Header=BB1_4 Depth=1
	ldr	x8, [sp, #16]
	ldrb	w8, [x8]
	ldr	x9, [sp, #24]
	strb	w8, [x9]
	ldr	x8, [sp, #24]
	add	x8, x8, #1
	str	x8, [sp, #24]
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	ldr	w8, [sp, #12]
	add	w8, w8, #1
	str	w8, [sp, #12]
	b	LBB1_4
LBB1_8:
	ldr	w8, [sp, #12]
	ldur	w9, [x29, #-20]
	subs	w8, w8, w9
	cset	w8, hs
	tbnz	w8, #0, LBB1_10
	b	LBB1_9
LBB1_9:
	ldr	x0, [sp, #24]
	ldur	w1, [x29, #-20]
	ldr	w2, [sp, #12]
	bl	_fillWithTerm
	stur	wzr, [x29, #-4]
	b	LBB1_13
LBB1_10:
	ldr	x8, [sp, #24]
	ldursb	w8, [x8, #-1]
	subs	w8, w8, #0
	cset	w8, eq
	tbnz	w8, #0, LBB1_12
	b	LBB1_11
LBB1_11:
	mov	w8, #5                          ; =0x5
	stur	w8, [x29, #-4]
	b	LBB1_13
LBB1_12:
	stur	wzr, [x29, #-4]
	b	LBB1_13
LBB1_13:
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	add	sp, sp, #80
	ret
                                        ; -- End function
	.p2align	2                               ; -- Begin function fillWithTerm
_fillWithTerm:                          ; @fillWithTerm
; %bb.0:
	sub	sp, sp, #32
	str	x0, [sp, #24]
	str	w1, [sp, #20]
	str	w2, [sp, #16]
	ldr	x8, [sp, #24]
	str	x8, [sp, #8]
	ldr	w8, [sp, #16]
	str	w8, [sp, #4]
	b	LBB2_1
LBB2_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #4]
	ldr	w9, [sp, #20]
	subs	w8, w8, w9
	cset	w8, hs
	tbnz	w8, #0, LBB2_4
	b	LBB2_2
LBB2_2:                                 ;   in Loop: Header=BB2_1 Depth=1
	ldr	x8, [sp, #8]
	strb	wzr, [x8]
	ldr	x8, [sp, #8]
	add	x8, x8, #1
	str	x8, [sp, #8]
	b	LBB2_3
LBB2_3:                                 ;   in Loop: Header=BB2_1 Depth=1
	ldr	w8, [sp, #4]
	add	w8, w8, #1
	str	w8, [sp, #4]
	b	LBB2_1
LBB2_4:
	add	sp, sp, #32
	ret
                                        ; -- End function
	.globl	_str_ncpy                       ; -- Begin function str_ncpy
	.p2align	2
_str_ncpy:                              ; @str_ncpy
; %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	stur	x0, [x29, #-16]
	stur	w1, [x29, #-20]
	str	x2, [sp, #32]
	str	w3, [sp, #28]
	ldur	w8, [x29, #-20]
	ldr	w9, [sp, #28]
	subs	w8, w8, w9
	cset	w8, hs
	tbnz	w8, #0, LBB3_2
	b	LBB3_1
LBB3_1:
	mov	w8, #3                          ; =0x3
	stur	w8, [x29, #-4]
	b	LBB3_17
LBB3_2:
	ldur	x8, [x29, #-16]
	subs	x8, x8, #0
	cset	w8, eq
	tbnz	w8, #0, LBB3_4
	b	LBB3_3
LBB3_3:
	ldr	x8, [sp, #32]
	subs	x8, x8, #0
	cset	w8, ne
	tbnz	w8, #0, LBB3_5
	b	LBB3_4
LBB3_4:
	mov	w8, #2                          ; =0x2
	stur	w8, [x29, #-4]
	b	LBB3_17
LBB3_5:
	str	wzr, [sp, #24]
	ldur	x8, [x29, #-16]
	str	x8, [sp, #16]
	ldr	x8, [sp, #32]
	str	x8, [sp, #8]
	b	LBB3_6
LBB3_6:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #8]
	ldrsb	w8, [x8]
	subs	w8, w8, #0
	cset	w8, eq
	mov	w9, #0                          ; =0x0
	str	w9, [sp, #4]                    ; 4-byte Folded Spill
	tbnz	w8, #0, LBB3_8
	b	LBB3_7
LBB3_7:                                 ;   in Loop: Header=BB3_6 Depth=1
	ldr	w8, [sp, #24]
	ldr	w9, [sp, #28]
	subs	w8, w8, w9
	cset	w8, lo
	str	w8, [sp, #4]                    ; 4-byte Folded Spill
	b	LBB3_8
LBB3_8:                                 ;   in Loop: Header=BB3_6 Depth=1
	ldr	w8, [sp, #4]                    ; 4-byte Folded Reload
	tbz	w8, #0, LBB3_10
	b	LBB3_9
LBB3_9:                                 ;   in Loop: Header=BB3_6 Depth=1
	ldr	x8, [sp, #8]
	ldrb	w8, [x8]
	ldr	x9, [sp, #16]
	strb	w8, [x9]
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	ldr	x8, [sp, #8]
	add	x8, x8, #1
	str	x8, [sp, #8]
	ldr	w8, [sp, #24]
	add	w8, w8, #1
	str	w8, [sp, #24]
	b	LBB3_6
LBB3_10:
	ldr	w8, [sp, #24]
	ldr	w9, [sp, #28]
	subs	w8, w8, w9
	cset	w8, lo
	tbnz	w8, #0, LBB3_12
	b	LBB3_11
LBB3_11:
	ldr	w8, [sp, #24]
	ldur	w9, [x29, #-20]
	subs	w8, w8, w9
	cset	w8, hs
	tbnz	w8, #0, LBB3_13
	b	LBB3_12
LBB3_12:
	ldr	x0, [sp, #16]
	ldur	w1, [x29, #-20]
	ldr	w2, [sp, #24]
	bl	_fillWithTerm
	b	LBB3_16
LBB3_13:
	ldr	x8, [sp, #16]
	ldursb	w8, [x8, #-1]
	subs	w8, w8, #0
	cset	w8, eq
	tbnz	w8, #0, LBB3_15
	b	LBB3_14
LBB3_14:
	mov	w8, #5                          ; =0x5
	stur	w8, [x29, #-4]
	b	LBB3_17
LBB3_15:
	b	LBB3_16
LBB3_16:
	stur	wzr, [x29, #-4]
	b	LBB3_17
LBB3_17:
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	add	sp, sp, #80
	ret
                                        ; -- End function
.subsections_via_symbols
