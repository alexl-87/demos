#include <stddef.h>
#ifndef H_Hash
#define H_Hash
//
enum exeStatus {OK, memallocE, paramE, hashingE, collision, True, False, exist};

typedef enum 		exeStatus exeStatus;
typedef struct 		HMap HMap;
typedef struct 		Element Element;

/*
* User functions template
*/
typedef size_t 		(*Hash_u)(void* key);
typedef int 		(*Compare_u)(void* key_1, void* key_2);
typedef exeStatus 	(*ForEachElem_u)(void* key, void* value);

HMap* 		createHMap		(unsigned long capacity, Hash_u hashFunc, Compare_u compareFunc);//Done
exeStatus 	add_by_value	(HMap* map, void* key, size_t key_size, void* value, size_t value_size);//Done
exeStatus 	add_by_reference(HMap* map, void* key, void* value);
exeStatus 	deleteValue		(HMap* map, void* key);
exeStatus 	eraseValue		(HMap* map, void* key);
exeStatus 	getValue		(HMap* map, void* key, void** value);
exeStatus 	forEach			(HMap* map, ForEachElem_u foreachFunc);//Done
exeStatus 	clearHMap 		(HMap* map);
exeStatus 	destroyHMap 	(HMap* map);
size_t 		get_noe			(HMap* map);//Done
size_t 		get_collisions	(HMap* map);//Done
size_t 		djb2			(void* key);/* djb2 set as default string 
											 hashing algorithm in case NULL 
											 parametr recieved as a user hash function
										*/
#endif