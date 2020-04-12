#include "H_Hash.h"
#include <stdlib.h>
#define CAPACITY_INC 1.5 

static exeStatus findValueIndex(HMap* map, void* key, size_t* index);
static exeStatus createHashNode(void* key, void* value, Element** new_element);
static exeStatus destroyHashNode(void* key, void* value);

struct Element
{
	void*		 m_value;
	void*		 m_key;
	Element* m_next;
};

struct HMap
{
	size_t		 	m_capacity;
	Element** 		m_bucket;
	Hash_u 	 		m_hashFunc;
	Compare_u	 	m_compareFunc;
};

exeStatus createHMap(size_t capacity, Hash_u hashFunc, Compare_u compareFunc, HMap** hMap)
{

	if(!hashFunc || !compareFunc || !hMap || capacity < 1) return paramE;

	*hMap = (HMap*)malloc(sizeof(HMap));
	if(!*hMap) return mallocE;

	*hMap->m_bucket = (Element*)calloc(capacity*CAPACITY_INC, sizeof(Element*));
	if(!*hMap->m_bucket)
	{
		free(*hMap);
		return mallocE;
	}

	*hMap->m_capacity = capacity*CAPACITY_INC;
	*hMap->m_hashFunc = hashFunc;
	*hMap->m_compareFunc = compareFunc;

	return OK;
}

exeStatus insertValue(HMap* map, void* key, void* value)
{

	int 			result = 0;
	size_t 			index = 0;
	Element* 		new_element;
	Element* 		tempNode;
	void* 			fictitiousValue;

	if(!map || !key || !value)return paramE;

	result = findValue(map, key, &fictitiousValue);
	if(result == True)return exist;

	result = findValueIndex(map, key, &index);
	if(result)return result;

	result = createHashNode(key, value, &new_element);
	if(result)return result;

	if(!map->m_bucket[index])
	{
		map->m_bucket[index] = new_element;
	} 

	else
	{
		tempNode = map->m_bucket[index];
		map->m_bucket[index] = new_element;
		new_element->m_next = tempNode;
	}

	return OK;
}

exeStatus findValue(HMap* map, void* key, void** value)
{
	size_t 		index = 0;
	int 		result = 0;
	Element* 	tempNode;

	/*if(!map || !key || !value)return paramE;
	* disabled in favor of efficiency
	*/

	result = findValueIndex(map, key, &index);
	if(result) return result;

	tempNode = map->m_bucket[index];

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

exeStatus deleteValue(HMap* map, void* key)
{	
	int 		result = 0;
	size_t 		index = 0;
	Element* 	tempNode;
	Element* 	tempPrevNode;

	if(!map || !key)return paramE;

	result = findValueIndex(map, key, &index);
	if(result)return result;
	if(!map->m_bucket[index]) return False;

	tempNode = map->m_bucket[index];
	result  = map->m_compareFunc(key, tempNode->m_key);

	if(result==True)
	{
		destroyHashNode(tempNode->m_key, tempNode->m_value);
		map->m_bucket[index] = tempNode->m_next;
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

exeStatus forEach(HMap* map, userForEachFunction foreachFunc)
{
	int 			i = 0;
	Element* 	tempNode_1;
	Element* 	tempNode_2;

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

exeStatus destroyHMap(HMap* map)
{
	userForEachFunction destroyFunc;
	if (!map)return paramE;

	destroyFunc = destroyHashNode;
	forEach(map, destroyFunc);
	free(map->m_bucket);
	free(map);

	return OK;
}

static exeStatus createHashNode(void* key, void* value, Element** new_element)
{

	*new_element = malloc(sizeof(Element));
	if(!*new_element)return mallocE;

	(*new_element)->m_key = key;
	(*new_element)->m_value = value;
	(*new_element)->m_next = NULL;

	return OK;
}

static exeStatus findValueIndex(HMap* map, void* key, size_t* index)
{
	int result = 0;

	result = map->m_hashFunc(key, index);
	if(result)return hashingE;

	*index = *index%map->m_capacity;
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