#include <stddef.h>
#ifndef H_Hash
#define H_Hash
//
enum exeStatus {OK, mallocE, paramE, hashingE, collision, True, False, exist};

typedef enum 		exeStatus exeStatus;
typedef struct 		HMap HMap;
typedef struct 		Element Element;

/*
* User functions template
*/
typedef size_t 		(*Hash_u)(void* key);
typedef exeStatus 	(*Compare_u)(void* key_1, void* key_2);
typedef exeStatus 	(*ForEachElem_u)(void* key, void* value);

HMap* 		createHMap		(unsigned long capacity, Hash_u hashFunc, Compare_u compareFunc);
exeStatus 	add_by_value	(HMap* map, void* key, size_t key_size, void* value, size_t value_size);
exeStatus 	add_by_reference(HMap* map, void* key, void* value);
exeStatus 	deleteValue		(HMap* map, void* key);
exeStatus 	eraseValue		(HMap* map, void* key);
exeStatus 	findValue		(HMap* map, void* key, void** value);
exeStatus 	forEach			(HMap* map, ForEachElem_u foreachFunc);
exeStatus 	clearHMap 		(HMap* map);
exeStatus 	destroyHMap 	(HMap* map);
size_t 		get_noe			(HMap* map);
size_t 		djb2			(void* key);/* djb2 set as default string 
											 hashing algorithm in case NULL 
											 parametr recieved as a user hash function
										*/
#endif