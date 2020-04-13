#include "H_Hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define CAPACITY_INC_CONST 2
exeStatus eachelem(void* key, void* value);
static size_t probing(HMap* map, void* key, void** ret_key);
static int are_equal(void* key_1, void* key_2);

struct Element
{
	void* 		m_value;
	void* 		m_key;
};

struct HMap
{
	size_t		 	m_capacity;
	size_t 			m_noe;
	size_t 			m_collisions;
	Element** 		m_buckets;
	Hash_u 	 		m_hashFunc;
	Compare_u	 	m_compareFunc;
	char 			m_err_msg[32];
};

HMap* createHMap(size_t capacity, Hash_u hashFunc, Compare_u compareFunc)
{

	HMap* newmap = NULL;
	newmap = (HMap*)malloc(sizeof(HMap));
	if (!newmap)
	{
		strcpy(newmap->m_err_msg, "memalloc failed: new map");
	}
	newmap->m_buckets = (Element**)calloc(capacity*CAPACITY_INC_CONST, sizeof(Element*));

	if (!newmap->m_buckets)
	{
		strcpy(newmap->m_err_msg, "memalloc failed: new buckets");
		free(newmap);
		newmap = NULL;

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

	exeStatus retval = OK;
	size_t index = -1;
	void* ret_key = NULL;
	Element* new_element = (Element*)malloc(sizeof(Element));
	if (!new_element)
	{
		strcpy(map->m_err_msg, "memalloc failed: new element");
		retval = memallocE;
	}
	else
	{

		new_element->m_value = malloc(value_size);
		new_element->m_key = malloc(key_size);

		if (!new_element->m_value || !new_element->m_key)
		{
			strcpy(map->m_err_msg, "memalloc failed: new element");
			retval = memallocE;
			free(new_element->m_value);
			free(new_element->m_key);
			free(new_element);

		}
		else
		{
			memcpy(new_element->m_key, key, key_size);
			memcpy(new_element->m_value, value, value_size);
			index = probing(map, new_element->m_key, &ret_key);
			map->m_buckets[index] = new_element;
		}
	}
	return retval;
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
	for(i = 0; i < map->m_capacity; ++i)
	{	
		foreachFunc(map->m_buckets[i]->m_key, map->m_buckets[i]->m_value);
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
size_t get_collisions(HMap* map)
{
	return map->m_collisions;
}

static size_t probing(HMap* map, void* key, void** ret_key)
{
	size_t index = -1;
	int collisions = 0;
	size_t capacity = map->m_capacity;

	index = map->m_hashFunc(key)%capacity;

	while(map->m_buckets[(index+collisions)%capacity] != NULL)
	{
		if (map->m_compareFunc(key, map->m_buckets[(index+collisions)%capacity]->m_key))
		{
			*ret_key = map->m_buckets[(index+collisions)%capacity]->m_key;
			return (index+collisions)%capacity;
		}
		collisions++;
	}

	map->m_noe++;
	map->m_collisions = (map->m_collisions<(size_t)collisions)?collisions:map->m_collisions;
	*ret_key = NULL;

	return (index+collisions)%capacity;
}

/****************MAIN*/

int main(int argc, char const *argv[])
{
	HMap* map = 0;
	int i = 0;
	printf("Map testing\n");
	char key[8];
	char value[8];
	int j = 0;

	srand(time(0));
	
	int size = atoi(argv[1]);
	map = createHMap(size, djb2, are_equal);
	if (map)
	{
		for (i = 0; i < size; ++i)
		{
			for(j = 0; j < 7; j++)
			{
				key[j] = (char)((rand()%(122-97+1))+97);
				value[j] = (char)((rand()%(122-97+1))+97);
			}
		key[j] = '\0';
		value[j] = '\0';
		add_by_value(map, (void*)key, 8, (void*)value, 8);
	}

	printf("NOE = %ld\n", get_noe(map));
	printf("Collisions = %ld\n", get_collisions(map) );
	//forEach(map, eachelem);
	}

	

	return 0;
}

static int are_equal(void* key_1, void* key_2)
{
	return !strcmp((char*)key_1, (char*)key_2);
}

exeStatus eachelem(void* key, void* value)
{	
	printf("%s\n",(char*) key);
	return OK;
}


