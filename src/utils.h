#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define MAX_PASSWORD 64

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#pragma pack(16)

#define CRACK_TYPE_DIRECT		(1<<0)
#define CRACK_TYPE_DICTIONARY	(1<<1)
#define CRACK_TYPE_BRUTEFORCE	(1<<2)

typedef enum _crack_id_t
{
	CRACK_ID_WORD, 
	CRACK_ID_OUTLOOK, 
	CRACK_ID_EXCEL, 
	CRACK_ID_ACCESS, 
	CRACK_ID_PDF, 
	CRACK_ID_RAR, 
	CRACK_ID_ZIP
} crack_id_t;

typedef struct _CrackContext
{
	char pw[MAX_PASSWORD];
	int  pw_len;
	char pw_next[256];
	int  pw_maxlen;
	char charset[256];
	unsigned int pw_num;
	struct _Cracker *c;
	void *priv_data;
	FILE *dict_fp;
	FILE *input_fp;
	FILE *output_fp;
	char *dict_filename;
	char *input_filename;
	char *output_filename;
} CrackContext;

typedef struct _Cracker
{
	const char *name;
	const char *exts;
    enum crack_type_t type;
    enum crack_id_t   id;
    int  priv_data_size;
	int  (* crack_open)  (CrackContext *ctx);
	int  (* crack)       (CrackContext *ctx, char *string, unsigned int len);
	int  (* crack_close) (CrackContext *ctx);
	struct _Cracker *next;
} Cracker;

#pragma pack()

extern int matrix_crack_register(Cracker *c);
extern int matrix_crack_open (CrackContext *ctx, char *filename);
extern int matrix_crack      (CrackContext *ctx);
extern int matrix_crack_close(CrackContext *ctx);

extern Cracker *_crack_list_header;
extern Cracker word_cracker;
extern Cracker excel_cracker;
extern Cracker access_cracker;
extern Cracker outlook_cracker;
extern Cracker zip_cracker;
extern Cracker rar_cracker;
extern Cracker pdf_cracker;

#ifdef __cplusplus
}
#endif

#endif
