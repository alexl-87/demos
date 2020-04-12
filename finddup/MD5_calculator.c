#include <openssl/md5.h> /* -c -lcrypto*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "MD5_calculator.h"
#define BUF_SIZE 512

MD5Status calculateMD5(char* filePath, unsigned char** MD5)
{
	int i;
	MD5_CTX c;
	char buf[BUF_SIZE];
	ssize_t bytes;
	FILE* file;
	int fd;
	unsigned char* newMD5;

	if(!filePath)return MD5_paramE;

	newMD5 = (unsigned char*)malloc(MD5_DIGEST_LENGTH+1);
	if(!newMD5)return MD5_mallocE;

	file = fopen(filePath, "r");
	if(!file)return MD5_fopenE;

	fd = fileno(file);

	MD5_Init(&c);
	bytes=read(fd, buf, BUF_SIZE);

	while(bytes > 0)
	{
		MD5_Update(&c, buf, bytes);
		bytes=read(fd, buf, BUF_SIZE);
	}

	MD5_Final(newMD5, &c);

	newMD5[MD5_DIGEST_LENGTH+1] = '\0';

	*MD5 = newMD5;
	
	fclose(file);
	return MD5_OK;
}
