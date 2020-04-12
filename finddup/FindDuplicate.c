#include "H_Hash.h"
#include "DirWalk.h"
#include "MD5_calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashMap* hMap;
userHashFunction hashFunc;
userCompareFunction compareFunc;
onFileFoundedAction action;
unsigned long capacity = 10000000;
int result = 0;

void createHMap();
void printScaningResults(char* filePath);
exeStatus perfectHash(void* key, unsigned long* index);
exeStatus compareKeys(void* key_1, void* key_2);

int main(int argc, char const* argv[])
{
	char* dirPath;
	system("clear");

	if(argc>1)dirPath = (char*)argv[1];
	else dirPath = ".";

	createHMap();
	action = printScaningResults;
	getFiles(dirPath, action);
	destroyHashMap(hMap);

	return 0;
}

void createHMap()
{
	hashFunc = perfectHash;
	compareFunc = compareKeys;
	result = createHashMap(capacity, hashFunc, compareFunc, &hMap);
	if(result != OK) exit(1);
}

exeStatus perfectHash(void* key, unsigned long* index)
{

	unsigned char* thisKey;
	int i = 0;
	int p = 31;
	unsigned long p_pow = 1;
	thisKey = (unsigned char*)key;

	for(i; i<16;i++)
	{
		*index = (*index + (thisKey[i]-'a'+1)*p_pow)%capacity;
		p_pow = (p_pow*p)%capacity;
	}

	return OK;
}

exeStatus compareKeys(void* key_1, void* key_2)
{
	char* thisKey_1;
	char* thisKey_2;

	thisKey_1 = (char*) key_1;
	thisKey_2 = (char*) key_2;
	result = strcmp(thisKey_1, thisKey_2);

	if(result) return False;

	return True;
}

void printScaningResults(char* filePath)
{
	int i = 0;
	int filePathLen;
	unsigned char* MD5;
	char* newValue;
	char* originalValue;

	filePathLen = strlen(filePath);

	newValue = malloc(filePathLen+1);
	if(!newValue)
	{
		printf("malloc failed\n");
		exit(result);
	}

	strcpy(newValue, filePath);

	result = calculateMD5(filePath, &MD5);
	if(!newValue)
	{
		printf("MD5 calculation failed\n");
		exit(result);
	}

	result =  insertValue(hMap, (void*) MD5, (void*) newValue);

	if(result == exist)
	{
		result = findValue(hMap, (void*) MD5, (void**) &originalValue);
		printf("**********************************************************************\n\n"
				"File:\t%s\n"
				"Exist in: %s\n\n"
				"**********************************************************************\n", 
				newValue, originalValue);
		free(newValue);
	}	
}
