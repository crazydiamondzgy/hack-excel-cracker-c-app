;=============================================================================
;	Copyright (C) 2004, crazyd
;	All rights reserved.
;
;	MD5 transform function
;	generate a digest for each block of 64 bytes
;
;	Author:	crazyd (crazydiamondzgy_at_hotmail_dot_com)
;	Date:	2002/12/29
;
;	History:
;		#remove the unused stuff from this file.(2007/02/26)
;		#write in nasm style for linux platform portation.(2005/02/15)
;		#created in inline assembly(2002/12/29)
;=============================================================================

BITS 32

;=============================================================================
;	Helper Macros
;=============================================================================

%define a esi
%define b edi
%define c edx
%define d ebx

%define x(i) [x + 4*i]

%define S11 7
%define S12 12
%define S13 17
%define S14 22
%define S21 5
%define S22 9
%define S23 14
%define S24 20
%define S31 4
%define S32 11
%define S33 16
%define S34 23
%define S41 6
%define S42 10
%define S43 15
%define S44 21

;=============================================================================
;	esi, edi, edx, ebx store the 128 bits digest
;	according to a, b, c, d of related C routines
;	while eax and ecx are using as temporary registers.
;	a,  b,  c,  d,  x,  s,  ac
;	%1, %2, %3, %4, %5, %6, %7
;=============================================================================

%macro  FF 7

	mov eax, %2
	and eax, %3
	mov ecx, %2
	not ecx
	and ecx, %4
	or  ecx, eax
	lea %1, [ecx + %1 + %7]
	add %1, %5
	rol %1, %6
	add %1, %2

%endmacro

%macro  GG 7

	mov eax, %2
	and eax, %4
	mov ecx, %4
	not ecx
	and ecx, %3
	or  ecx, eax
	lea %1, [ecx + %1 + %7]
	add %1, %5
	rol %1, %6
	add %1, %2

%endmacro

%macro  HH 7

	mov ecx, %2
	xor ecx, %3
	xor ecx, %4
	lea %1, [ecx + %1 + %7]
	add %1, %5
	rol %1, %6
	add %1, %2

%endmacro

%macro  II 7

	mov ecx, %4
	not ecx
	or  ecx, %2
	xor ecx, %3
	lea %1, [ecx + %1 + %7]
	add %1, %5
	rol %1, %6
	add %1, %2

%endmacro
	
%define FF(a, b, c, d, x, s, ac) FF a, b, c, d, x, s, ac
%define GG(a, b, c, d, x, s, ac) GG a, b, c, d, x, s, ac
%define HH(a, b, c, d, x, s, ac) HH a, b, c, d, x, s, ac
%define II(a, b, c, d, x, s, ac) II a, b, c, d, x, s, ac

;=============================================================================
; Data
;=============================================================================

SECTION .data

ALIGN 16
x:
	times 16 dd 0x0
	
;=============================================================================
; Code
;=============================================================================

SECTION .text

global _MD5Transform

;
; void 
; MD5Transform(uint32_t state[4], unsigned char block[64])
;

ALIGN 16

_MD5Transform:
	
	; init the frame and stack
	
	push ebp
	mov ebp, esp
	push ebx
	push edx
	push esi
	push edi
	
	; copy a block of 512 bits to array x
	
	mov esi, [ebp + 12]
	mov edi, x
	
	mov ecx, 16
	rep movsd
	
	; state
	
	mov eax, [ebp + 8]
	mov a, [eax]
	mov b, [eax + 4]
	mov c, [eax + 8]
	mov d, [eax + 12]
	
	; Round 1
	
	FF(a, b, c, d, x( 0), S11, 0xd76aa478) ; 1 
	FF(d, a, b, c, x( 1), S12, 0xe8c7b756) ; 2 
	FF(c, d, a, b, x( 2), S13, 0x242070db) ; 3 
	FF(b, c, d, a, x( 3), S14, 0xc1bdceee) ; 4 
	FF(a, b, c, d, x( 4), S11, 0xf57c0faf) ; 5 
	FF(d, a, b, c, x( 5), S12, 0x4787c62a) ; 6 
	FF(c, d, a, b, x( 6), S13, 0xa8304613) ; 7 
	FF(b, c, d, a, x( 7), S14, 0xfd469501) ; 8 
	FF(a, b, c, d, x( 8), S11, 0x698098d8) ; 9 
	FF(d, a, b, c, x( 9), S12, 0x8b44f7af) ; 10
	FF(c, d, a, b, x(10), S13, 0xffff5bb1) ; 11
	FF(b, c, d, a, x(11), S14, 0x895cd7be) ; 12
	FF(a, b, c, d, x(12), S11, 0x6b901122) ; 13
	FF(d, a, b, c, x(13), S12, 0xfd987193) ; 14
	FF(c, d, a, b, x(14), S13, 0xa679438e) ; 15
	FF(b, c, d, a, x(15), S14, 0x49b40821) ; 16
	                                        
	; Round 2                               
	                                        
	GG(a, b, c, d, x( 1), S21, 0xf61e2562) ; 17
	GG(d, a, b, c, x( 6), S22, 0xc040b340) ; 18
	GG(c, d, a, b, x(11), S23, 0x265e5a51) ; 19
	GG(b, c, d, a, x( 0), S24, 0xe9b6c7aa) ; 20
	GG(a, b, c, d, x( 5), S21, 0xd62f105d) ; 21
	GG(d, a, b, c, x(10), S22,  0x2441453) ; 22
	GG(c, d, a, b, x(15), S23, 0xd8a1e681) ; 23
	GG(b, c, d, a, x( 4), S24, 0xe7d3fbc8) ; 24
	GG(a, b, c, d, x( 9), S21, 0x21e1cde6) ; 25
	GG(d, a, b, c, x(14), S22, 0xc33707d6) ; 26
	GG(c, d, a, b, x( 3), S23, 0xf4d50d87) ; 27
	GG(b, c, d, a, x( 8), S24, 0x455a14ed) ; 28
	GG(a, b, c, d, x(13), S21, 0xa9e3e905) ; 29
	GG(d, a, b, c, x( 2), S22, 0xfcefa3f8) ; 30
	GG(c, d, a, b, x( 7), S23, 0x676f02d9) ; 31
	GG(b, c, d, a, x(12), S24, 0x8d2a4c8a) ; 32
	                                        
	; Round 3                               
	                                        
	HH(a, b, c, d, x( 5), S31, 0xfffa3942) ; 33
	HH(d, a, b, c, x( 8), S32, 0x8771f681) ; 34
	HH(c, d, a, b, x(11), S33, 0x6d9d6122) ; 35
	HH(b, c, d, a, x(14), S34, 0xfde5380c) ; 36
	HH(a, b, c, d, x( 1), S31, 0xa4beea44) ; 37
	HH(d, a, b, c, x( 4), S32, 0x4bdecfa9) ; 38
	HH(c, d, a, b, x( 7), S33, 0xf6bb4b60) ; 39
	HH(b, c, d, a, x(10), S34, 0xbebfbc70) ; 40
	HH(a, b, c, d, x(13), S31, 0x289b7ec6) ; 41
	HH(d, a, b, c, x( 0), S32, 0xeaa127fa) ; 42
	HH(c, d, a, b, x( 3), S33, 0xd4ef3085) ; 43
	HH(b, c, d, a, x( 6), S34,  0x4881d05) ; 44
	HH(a, b, c, d, x( 9), S31, 0xd9d4d039) ; 45
	HH(d, a, b, c, x(12), S32, 0xe6db99e5) ; 46
	HH(c, d, a, b, x(15), S33, 0x1fa27cf8) ; 47
	HH(b, c, d, a, x( 2), S34, 0xc4ac5665) ; 48
	                                        
	; Round 4                               
	                                        
	II(a, b, c, d, x( 0), S41, 0xf4292244) ; 49
	II(d, a, b, c, x( 7), S42, 0x432aff97) ; 50
	II(c, d, a, b, x(14), S43, 0xab9423a7) ; 51
	II(b, c, d, a, x( 5), S44, 0xfc93a039) ; 52
	II(a, b, c, d, x(12), S41, 0x655b59c3) ; 53
	II(d, a, b, c, x( 3), S42, 0x8f0ccc92) ; 54
	II(c, d, a, b, x(10), S43, 0xffeff47d) ; 55
	II(b, c, d, a, x( 1), S44, 0x85845dd1) ; 56
	II(a, b, c, d, x( 8), S41, 0x6fa87e4f) ; 57
	II(d, a, b, c, x(15), S42, 0xfe2ce6e0) ; 58
	II(c, d, a, b, x( 6), S43, 0xa3014314) ; 59
	II(b, c, d, a, x(13), S44, 0x4e0811a1) ; 60
	II(a, b, c, d, x( 4), S41, 0xf7537e82) ; 61
	II(d, a, b, c, x(11), S42, 0xbd3af235) ; 62
	II(c, d, a, b, x( 2), S43, 0x2ad7d2bb) ; 63
	II(b, c, d, a, x( 9), S44, 0xeb86d391) ; 64

	mov eax, [ebp + 8]
	add [eax], a
	add [eax + 4], b
	add [eax + 8], c
	add [eax + 12], d
	
	; fini the frame and stack
	
	pop edi
	pop esi
	pop edx
	pop ebx
	pop ebp
	
	ret
