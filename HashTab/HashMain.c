#include "H_Hash.h"
#include <stdio.h>
#include <stdlib.h>

	HashMap* hMap;
	userHashFunction hashFunc;
	userCompareFunction compareFunc;
	unsigned long capacity = 10;
	int result = 0;

void createHashMapTest();
void insertValueTest();
void deleteValueTest();
void forEachTest();
void findValueTest();
exeStatus perfectHash(void* key, unsigned long* index);
exeStatus compareKeys(void* key_1, void* key_2);
exeStatus printMap(void* key, void* value);

int main()
{
	
	system("clear");
	createHashMapTest();
	insertValueTest();
	deleteValueTest();
	findValueTest();


	return 0;
}

void createHashMapTest()
{
	hashFunc = perfectHash;
	compareFunc = compareKeys;
	printf("*** CREATE TESTING ***\n");

	result = createHashMap(capacity, NULL, NULL, &hMap);
	if(result != paramE) 
	{
		printf("*** NULL ARGUMENTS FAIL ***\n");
		return;
	}
	result = createHashMap(0, hashFunc, compareFunc, &hMap);

	if(result != paramE) 
	{
		printf("*** CAPACITY < 0 FAIL ***\n");
		return;
	}

	result = createHashMap(capacity, hashFunc, compareFunc, NULL);
	if(result != paramE) 
	{
		printf("*** NULL MAP FAIL ***\n");
		return;
	}
	result = createHashMap(capacity, hashFunc, compareFunc, &hMap);
	if(result != OK) 
	{
		printf("*** NULL MAP FAIL ***\n");
		return;
	}
	forEachTest(&hMap);
	destroyHashMap(hMap);
	printf("*** PASS ***\n");
}

void insertValueTest()
{
	char* key = NULL;
	int* value = NULL;
	int i = 0;
	hashFunc = perfectHash;
	compareFunc = compareKeys;

	createHashMap(capacity, hashFunc, compareFunc, &hMap);

	result =  insertValue(hMap, (void*) key, (void*) value);
	if(result != paramE) 
	{
		printf("*** NULL ARGUMENTS FAIL ***\n");
		return;
	}

	for(i; i < 8; i++)
	{
		printf("Insert key\n");
		key = malloc(20);
		scanf("%s", key);
		printf("Insert value\n");
		value = malloc(sizeof(int));
		scanf("%d", value);
		result = insertValue(hMap, (void*) key, (void*) value);
	}
		key = malloc(20);
		value = malloc(sizeof(int));
		key = "five";
		*value = 5;
		result = insertValue(hMap, (void*) key, (void*) value);
		if(result != exist)
		{
			printf("*** ALREADY EXISTING ELEMENT FAIL ***\n");
			free(key);
			free(value);
			return;
		}

	forEachTest(hMap);
	printf("*** PASS ***\n");
}

void deleteValueTest()
{	
	char* key = NULL;
	int i = 0;
	printf("*** DELETE VALUE TESTING ***\n");

	for(i; i < 4; i++)
	{
		printf("Insert key\n");
		key = malloc(20);
		scanf("%s", key);
		deleteValue(hMap, key);
		forEachTest(hMap);
	}
	result = deleteValue(hMap, NULL);
	if(result != paramE) 
	{
		printf("*** NULL ARGUMENTS FAIL ***\n");
		return;
	}
	printf("*** PASS ***\n");
}

void forEachTest()
{
	userForEachFunction forEachFunc;
	forEachFunc = printMap;
	forEach(hMap, forEachFunc);
}


void findValueTest()
{
	char key[20];
	int* valueAdress;
	int i = 0;
	printf("*** FIND VALUE TESTING ***\n");

	for(i; i < 3; i++)
	{
		printf("Insert key:\n");
		scanf("%s", key);

		result = findValue(hMap, (void*) key, (void**) &valueAdress);

		if(result == True)
		{
		printf("Key: %s\t Value: %d\n",key, *valueAdress);
		}

		else
		{
		printf("Not founded\n");
		}
	}
	result = findValue(NULL, (void*) key, NULL);
	if(result != paramE) 
	{
		printf("*** NULL MAP FAIL ***\n");
		return;
	}
	destroyHashMap(hMap);
	printf("*** PASS ***\n");
}

exeStatus printMap(void* key, void* value)
{
	char* _key;
	int* _value;
	_key = (char*)key;
	_value = (int*)value;
	printf("Key: %s\t Value: %d\n",_key, *_value);
}

exeStatus perfectHash(void* key, unsigned long* index)
{

	char* thisKey;
	int i = 0;
	int thisIndex = 0;
	thisKey = (char*)key;
	while(thisKey[i] != '\0')
	{
		*index+=thisKey[i];
		i++;
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