/**
 * Integer to integer map based on GenericMap.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "IntMap.h"

// OOP class object.
struct intmap_class intmap = {

		// Constructors.
		.create = intmap_create,
		.create1 = intmap_create1,

		// Basic operations.
		.put = intmap_put,
		.get = intmap_get,
		.getOrDefault = intmap_getOrDefault,
		.containsKey = intmap_containsKey,
		.remove = intmap_remove,
		.clear = intmap_clear,

		// More operations.
		.iterator = intmap_iterator,
		.next = intmap_next,
		.getKeyValueList = intmap_getKeyValueList,
		.freeKeyValueList = intmap_freeKeyValueList,
		.each = intmap_each,
		.print = intmap_print,
		.fprint = intmap_fprint,
		.hashDeviation = intmap_hashDeviation,

		// Destructor.
		.free = intmap_free

};

/*******************************************************************************************/

// Constructors.

struct intmap_map *intmap_create(void) {
	struct gmap_config config = { .keyType = NULL, .restrictValueToType = NULL };
	return intmap_create1(config);
}

struct intmap_map *intmap_create1(struct gmap_config config) {
	if (config.keyType == NULL) {
		config.keyType = gvalue.intType;
	}
	else if (config.keyType != gvalue.intType) {
		printf("Error: intmap: keyType must be %s if given.", gvalue.intType->name);
		return NULL;
	}

	if (config.restrictValueToType == NULL) {
		config.restrictValueToType = gvalue.intType;
	}
	else if (config.restrictValueToType != gvalue.intType) {
		printf("Error: intmap: restrictValueToType must be %s if given.", gvalue.intType->name);
		return NULL;
	}

	return (struct intmap_map *) gmap_create1(config);
}

/*******************************************************************************************/


// Basic operations.

bool intmap_put(struct intmap_map *map, int32_t key, int32_t value) {
	return gmap_put(&(map->gmap), gvalue_getInt(key), gvalue_getInt(value));
}

int32_t intmap_get(struct intmap_map *map, int32_t key) {
	return intmap_getOrDefault(map, key, 0);
}

int32_t intmap_getOrDefault(struct intmap_map *map, int32_t key, int32_t defaultValue) {
	struct gvalue_value *value = gmap_get(&(map->gmap), gvalue_getInt(key));
	return (value != NULL) ? value->primitive.intValue : defaultValue;
}

bool intmap_containsKey(struct intmap_map *map, int32_t key) {
	return gmap_containsKey(&(map->gmap), gvalue_getInt(key));
}

bool intmap_remove(struct intmap_map *map, int32_t key) {
	return gmap_remove(&(map->gmap), gvalue_getInt(key));
}

void intmap_clear(struct intmap_map *map) {
	gmap_clear(&(map->gmap));
}

/*******************************************************************************************/

// More operations.

struct intmap_iterator intmap_iterator(struct intmap_map *map) {
	struct gmap_iterator gIterator = gmap_iterator(&(map->gmap));
	struct intmap_iterator iterator = { .iterator = gIterator };
	return iterator;
}

bool intmap_next(struct intmap_iterator *iterator) {
	bool hasNext = gmap_next(&(iterator->iterator));
	if (hasNext) {
		iterator->key = iterator->iterator.key.primitive.intValue;
		iterator->value = iterator->iterator.value.primitive.intValue;
	}
	return hasNext;
}

struct intmap_keyvalue_list intmap_getKeyValueList(struct intmap_map *map) {
	struct intmap_keyvalue_list kvlist = {
			.size = map->gmap.size,
			.keyValuePairs = (map->gmap.size == 0) ? NULL : malloc(sizeof(struct intmap_keyvalue) * map->gmap.size)
	};

	if (map->gmap.size > 0) {
		struct intmap_iterator iterator = intmap_iterator(map);
		size_t index = 0;
		while (intmap_next(&iterator)) {
			struct intmap_keyvalue kv = { .key = iterator.key, .value = iterator.value };
			kvlist.keyValuePairs[index++] = kv;
		}
	}

	return kvlist;
}

void intmap_freeKeyValueList(struct intmap_keyvalue_list kvlist) {
	if (kvlist.keyValuePairs != NULL) {
		free(kvlist.keyValuePairs);
	}
}

void intmap_each(struct intmap_map *map, void (*func)(int32_t, int32_t)) {
	struct intmap_iterator iter = intmap_iterator(map);
	while (intmap_next(&iter)) {
		func(iter.key, iter.value);
	}
}

void intmap_print(struct intmap_map* map) {
	gmap_print(&(map->gmap));
}

void intmap_fprint(struct intmap_map* map, FILE *stream) {
	gmap_fprint(&(map->gmap), stream);
}

float intmap_hashDeviation(struct intmap_map* map) {
	return gmap_hashDeviation(&(map->gmap));
}

/*******************************************************************************************/

// Destructor.

void intmap_free(struct intmap_map *map) {
	gmap_free(&(map->gmap));
}
