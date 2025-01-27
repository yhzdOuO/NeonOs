.code16
.section .text
.global _start

_start:
	mov		$0x00, %ax
	mov		%ax, %ss
	mov		%ax, %ds
	mov		$0x7c00, %sp

	call	print
	.string	"\nload start!\n\r"


load_disk:
	mov		$1024, %cx
	mov		$0x2000, %di
	mov		$1, %bx
	mov		$0x80, %dl

load_loop:
	test	$0xf, %cx
	jnz		load
	push	%ax
	push	%si
	call 	print
	.string "."
	pop		%si
	pop		%ax

load:
	mov		$0, %ax
	push	%ax
	push	%ax			# LBA high
	push	%ax
	push	%bx			# LBA low
	add 	$1, %bx

	push	%di			
	push	%ax			# segment:offset
	add		$0x20, %di	

	mov		$1, %ax		
	push	%ax			# length

	mov		$0x0010, %ax
	push	%ax			# unused & size
	
	mov		%ss, %ax
	mov		%ax, %ds
	mov		%sp, %si

	mov		$0x4200, %ax

	int		$0x13
	jc		error

	add		$0x10, %sp
	loop	load_loop

	call 	print
	.string	"\r\nload ok!\n\r"
	call	print
	.string	"\r\nhello NeonOS!\n\r"
	jmp		end


error:
	call	print
	.string	"\r\nload error!\n\r"
	jmp 	end

print:
	pop		%si

printloop:
	movb	(%si), %al
	add		$1, %si
	cmp		$0, %al
	je		pret
	mov		$0x0e, %ah
	int		$0x10
	jc		end
	jmp		printloop

pret:
	push	%si
	ret


end:
	hlt
	jmp 	end
	
	.space 510-(.-_start), 0
	
	.byte 0x55, 0xaa
