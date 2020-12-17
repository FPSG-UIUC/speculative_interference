.global support_asm_fn

.section .data

	indices: .long 255,255,255,255,255,255,255,255
.align 64
	hit0:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit1:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit2:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit3:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit4:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit5:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit6:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit7:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit8:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit9:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit10:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit11:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit12:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit13:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit14:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit15:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit16:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit17:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit18:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit19:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit20:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit21:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit22:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit23:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit24:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit25:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit26:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit27:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit28:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit29:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit30:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	hit31:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0

	miss0:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss1:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss2:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss3:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss4:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss5:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss6:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss7:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss8:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss9:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss10:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss11:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss12:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss13:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss14:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss15:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss16:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss17:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss18:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss19:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss20:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss21:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss22:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss23:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss24:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss25:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss26:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss27:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss28:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss29:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss30:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
	miss31:	.double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0


.section .text

	#### VSQRTPD + VSQRTPD ####
	support_asm_fn:

	push	%rbp
	mov	%rsp,%rbp

	movlpd indices(%rip), %xmm0		# Initialize indicies for ymm0 VPERMD
	movhpd indices+8(%rip), %xmm0
	vinsertf128 $0x0, %xmm0, %ymm4, %ymm4 
	movlpd indices+16(%rip), %xmm0
	movhpd indices+24(%rip), %xmm0   
	vinsertf128 $0x1, %xmm0, %ymm4, %ymm4 
	vmovapd %ymm4, %ymm0  
  
	vpcmpeqw %xmm0, %xmm0, %xmm0       	# Initialize ymm1-15 registers
	vpsllq $54, %xmm0, %xmm0
	vpsrlq $2, %xmm0, %xmm0
	vinsertf128 $0x1, %xmm0, %ymm0, %ymm0
	vaddpd %ymm0, %ymm0, %ymm1
	vaddpd %ymm1, %ymm1, %ymm2
	vmovapd %ymm2, %ymm3
	vmovapd %ymm2, %ymm4
	vmovapd %ymm2, %ymm5
	vmovapd %ymm2, %ymm6
	vmovapd %ymm2, %ymm7
	vmovapd %ymm2, %ymm8
	vmovapd %ymm2, %ymm9
	vmovapd %ymm2, %ymm10
	vmovapd %ymm2, %ymm11
	vmovapd %ymm2, %ymm12
	vmovapd %ymm2, %ymm13
	vmovapd %ymm2, %ymm14
	vmovapd %ymm2, %ymm15

	leave
	ret

