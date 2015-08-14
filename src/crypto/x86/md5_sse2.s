BITS 32

;=============================================================================
;	Helper Macros
;=============================================================================

%define a xmm0
%define b xmm1
%define c xmm2
%define d xmm3
%define m xmm4 ; temp reg1
%define n xmm5 ; temp reg2
%define t xmm6 ; temp reg3
%define k xmm7 ; const 0xffffffffffffffff

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

;-----------------------------------------------------------------------------
;	esi, edi, edx, ebx store the 128 bits digest
;	according to a, b, c, d of related C routines
;	while eax and ecx are using as temporary registers.
;	a,  b,  c,  d,  x,  s,  ac, flag
;	%1, %2, %3, %4, %5, %6, %7, %8
;-----------------------------------------------------------------------------

%macro  FF 7

	movdqa	m, %3
	pxor	m, %4
	paddd	%1, %7
	pand	m, %2
	paddd	%1, %5
	pxor	m, %4
	paddd	%1, m
	movdqa	t, %1
	pslld	%1, %6
	psrld	t, 32-%6
	pxor	%1, t
	paddd	%1, %2

%endmacro

%macro  GG 7

	movdqa	m, %2
	movdqa	n, %1
	paddd	n, %7
	pxor	m, %3
	paddd	n, %5
	pand	m, %4
	pxor	m, %3
	movdqa	%1, m
	paddd	%1, n
	movdqa	t, %1
	pslld	%1, %6
	psrld	t, 32-%6
	pxor	%1, t
	paddd	%1, %2

%endmacro

%macro  HH 7

	movdqa	n, %2
	movdqa	m, %1
	paddd	m, %7
	pxor	n, %3
	paddd	m, %5
	pxor	n, %4
	movdqa	%1, n
	paddd	%1, m
	movdqa	t, %1
	pslld	%1, %6
	psrld	t, 32-%6
	pxor	%1, t
	paddd	%1, %2

%endmacro

%macro  II 7

	movdqa	n, %4
	movdqa	m, %1
	pxor	n, k
	paddd	m, %7
	por		n, %2
	paddd	m, %5
	pxor	n, %3
	movdqa	%1, n
	paddd	%1, m
	movdqa	t, %1
	pslld	%1, %6
	psrld	t, 32-%6
	pxor	%1, t
	paddd	%1, %2

%endmacro
	
%define FF(a, b, c, d, x, s, ac) FF a, b, c, d, x, s, ac
%define GG(a, b, c, d, x, s, ac) GG a, b, c, d, x, s, ac
%define HH(a, b, c, d, x, s, ac) HH a, b, c, d, x, s, ac
%define II(a, b, c, d, x, s, ac) II a, b, c, d, x, s, ac

;=============================================================================
; Data
;=============================================================================

SECTION .data align=16

a0: times 4 dd 0x67452301
b0: times 4 dd 0xefcdab89
c0: times 4 dd 0x98badcfe
d0: times 4 dd 0x10325476

ac: times 4 dd 0xd76aa478    
	times 4 dd 0xe8c7b756
	times 4 dd 0x242070db
	times 4 dd 0xc1bdceee
	times 4 dd 0xf57c0faf
	times 4 dd 0x4787c62a
	times 4 dd 0xa8304613
	times 4 dd 0xfd469501
	times 4 dd 0x698098d8
	times 4 dd 0x8b44f7af
	times 4 dd 0xffff5bb1
	times 4 dd 0x895cd7be
	times 4 dd 0x6b901122
	times 4 dd 0xfd987193
	times 4 dd 0xa679438e
	times 4 dd 0x49b40821
                             
	times 4 dd 0xf61e2562
	times 4 dd 0xc040b340
	times 4 dd 0x265e5a51
	times 4 dd 0xe9b6c7aa
	times 4 dd 0xd62f105d
	times 4 dd 0x02441453
	times 4 dd 0xd8a1e681
	times 4 dd 0xe7d3fbc8
	times 4 dd 0x21e1cde6
	times 4 dd 0xc33707d6
	times 4 dd 0xf4d50d87
	times 4 dd 0x455a14ed
	times 4 dd 0xa9e3e905
	times 4 dd 0xfcefa3f8
	times 4 dd 0x676f02d9
	times 4 dd 0x8d2a4c8a
	
	times 4 dd 0xfffa3942
	times 4 dd 0x8771f681
	times 4 dd 0x6d9d6122
	times 4 dd 0xfde5380c
	times 4 dd 0xa4beea44
	times 4 dd 0x4bdecfa9
	times 4 dd 0xf6bb4b60
	times 4 dd 0xbebfbc70
	times 4 dd 0x289b7ec6
	times 4 dd 0xeaa127fa
	times 4 dd 0xd4ef3085
	times 4 dd  0x4881d05
	times 4 dd 0xd9d4d039
	times 4 dd 0xe6db99e5
	times 4 dd 0x1fa27cf8
	times 4 dd 0xc4ac5665
	
	times 4 dd 0xf4292244
	times 4 dd 0x432aff97
	times 4 dd 0xab9423a7
	times 4 dd 0xfc93a039
	times 4 dd 0x655b59c3
	times 4 dd 0x8f0ccc92
	times 4 dd 0xffeff47d
	times 4 dd 0x85845dd1
	times 4 dd 0x6fa87e4f
	times 4 dd 0xfe2ce6e0
	times 4 dd 0xa3014314
	times 4 dd 0x4e0811a1
	times 4 dd 0xf7537e82
	times 4 dd 0xbd3af235
	times 4 dd 0x2ad7d2bb
	times 4 dd 0xeb86d391

;=============================================================================
; Code
;=============================================================================

SECTION .text

global _md5_transform_sse2

;-----------------------------------------------------------------------------
; void md5_transform_sse2(uint32_t state[4], unsigned char block[64])
;
; input data format:
; +-----------------------+-----------------+
; |data...|0x80|0000..0000|length of data(b)|
; +-----------------------+-----------------+
; ^0B                     ^56B              ^64B
;
; remark:
; only support the data less than or equal 56 bytes.
;
; return:
;-----------------------------------------------------------------------------

ALIGN 16

%define stack_size 20
%define x(i)  [ebp + 16*i]
%define ac(i) [ecx + 16*i]
_md5_transform_sse2:
	
	; init the frame and stack
	
	push ebp
	push ebx
	push edx
	push esi
	push edi

	; copy a block of 512 bits to array x
	
	mov ebp, [esp + stack_size + 8]
	mov ecx, ac

	movdqa a, [a0]
	movdqa b, [b0]
	movdqa c, [c0]
	movdqa d, [d0]
	pcmpeqd k, k
	
	; Round 1
	
	FF(a, b, c, d, x( 0), S11, ac( 0)) ; 1 
	FF(d, a, b, c, x( 1), S12, ac( 1)) ; 2 
	FF(c, d, a, b, x( 2), S13, ac( 2)) ; 3 
	FF(b, c, d, a, x( 3), S14, ac( 3)) ; 4 
	FF(a, b, c, d, x( 4), S11, ac( 4)) ; 5 
	FF(d, a, b, c, x( 5), S12, ac( 5)) ; 6 
	FF(c, d, a, b, x( 6), S13, ac( 6)) ; 7 
	FF(b, c, d, a, x( 7), S14, ac( 7)) ; 8 
	FF(a, b, c, d, x( 8), S11, ac( 8)) ; 9 
	FF(d, a, b, c, x( 9), S12, ac( 9)) ; 10
	FF(c, d, a, b, x(10), S13, ac(10)) ; 11
	FF(b, c, d, a, x(11), S14, ac(11)) ; 12
	FF(a, b, c, d, x(12), S11, ac(12)) ; 13
	FF(d, a, b, c, x(13), S12, ac(13)) ; 14
	FF(c, d, a, b, x(14), S13, ac(14)) ; 15
	FF(b, c, d, a, x(15), S14, ac(15)) ; 16

	; Round 2                               
	                                        
	GG(a, b, c, d, x( 1), S21, ac(16)) ; 17
	GG(d, a, b, c, x( 6), S22, ac(17)) ; 18
	GG(c, d, a, b, x(11), S23, ac(18)) ; 19
	GG(b, c, d, a, x( 0), S24, ac(19)) ; 20
	GG(a, b, c, d, x( 5), S21, ac(20)) ; 21
	GG(d, a, b, c, x(10), S22, ac(21)) ; 22
	GG(c, d, a, b, x(15), S23, ac(22)) ; 23
	GG(b, c, d, a, x( 4), S24, ac(23)) ; 24
	GG(a, b, c, d, x( 9), S21, ac(24)) ; 25
	GG(d, a, b, c, x(14), S22, ac(25)) ; 26
	GG(c, d, a, b, x( 3), S23, ac(26)) ; 27
	GG(b, c, d, a, x( 8), S24, ac(27)) ; 28
	GG(a, b, c, d, x(13), S21, ac(28)) ; 29
	GG(d, a, b, c, x( 2), S22, ac(29)) ; 30
	GG(c, d, a, b, x( 7), S23, ac(30)) ; 31
	GG(b, c, d, a, x(12), S24, ac(31)) ; 32
                                       
	; Round 3                               
	                                        
	HH(a, b, c, d, x( 5), S31, ac(32)) ; 33
	HH(d, a, b, c, x( 8), S32, ac(33)) ; 34
	HH(c, d, a, b, x(11), S33, ac(34)) ; 35
	HH(b, c, d, a, x(14), S34, ac(35)) ; 36
	HH(a, b, c, d, x( 1), S31, ac(36)) ; 37
	HH(d, a, b, c, x( 4), S32, ac(37)) ; 38
	HH(c, d, a, b, x( 7), S33, ac(38)) ; 39
	HH(b, c, d, a, x(10), S34, ac(39)) ; 40
	HH(a, b, c, d, x(13), S31, ac(40)) ; 41
	HH(d, a, b, c, x( 0), S32, ac(41)) ; 42
	HH(c, d, a, b, x( 3), S33, ac(42)) ; 43
	HH(b, c, d, a, x( 6), S34, ac(43)) ; 44
	HH(a, b, c, d, x( 9), S31, ac(44)) ; 45
	HH(d, a, b, c, x(12), S32, ac(45)) ; 46
	HH(c, d, a, b, x(15), S33, ac(46)) ; 47
	HH(b, c, d, a, x( 2), S34, ac(47)) ; 48
	                                        
	; Round 4                               
	                                        
	II(a, b, c, d, x( 0), S41, ac(48)) ; 49
	II(d, a, b, c, x( 7), S42, ac(49)) ; 50
	II(c, d, a, b, x(14), S43, ac(50)) ; 51
	II(b, c, d, a, x( 5), S44, ac(51)) ; 52
	II(a, b, c, d, x(12), S41, ac(52)) ; 53
	II(d, a, b, c, x( 3), S42, ac(53)) ; 54
	II(c, d, a, b, x(10), S43, ac(54)) ; 55
	II(b, c, d, a, x( 1), S44, ac(55)) ; 56
	II(a, b, c, d, x( 8), S41, ac(56)) ; 57
	II(d, a, b, c, x(15), S42, ac(57)) ; 58
	II(c, d, a, b, x( 6), S43, ac(58)) ; 59
	II(b, c, d, a, x(13), S44, ac(59)) ; 60
	II(a, b, c, d, x( 4), S41, ac(60)) ; 61
	II(d, a, b, c, x(11), S42, ac(61)) ; 62
	II(c, d, a, b, x( 2), S43, ac(62)) ; 63
	II(b, c, d, a, x( 9), S44, ac(63)) ; 64

	mov eax, [esp + stack_size + 4]
	paddd a, [a0]
	paddd b, [b0]
	paddd c, [c0]
	paddd d, [d0]
	movdqu [eax], a
%if 0
	mov [eax], a
	mov [eax + 4], b
	mov [eax + 8], c
	mov [eax + 12], d

	; fini the frame and stack
%endif	
	pop edi
	pop esi
	pop edx
	pop ebx
	pop ebp
	
	ret

%undef stack_size
%undef x(i)

