/**
 *  Copyright (C) 2005, FastLib
 *  All rights reserved.
 *
 *  @file 		av_crypto.h
 *  @brief		cryptogragh related
 *
 *  @version	0.1
 *  @author		crazyd (crazy_diamond_at_sourceforge.net)
 *	@brief		created
 *  @date		2007/2/26
 */

#ifndef _AV_CRYPTO_H_
#define _AV_CRYPTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "..\utils.h"
typedef struct MD5Context
{
  u32 state[4];			///< state (ABCD)
  u32 count[2];			///< number of bits, modulo 2^64 (lsb first)
  u8 buffer[64];		///< input buffer
} MD5Context;

void MD5Init(MD5Context *);
void MD5Update(MD5Context *, u8 *buf, unsigned int len);
void MD5Final (u8 digest[16], MD5Context *context);
int MD5MessageDigest(u8 *digest, u8 *buf, int len);
void MD5Transform(u32 state[4], unsigned char block[64]);

#ifdef __cplusplus
}
#endif

#endif
