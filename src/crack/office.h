#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


#include "..\utils.h"
#include "..\crypto\rc4.h"
#include "..\crypto\md5.h"


typedef struct _OfficeContext
{
	u8  docid[16];
	u8  salt[64];
	u8  hashedsalt[16];
	u8  digest[16];
	u8  the_key[16];
	u32 table1_pos;
	u32 table1_len;
	u32 document_pos;
	u32 document_len;
	u32 dop_pos;
	u32 dop_len;
} OfficeContext;

void MD5Digest(u8 *digest, MD5Context *mdContext);
void makekey(OfficeContext *ctx, u32 block, rc4_key *key, u8 *digest);
int verifypwd(OfficeContext *ctx, u8 pwarray[64], MD5Context *valContext);
void expandpw(u8 password[16], u8 pwarray[64]);
int office_crack(CrackContext *ctx, char *string, unsigned int len);
int office_close(CrackContext *ctx);

#ifdef __cplusplus
}
#endif
