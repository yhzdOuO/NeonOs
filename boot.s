.code16
.section .text
.global _start

_start:	
	movw	$msg, %si
	movb	$0x0E, %ah
	
print:	
	movb 	(%si), %al
	cmpb	$0, %al
	je	end
	int	$0x10
	incw	%si
	jmp	print

end:	
	hlt
	jmp 	end

msg:	
	.ascii "\n\nhello NeonOs!\n"
	
	.space 510-(.-_start), 0
	
	.byte 0x55, 0xaa
