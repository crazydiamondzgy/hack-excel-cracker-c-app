#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <wchar.h>
#include "office.h"

static int 
excel_open(CrackContext *ctx)
{
	u8 buf[512];
    int i, j, section_size;
	u32 x;
	OfficeContext *wc = (OfficeContext *)ctx->priv_data;
	
	ctx->input_fp = fopen(ctx->input_filename, "rb");
	if (NULL == ctx->input_fp)
	{
		printf("failed to open input wordfile!\n");
		return -1;
	}
	
	fread(buf, 1, 512, ctx->input_fp);

	//
	// 获得每个section的尺寸
	//
	
	x = *(u8 *)(buf+30);
	section_size = (u32)pow(2, x);
	
	//
	// 获得directory的地址
	//
	
	x = *(u8 *)(buf+48);
	x = x*section_size + 512;
	
	//
	// 读取directory的全部数据
	//
	
	rewind(ctx->input_fp);
	fseek(ctx->input_fp, x, SEEK_SET);
	fread(buf, 1, 512, ctx->input_fp);
	
	//
	// 获得Workbook的地址
	//
	
	for (i=0; i<section_size; i+=128)
	{
		if (wcscmp((unsigned short *)(buf+i), L"Workbook") == 0)
		{
			wc->table1_pos = *(u32 *)(buf+i+116);
			wc->table1_pos = wc->table1_pos*section_size + 512;
			wc->table1_len = *(u32 *)(buf+i+120);
			continue;
		}
	}
	
	if (0 == wc->table1_pos)
	{
		return -1;
	}
	
	//
	// 读取1Table的内容
	//
	
	rewind(ctx->input_fp);
	fseek(ctx->input_fp, wc->table1_pos, SEEK_SET);
	fread(buf, 1, 512, ctx->input_fp);
	
	for (j=0; j<512;)
	{
		if (*(unsigned short *)(buf+j) == 0x002F)
		{
			j += 10;
			break;
		}
		else
		{
			j = j + *(unsigned short *)(buf+j+2) + 4;
		}
	}
	
	if (j >= 512)
	{
		exit(-1);
	}
	
    for (i=0; i<16; i++)
		wc->docid[i] = buf[j+i];
	
    for (i=0; i<16; i++)
		wc->salt[i] = buf[j+16+i];
	
    for (i=0; i<16; i++)
		wc->hashedsalt[i] = buf[j+32+i];
	
	fclose(ctx->input_fp);
	return 0;
}
/*
int remove_excel()
{
	//	unsigned short key;
	//	unsigned char keys[16];
	int j, file_size = 0;
	u8 *file_buf = NULL, *p = NULL;
	DWORD t = GetTickCount();
	FILE *fp;
	
	//	key = GetKey(true_password);
	//	GetKeys(keys, true_password, key);
	
	//
	// 取消写密码
	//
	
	
	init_excel();
	
	//
	// 读excel文件
	//
	
	fp = fopen(filepath, "rb");
	if (NULL == fp)
	{
		return -1;
	}
	
	//
	// 获得excel文件尺寸
	//
	
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	
	file_buf = (u8 *)malloc(file_size);
	if (file_buf == NULL)
	{
		return -1;
	}
	
	fseek(fp, 0, SEEK_SET);
	fread(file_buf, 1, file_size, fp);
	fclose(fp);
	
	//
	// 寻找Write Protect密码字段
	//
	
	p = file_buf + table1_pos;
	for (j=0; j<512;)
	{
		if (*(p+j) == 0x5B)
		{
			break;
		}
		else
		{
			j = j + *(unsigned short *)(p+j+2) + 4;
		}
	}
	
	if (j < 512)
	{
		*(p+j) = 0x5c;
	}
	//
	// 保存破解后的文件
	//
	
	fp = fopen("foo.xls", "wb");
	if (fp == NULL)
	{
		return -1;
	}
	
	fwrite(file_buf, 1, file_size, fp);
	fclose(fp);
	
	return 0;
}
*/

Cracker excel_cracker = 
{
	"matrix excel cracker", 
	"xls", 
	CRACK_TYPE_DICTIONARY | CRACK_TYPE_BRUTEFORCE, 
    CRACK_ID_EXCEL, 
	sizeof(OfficeContext), 
	excel_open, 
	office_crack, 
	office_close, 
	NULL
};
