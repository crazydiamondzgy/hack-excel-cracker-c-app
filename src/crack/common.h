#ifndef _PDFCOMMON_H_
#define _PDFCOMMON_H_

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

//typedef int bool;
//#define false 0
//#define true 1

#define likely(x) (x)//      __builtin_expect((x),1)
#define unlikely(x) (x) //     __builtin_expect((x),0)

/** 
    EncData holds all the information regarding the encryption-setting of a 
    specific pdf. 
    s_handler - Security handler string.
    o_string - Owner-string, 32 bytes, not null-terminated
    u_string - User-string, 32 bytes, not null-terminated
    fileID - file ID in fileIDLen bytes, not null-terminated
*/
struct EncData {
  char *s_handler;
  uint8_t *o_string;
  uint8_t *u_string;
  uint8_t *fileID;
  int encryptMetaData;
  unsigned int fileIDLen;
  unsigned int version_major;
  unsigned int version_minor;
  int length;
  int permissions;
  int revision;
  int version;
};

typedef struct EncData EncData;

typedef enum passwordMethod {
  Wordlist=1,
  Generative
} passwordMethod;

void
freeEncData(EncData *e);

void
printEncData(EncData *e);

#endif /** _PDFCOMMON_H_ */
