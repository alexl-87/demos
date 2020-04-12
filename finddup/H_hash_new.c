#include "H_Hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CAPACITY_INC_CONST 1.5
exeStatus comparing(void* key_1, void* key_2);
exeStatus eachelem(void* key, void* value);


//End of testing area

static exeStatus probing(HMap* map, Element* element);

struct Element
{
	void* 		m_value;
	void* 		m_key;
	Element* 	m_next;
};

struct HMap
{
	size_t		 	m_capacity;
	size_t 			m_noe;
	size_t 			m_collisions;
	Element** 		m_bucket;
	Hash_u 	 		m_hashFunc;
	Compare_u	 	m_compareFunc;
};

HMap* createHMap(size_t capacity, Hash_u hashFunc, Compare_u compareFunc)
{

	HMap* newmap = NULL;
	newmap = (HMap*)malloc(sizeof(HMap));
	if (!newmap)
	{
		printf("Newmap error handling\n");
	}
	newmap->m_bucket = (Element**)calloc(capacity*CAPACITY_INC_CONST, sizeof(Element*));

	if (!newmap->m_bucket)
	{
		printf("Bucket error handling\n");
		free(newmap);

	}else{
		newmap->m_capacity = capacity*CAPACITY_INC_CONST;
		newmap->m_noe = 0;
		newmap->m_collisions = 0;
		newmap->m_hashFunc = hashFunc;
		newmap->m_compareFunc = compareFunc;
	}

	return newmap;
}

exeStatus add_by_value(HMap* map, void* key, size_t key_size, void* value, size_t value_size)
{

	Element* new_element = (Element*)malloc(sizeof(Element));
	if (!new_element)
	{
		exit(1);
	}

	new_element->m_value = malloc(value_size);
	new_element->m_key = malloc(key_size);

	if (!new_element->m_value || !new_element->m_key)
	{
		free(new_element->m_value);
		free(new_element->m_key);
		exit(1);
	}

	memcpy(new_element->m_key, key, key_size);
	memcpy(new_element->m_value, value, value_size);


	return probing(map, new_element);
}

exeStatus deleteValue(HMap* map, void* key)
{
	return OK;
}

exeStatus eraseValue(HMap* map, void* key)
{
	return OK;
}

exeStatus findValue	(HMap* map, void* key, void** value)
{
	return OK;
}

exeStatus forEach(HMap* map, ForEachElem_u foreachFunc)
{
	size_t i = 0;
	int counter = 0;
	for(i = 0; i < map->m_capacity; ++i)
	{	
		if (map->m_bucket[i] == NULL)
		{
			printf("map->m_bucket[%ld] == NULL\n", i);
		}else{

			foreachFunc(map->m_bucket[i]->m_key, map->m_bucket[i]->m_value);
			printf("counter = %d\n", ++counter);
		}
	}
	return OK;
}

exeStatus destroyHMap(HMap* map)
{
	return OK;
}

size_t djb2 (void* key)
{
	char* str = (char*) key;

	size_t index = 5381;

	int c;

	while((c = *(str++)))
		index = (((index << 5) + index) + c)%SIZE_MAX;

	return index;
}

size_t get_noe(HMap* map)
{
	return map->m_noe;
}

static exeStatus probing(HMap* map, Element* element)
{
	size_t index = -1;
	int collisions = 0;
	//Element** temp = NULL;

	element->m_next = NULL;

	index = map->m_hashFunc(element->m_key)%map->m_capacity;

	while(map->m_bucket[index+collisions] != NULL)
	{
		collisions++;
	}
	if (collisions > 0)
	{
		printf("collisions = %d\n", collisions);
	}
	
	map->m_bucket[index+collisions] = element;
	map->m_noe++;
	map->m_collisions = (map->m_collisions<collisions)?collisions:map->m_collisions;
	return OK;
}

/****************MAIN*/

int main(int argc, char const *argv[])
{
	HMap* map = 0;
	int i = 0;
	printf("Map testing\n");
	char key[6] = {'a', 'a', 'a', 'a', 'a', '\0'};
	char value[6] = {'z', 'z', 'z', 'z', 'z', '\0'};

	
	int size = 126;
	map = createHMap(size, djb2, comparing);
	for (i = 0; i < size; ++i)
	{
		key[i%5]++;
		value[i%5]--;
		add_by_value(map, (void*)key, 5, (void*)value, 5);
	}
	printf("NOE = %ld\n", get_noe(map));
	forEach(map, eachelem);
	

	return 0;
}

exeStatus comparing(void* key_1, void* key_2)
{
	return OK;
}

exeStatus eachelem(void* key, void* value)
{	
	printf("Key = %s\nValue = %s\n",(char*) key, (char*) value);
	return OK;
}


