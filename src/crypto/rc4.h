#ifndef _RC4_H
#define _RC4_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rc4_key {
    unsigned char state[256];
    unsigned char x;
    unsigned char y;
} rc4_key;
void prepare_key (unsigned char *key_data_ptr, int key_data_len, rc4_key * key);
void rc4(unsigned char *out, unsigned char *in, int buffer_len, rc4_key *key);

#ifdef __cplusplus
}
#endif

#endif
