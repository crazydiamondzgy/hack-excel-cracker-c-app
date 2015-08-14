#include <string.h>
#include <stdlib.h>
#include "utils.h"

int match_ext(const char *filename, const char *extensions)
{
    const char *ext, *p;
    char ext1[32], *q;

    if(!filename)
        return 0;

    ext = strrchr(filename, '.');
    if (ext) 
	{
        ext++;
        p = extensions;
        for(;;) 
		{
            q = ext1;
            while (*p != '\0' && *p != ',' && q-ext1<sizeof(ext1)-1)
                *q++ = *p++;
            *q = '\0';
            if (!strcmpi(ext1, ext))
                return 1;
            if (*p == '\0')
                break;
            p++;
        }
    }
    return 0;
}

Cracker *_crack_list_header;
int matrix_crack_register(Cracker *c)
{
	c->next = _crack_list_header;
	_crack_list_header = c;
	return 0;
}


int matrix_crack_open(CrackContext *ctx, char *filename)
{
	char *ext = NULL;
	Cracker *c = NULL;
	int ret = 0;
	unsigned char pw_last;
	unsigned char *p;
	memset(ctx, 0, sizeof(ctx));

	if (filename)
		ctx->input_filename = strdup(filename);
	else
		return -1;

	//
	// search cracker
	//

	for (c=_crack_list_header; c; c=c->next)
	{
		if (match_ext(filename, c->exts))
		{
			break;
		}
	}

	if (c == NULL)
		return -1;

	//
	// if found cracker, alloc the private data
	//

	if (c->priv_data_size)
	{
		ctx->priv_data = malloc(c->priv_data_size);
	}

	//
	// open the cracker
	//

	ret = c->crack_open(ctx);
	if (ret < 0)
		return ret;
	ctx->c = c;

	//
	// open dictionary
	//

	if (ctx->dict_filename)
		ctx->dict_fp = fopen(ctx->dict_filename, "r");
	
	//
	// init brute-force
	//

	
	strcpy(ctx->charset, /*"abcdefghijklmnopqrstuvwxyz*/"0123456789");
	ctx->pw_maxlen = 8;
	
	/*
	while (*cs)
		switch (*cs++)
	{
      case 'a':
		  strcpy ((char *) p, "abcdefghijklmnopqrstuvwxyz");
		  p += 26;
		  break;
		  
      case 'A':
		  strcpy ((char *) p, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		  p += 26;
		  break;
		  
      case '1':
		  strcpy ((char *) p, "0123456789");
		  p += 10;
		  break;
		  
      case '!':
		  strcpy ((char *) p, "!:$%&/()=?{[]}+-*~#");
		  p += 18;
		  break;
		  
      case ':':
		  while (*cs)
			  *p++ = *cs++;
		  break;
		  
      default:
		  printf ("unknown charset specifier, only 'aA1!:' recognized\n");
		  exit (1);
	}
	*p = 0;
	*/
	
	p = ctx->charset;
	pw_last = *p++;
	memset (ctx->pw_next, pw_last, sizeof(ctx->pw_next));

	for (; *p; p++)
	{
		ctx->pw_next[pw_last] = *p;
		pw_last = *p;
	}

	return 0;
}


int matrix_crack(CrackContext *ctx)
{
	char pw[MAX_PASSWORD];
	char *pw_end, pw_last, *p;

	//
	// dictionary crack
	//

	if (ctx->dict_fp)
	{
		while (fgets(pw, MAX_PASSWORD + 1, ctx->dict_fp))
		{
			pw[strlen(pw) - 1] = 0;
			if (ctx->c->crack(ctx, pw, 0))
				return 1;
		}
	}

	//
	// brute-force crack
	//

	pw_last = ctx->charset[0];

	for (p=pw_end=pw+1; pw_end-pw<ctx->pw_maxlen; p=++pw_end)
	{		
		*p = 0;
		while (p > pw)
			*--p = ctx->pw_next[255];
		
		ctx->pw_num++;
		if (ctx->c->crack(ctx, pw, 0))
			return 1;

		for (p = pw_end; p > pw; )
		{
			*p = ctx->pw_next[*--p];
			if (*p != pw_last)
			{
				ctx->pw_num++;
				if (!ctx->c->crack(ctx, pw, 0))
					p = pw_end;
				else
					return 1;
			}
		}
	}

	return 0;
}


int matrix_crack_close(CrackContext *ctx)
{
	//
	// close dictionary
	//

	if (ctx->dict_fp)
	{
		fclose(ctx->dict_fp);
		ctx->dict_fp = NULL;
	}

	//
	// destroy alloced memroy
	//

	if (ctx->priv_data)
	{
		free(ctx->priv_data);
		ctx->priv_data = NULL;
	}

	//
	// close cracker
	//

	if (ctx->input_filename)
	{
		free(ctx->input_filename);
		ctx->input_filename = NULL;
	}

	ctx->c->crack_close(ctx);
	return 0;
}

