#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "office.h"

void MD5Digest(u8 *digest, MD5Context *mdContext)
{
    unsigned int i, ii;
    for (i = 0, ii = 0; i < 4; i++, ii += 4)
    {
        digest[ii] = (unsigned char)(mdContext->state[i] &0xFF);
        digest[ii + 1] = (unsigned char)((mdContext->state[i] >> 8)&0xFF);
        digest[ii + 2] = (unsigned char)((mdContext->state[i] >> 16)&0xFF);
        digest[ii + 3] = (unsigned char)((mdContext->state[i] >> 24)&0xFF);
    }
}

static void makekey(OfficeContext *ctx, u32 block, rc4_key *key, u8 *digest)
{
    MD5Context mdContext;
    u8 pwarray[64];
	
    memset(pwarray, 0, 64);
    //pwarray数组清零
    /* 40 bit of hashed password, set by verifypwd() */
    memcpy(pwarray, digest, 5);
	memcpy(ctx->the_key, digest, 5);
    //拷过去40位（5个字节）
    /* put block number in byte 6...9 */
    pwarray[5] = (u8)(block &0xFF);
    pwarray[6] = (u8)((block >> 8) &0xFF);
    pwarray[7] = (u8)((block >> 16) &0xFF);
    pwarray[8] = (u8)((block >> 24) &0xFF);
	
    pwarray[9] = 0x80;
    pwarray[56] = 0x48;
    //准备数据
    MD5Init(&mdContext);
    MD5Update(&mdContext, pwarray, 64);
    MD5Digest(digest, &mdContext);
    //对pwarrey计算一轮标准的MD5 hash
    prepare_key(digest, 16, key);
    //结果去解密
}

int verifypwd(OfficeContext *ctx, u8 pwarray[64], MD5Context *valContext)
{
    MD5Context mdContext1, mdContext2;
    rc4_key key;
    int offset, keyoffset;
    unsigned int tocopy;
	u8  salt[64];
	u8  hashedsalt[16];

    MD5Init(&mdContext1);
    MD5Update(&mdContext1, pwarray, 64);
    MD5Digest(ctx->digest, &mdContext1);
    //对输入的口令计算一轮hash
    offset = 0;
    keyoffset = 0;
    tocopy = 5;
    //加上while循环一共5轮非标准的MD5 hash
    MD5Init(valContext);
	
    while (offset != 16)
    {
        if ((64-offset) < 5)
        {
            tocopy = 64-offset;
        }
		
        memcpy(pwarray + offset, ctx->digest + keyoffset, tocopy);
        offset += tocopy;
        if (offset == 64)
        {
            MD5Update(valContext, pwarray, 64);
            keyoffset = tocopy;
            tocopy = 5-tocopy;
            offset = 0;
            continue;
        }
		
        keyoffset = 0;
        tocopy = 5;
        memcpy(pwarray + offset, ctx->docid, 16);
        offset += 16;
    }
    //5轮非标准的MD5 hash计算结束
    /* Fix (zero) all but first 16 bytes */
    //除pwarray数组除前16个字节之外全部清零
    pwarray[16] = 0x80;
    memset(pwarray + 17, 0, 47);
    pwarray[56] = 0x80;
    pwarray[57] = 0x0A;
    //再算一轮MD5 hash
    MD5Update(valContext, pwarray, 64);
    MD5Digest(ctx->digest, valContext);
	
    /* Generate 40-bit RC4 key from 128-bit hashed password */
    //makekey函数将取hash结果的前5个字节（40位）扩展并计算hash后生成RC4初始化向量
    makekey(ctx, 0, &key, ctx->digest);
	// c3 9b c3 dc 45
	
    rc4(salt, ctx->salt, 16, &key);
    rc4(hashedsalt, ctx->hashedsalt, 16, &key);
    //salt和hashedsalt在存入1Table stream之前都已经被RC4加密了
    //所以这里把他们全部解密
    salt[16] = 0x80;
    memset(salt + 17, 0, 47);
    salt[56] = 0x80;
    //salt扩展并计算hash后结果与hashedsalt比较，如果相同则口令正确，如果不同，则口令错误
    MD5Init(&mdContext2);
    MD5Update(&mdContext2, salt, 64);
    MD5Digest(ctx->digest, &mdContext2);
	
    return (memcmp(ctx->digest, hashedsalt, 16));
}

//expandpw函数把用户输入的口令Unicode扩展为64个字节的pwarray数组
static void expandpw(u8 password[16], u8 pwarray[64])
{
    /* expandpw expects null terminated 16bit unicode input */
    int i;
	
    for (i = 0; i < 64; i++)
    {
        pwarray[i] = 0;
    }
    //pwarray数组清零
    i = 0;
    while (password[i])
    {
        pwarray[2 *i] = (password[i] &0xff);
        pwarray[(2 *i) + 1] = ((password[i] >> 8) &0xff);
        i++;
    }
	
    pwarray[2 *i] = 0x80;
    pwarray[56] = (i << 4);
}

int 
office_crack(CrackContext *ctx, char *string, unsigned int len)
{
	int i = 0;
	MD5Context MD5Ctx;
	u8 pwarray[64];
	OfficeContext *wc = (OfficeContext *)ctx->priv_data;

	//
	// 扩展用户输入的口令
	//
	
    expandpw (string, pwarray);
	
	//
	// 校验口令是否正确
	//
	
	if (verifypwd (wc, pwarray, &MD5Ctx) == 0)
	{
		strncpy(ctx->pw, string, sizeof(string));
		return 1;
	}
	
	return 0;
}

int 
office_close(CrackContext *ctx)
{
	return 0;
}
