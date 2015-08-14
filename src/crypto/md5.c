#include <memory.h>
#include <stdio.h>

#include "md5.h"

static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
void MD5Init (MD5Context *context)                                       /* context */
{
  context->count[0] = context->count[1] = 0;
  /* Load magic initialization constants.
*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

void MD5Update (MD5Context *context, unsigned char *input, unsigned int inputLen)
{
	unsigned int i, index, partLen;
	
	// Compute number of bytes mod 64
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);
	
	// Update number of bits
	if ((context->count[0] += ((u32)inputLen << 3)) < ((u32)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((u32)inputLen >> 29);
	
	partLen = 64 - index;
	
	//Transform as many times as possible.
	if (inputLen >= partLen) {
		memcpy((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
		MD5Transform (context->state, context->buffer);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
		{
			MD5Transform (context->state, &input[i]);
		}
		
		index = 0;
	}
	else
		i = 0;
	
	// Buffer remaining input
	memcpy((u8 *)&context->buffer[index], (u8 *)&input[i], inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void MD5Final (u8 digest[16], MD5Context *context)
{
	unsigned char bits[8];
	unsigned int index, padLen;
	
	// Save number of bits
	memcpy(bits, context->count, 8);
	
	// Pad out to 56 mod 64.
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update (context, PADDING, padLen);
	
	// Append length (before padding
	MD5Update (context, bits, 8);
	
	//Store state in digest
	memcpy(digest, context->state, 16);
}

int 
MD5MessageDigest(u8 *digest, u8 *buf, int len)
{
	MD5Context context;
	
	MD5Init (&context);
	MD5Update (&context, buf, len);
	MD5Final (digest, &context);

	return 0;
}
