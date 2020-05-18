#include "H_Hash.h"
#include <stdlib.h>

static exeStatus findValueIndex(HashMap* map, void* key, unsigned long* retIndex);
static exeStatus createHashNode(void* key, void* value, HashMapNode** newNode);
static exeStatus destroyHashNode(void* key, void* value);

struct HashMapNode
{
	void*		 m_value;
	void*		 m_key;
	HashMapNode* m_next;
};

struct HashMap
{
	unsigned long		 m_capacity;
	HashMapNode**		 m_bucket;
	userHashFunction 	 m_hashFunc;
	userCompareFunction	 m_compareFunc;
};

exeStatus createHashMap(unsigned long capacity, userHashFunction hashFunc, userCompareFunction compareFunc, HashMap** hMap)
{
	HashMap* newMap;

	if(!hashFunc || !compareFunc || !hMap || capacity < 1) return paramE;

	newMap = malloc(sizeof(HashMap));
	if(!newMap) return mallocE;

	newMap->m_bucket = calloc(capacity*1.5, sizeof(HashMapNode*));
	if(!newMap->m_bucket)
	{
		free(newMap);
		return mallocE;
	}

	newMap->m_capacity = capacity*1.5;
	newMap->m_hashFunc = hashFunc;
	newMap->m_compareFunc = compareFunc;

	*hMap = newMap;
	return OK;
}

exeStatus insertValue(HashMap* map, void* key, void* value)
{

	int 			result = 0;
	unsigned long 	retIndex = 0;
	HashMapNode* 	newNode;
	HashMapNode* 	tempNode;
	void* 			fictitiousValue;

	if(!map || !key || !value)return paramE;

	result = findValue(map, key, &fictitiousValue);
	if(result == True)return exist;

	result = findValueIndex(map, key, &retIndex);
	if(result)return result;

	result = createHashNode(key, value, &newNode);
	if(result)return result;

	if(!map->m_bucket[retIndex])
	{
		map->m_bucket[retIndex] = newNode;
	} 

	else
	{
		tempNode = map->m_bucket[retIndex];
		map->m_bucket[retIndex] = newNode;
		newNode->m_next = tempNode;
	}

	return OK;
}

exeStatus findValue(HashMap* map, void* key, void** value)
{
	unsigned long 	retIndex = 0;
	int 			result = -1;
	HashMapNode* 	tempNode;

	if(!map || !key || !value)return paramE;

	result = findValueIndex(map, key, &retIndex);
	if(result) return result;

	tempNode = map->m_bucket[retIndex];

	while(tempNode)
	{
		result = map->m_compareFunc(key, tempNode->m_key);
		if(result == True)
		{
			*value = tempNode->m_value;
			return result;
		}
		tempNode = tempNode->m_next;
	}
	return False;
}

exeStatus deleteValue(HashMap* map, void* key)
{	
	int 			result = 0;
	unsigned long 	retIndex = 0;
	HashMapNode* 	tempNode;
	HashMapNode* 	tempPrevNode;

	if(!map || !key)return paramE;

	result = findValueIndex(map, key, &retIndex);
	if(result)return result;
	if(!map->m_bucket[retIndex]) return False;

	tempNode = map->m_bucket[retIndex];
	result  = map->m_compareFunc(key, tempNode->m_key);

	if(result==True)
	{
		destroyHashNode(tempNode->m_key, tempNode->m_value);
		map->m_bucket[retIndex] = tempNode->m_next;
		free(tempNode);
		return OK;
	}

	else
	{
		tempPrevNode = tempNode;
		tempNode = tempNode->m_next;
		while(tempNode)
		{
			result = map->m_compareFunc(key, tempNode->m_key);
			if(result==True)
			{
				tempPrevNode->m_next = tempNode->m_next;
				destroyHashNode(tempNode->m_key, tempNode->m_value);
				free(tempNode);
				return OK;
			}
			tempPrevNode = tempNode;
			tempNode = tempNode->m_next;
		}
	}
	return False;
}

exeStatus forEach(HashMap* map, userForEachFunction foreachFunc)
{
	int 			i = 0;
	HashMapNode* 	tempNode_1;
	HashMapNode* 	tempNode_2;

	if(!map || !foreachFunc)return paramE;

	for(i; i < map->m_capacity; i++)
	{
		tempNode_1 = map->m_bucket[i];
		while(tempNode_1)
		{
			tempNode_2 = tempNode_1->m_next;
			foreachFunc(tempNode_1->m_key, tempNode_1->m_value);
			tempNode_1 = tempNode_2;
		}
	}
	return OK;
}

exeStatus destroyHashMap(HashMap* map)
{
	userForEachFunction destroyFunc;
	if (!map)return paramE;

	destroyFunc = destroyHashNode;
	forEach(map, destroyFunc);
	free(map->m_bucket);
	free(map);

	return OK;
}

static exeStatus createHashNode(void* key, void* value, HashMapNode** newNode)
{

	*newNode = malloc(sizeof(HashMapNode));
	if(!*newNode)return mallocE;

	(*newNode)->m_key = key;
	(*newNode)->m_value = value;
	(*newNode)->m_next = NULL;

	return OK;
}

static exeStatus findValueIndex(HashMap* map, void* key, unsigned long* retIndex)
{
	int result = 0;

	result = map->m_hashFunc(key, retIndex);
	if(result)return hashingE;

	*retIndex = *retIndex%map->m_capacity;
	return OK;
}

static exeStatus destroyHashNode(void* key, void* value)
{	
	int result = OK;
	if(!key || !value) result = paramE;

	free(key);
	free(value);

	return result;
}