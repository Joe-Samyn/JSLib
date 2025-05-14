; TODO: How is Metadata data added into memory:
; - Does this allocate the data on the memory block or a different address
; - If its the same memory block, is it one after the other

; TODO: Does returning metadata + 1 work as expected? 
; - Does it return the address metadata + sizeof(Metadata)?
; - Does it return the address metadata + 1 byte? 


	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 4
	.globl	_allocMemory                    ; -- Begin function allocMemory
	.p2align	2

; (size_t size) stored in x0
_allocMemory:                           ; @allocMemory
; %bb.0:
	sub	sp, sp, #64						; Allocate 64 bytes on the stack for variables 
	stp	x29, x30, [sp, #48]             ; Store FP and LR address onto the stack 
	add	x29, sp, #48   					; Store beginning of stack frame into FP, its only 48 bytes b/c FP + LR = 16 bytes and 64 - 16 = 48
	stur	x0, [x29, #-16]				; Store size function parameter on stack 
	ldur	x8, [x29, #-16]				; Store size into x8 general purpose register
	add	x8, x8, #24						; memorySize = sizeof(Metadata) + size
                                        
	stur	w8, [x29, #-20]
	ldur	w8, [x29, #-20]
	add	w8, w8, #7
	and	w8, w8, #0xfffffff8
	str	w8, [sp, #24]
	ldrsw	x1, [sp, #24]
	mov	x0, #0                          ; =0x0
	mov	w2, #3                          ; =0x3
	mov	w3, #4098                       ; =0x1002
	mov	w4, #-1                         ; =0xffffffff
	mov	x5, #0                          ; =0x0
	bl	_mmap
	str	x0, [sp, #16]
	ldr	x8, [sp, #16]
	adds	x8, x8, #1
	cset	w8, ne
	tbnz	w8, #0, LBB0_4
	b	LBB0_1
LBB0_1:
	bl	___error
	ldr	w8, [x0]
	subs	w8, w8, #22
	cset	w8, ne
	tbnz	w8, #0, LBB0_3
	b	LBB0_2
LBB0_2:
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]
	b	LBB0_5
LBB0_3:
	b	LBB0_4
LBB0_4:
	ldr	x8, [sp, #16]
	str	x8, [sp, #8]
	ldur	x8, [x29, #-16]
	ldr	x9, [sp, #8]
	str	x8, [x9, #8]
	ldr	x8, [sp, #8]
                                        ; kill: def $x9 killed $xzr
	str	xzr, [x8]
	ldr	x8, [sp, #8]
	str	wzr, [x8, #16]
	ldr	x8, [sp, #8]
	add	x8, x8, #24
	stur	x8, [x29, #-8]
	b	LBB0_5
LBB0_5:
	ldur	x0, [x29, #-8]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
                                        ; -- End function
.subsections_via_symbols
