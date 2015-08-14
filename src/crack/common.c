#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void
freeEncData(EncData *e) {
  if(!e)
    return;
  if(e->o_string)
    free(e->o_string);
  if(e->u_string)
    free(e->u_string);
  if(e->fileID)
    free(e->fileID);
  if(e->s_handler)
    free(e->s_handler);
}

void
printEncData(EncData *e) {
  unsigned int i;
  uint8_t ch;

  printf("PDF version %d.%d\n", e->version_major, e->version_minor);
  if(e->s_handler)
    printf("Security Handler: %s\n",e->s_handler);
  printf("V: %d\nR: %d\nP: %d\nLength: %d\nEncrypted Metadata: %s\n", 
	 e->version, e->revision, e->permissions, e->length, 
	 e->encryptMetaData?"True":"False");
  printf("FileID: ");
  for(i=0; i < e->fileIDLen; i++) {
    ch = e->fileID[i];
    if(ch < 16)
      printf("0%x", ch);
    else
      printf("%x", ch);
  }

  /** Assume u_string and o_string is of length 32. Not safe, but the code
      as a whole needs a rewrite anyway
  */
  if(e->u_string) {
    printf("\nU: ");
    for(i=0; i<32; i++) {
      ch = e->u_string[i];
      if(ch < 16)
	printf("0%x", ch);
      else
	printf("%x", ch);
    }
  }
  if(e->o_string) {
    printf("\nO: ");
    for(i=0; i<32; i++) {
      ch = e->o_string[i];
      if(ch < 16)
	printf("0%x", ch);
      else
	printf("%x", ch);
    }
  }
  printf("\n");
}
