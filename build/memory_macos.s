
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
                                        
	stur	w8, [x29, #-20]				; store memorySize on the stack 
	ldur	w8, [x29, #-20]				; Load memorySize from the stack 
	; align(x) macro expanded
	add	w8, w8, #7						; (memorySize + ALIGNMENT - 1) = (memorySize + (7 - 1))
	and	w8, w8, #0xfffffff8				; 0xfffffff8 is ~7, (memorySize + 7) & ~7
	str	w8, [sp, #24]					; alignedSize = w8
	ldr	x1, [sp, #24]					; Load alignedSize from the stack into x1 8 byte register
	mov	x0, #0                          ; addr = NULL
	mov	w2, #3                          ; prot = PROT_READ | PROT_WRITE
	mov	w3, #4098                       ; flags = MAP_PRIVATE | MAP_ANON
	mov	w4, #-1                         ; fd = -1
	mov	x5, #0                          ; offset = 0
	bl	_mmap							; call mmap function 
	str	x0, [sp, #16]					; memory = mmap(...)
	ldr	x8, [sp, #16]					; load memory from stack 
	adds	x8, x8, #1					; memory + 1
	cset	w8, ne						; memory + 1 == 0?
	tbnz	w8, #0, LBB0_4				; jump to LBB0_4 if memory != 0
	b	LBB0_1							; jump to LBB0_1 if memory == 0
LBB0_1:
	bl	___error						; Get the error code
	ldr	w8, [x0]						; store error code in w8
	subs	w8, w8, #22					; error code - 22
	cset	w8, ne						; check if result == 0
	tbnz	w8, #0, LBB0_3				; error code != EINVAL, jump to LBB0_3
	b	LBB0_2							; else jump to LBB0_2
LBB0_2:
                                        ; kill: def $x8 killed $xzr
	stur	xzr, [x29, #-8]				; Store 0 into x29 - 8
	b	LBB0_5
LBB0_3:
	b	LBB0_4
LBB0_4:
	; Initialize metadata struct
	ldr	x8, [sp, #16]					; load address returned from mmap into x8
	str	x8, [sp, #8]					; metadata = (Metadata*)memory)
	ldur	x8, [x29, #-16]				; load size from stack into register
	ldr	x9, [sp, #8]					; load metadata pointer
	str	x8, [x9, #8]					; (*metadata).size = size
	ldr	x8, [sp, #8]					; load metadata pointer
                                        ; kill: def $x9 killed $xzr
	str	xzr, [x8]						; (*metadata).next = 0
	ldr	x8, [sp, #8]					; load metadata pointer
	str	wzr, [x8, #16]					; (*metadata).free = 0
	ldr	x8, [sp, #8]					; load metadata pointer
	add	x8, x8, #24						; return metadata pointer + 24 bytes 
	stur	x8, [x29, #-8]				; store return value on stack 
	b	LBB0_5
LBB0_5:
	ldur	x0, [x29, #-8]				; load return value
	ldp	x29, x30, [sp, #48]             ; stack tear down
	add	sp, sp, #64						; stack tear down
	ret									; return (either 0 or metadata addr + 24 bytes)
                                        ; -- End function
.subsections_via_symbols
