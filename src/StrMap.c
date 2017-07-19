/**
 * Integer to integer map based on GenericMap.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StrMap.h"

// OOP class object.
struct strmap_class strmap = {

		// Constructors.
		.create = strmap_create,
		.create1 = strmap_create1,

		// Basic operations.
		.put = strmap_put,
		.put1 = strmap_put1,
		.get = strmap_get,
		.getOrDefault = strmap_getOrDefault,
		.containsKey = strmap_containsKey,
		.remove = strmap_remove,
		.clear = gmap_clear,

		// More operations.
		.iterator = strmap_iterator,
		.next = strmap_next,
		.getKeyValueList = strmap_getKeyValueList,
		.freeKeyValueList = strmap_freeKeyValueList,
		.each = strmap_each,
		.print = gmap_print,
		.hashDeviation = gmap_hashDeviation,

		// Destructor.
		.free = gmap_free

};

/*******************************************************************************************/

// Constructors.

struct gmap_map *strmap_create() {
	struct gmap_config config = { .keyType = NULL, .restrictValueToType = NULL };
	return strmap_create1(config);
}

struct gmap_map *strmap_create1(struct gmap_config config) {
	if (config.keyType == NULL) {
		config.keyType = gvalue.stringType;
	}
	else if (config.keyType != gvalue.stringType) {
		printf("Error: strmap: keyType must be %s if given.", gvalue.stringType->name);
		return NULL;
	}

	if (config.restrictValueToType == NULL) {
		config.restrictValueToType = gvalue.stringType;
	}
	else if (config.restrictValueToType != gvalue.stringType) {
		printf("Error: strmap: restrictValueToType must be %s if given.", gvalue.stringType->name);
		return NULL;
	}

	return gmap_create1(config);
}

/*******************************************************************************************/


// Basic operations.

bool strmap_put(struct gmap_map *map, char *key, char *value) {
	return gmap_put(map, gvalue_getString(key), gvalue_getString(value));
}

bool strmap_put1(struct gmap_map *map, char *key, char *value, bool freeKeyOnRemove, bool freeValueOnRemove) {
	return gmap_put1(map, gvalue_getString(key), gvalue_getString(value), freeKeyOnRemove, freeValueOnRemove);
}

char *strmap_get(struct gmap_map *map, char *key) {
	return strmap_getOrDefault(map, key, 0);
}

char *strmap_getOrDefault(struct gmap_map *map, char *key, char *defaultValue) {
	struct gvalue_value *value = gmap_get(map, gvalue_getString(key));
	return (value != NULL) ? value->primitive.stringValue : defaultValue;
}

bool strmap_containsKey(struct gmap_map *map, char *key) {
	return gmap_containsKey(map, gvalue_getString(key));
}

bool strmap_remove(struct gmap_map *map, char *key) {
	return gmap_remove(map, gvalue_getString(key));
}

/*******************************************************************************************/

// More operations.

struct strmap_iterator strmap_iterator(struct gmap_map *map) {
	struct gmap_iterator gIterator = gmap_iterator(map);
	struct strmap_iterator iterator = { .iterator = gIterator };
	return iterator;
}

bool strmap_next(struct strmap_iterator *iterator) {
	bool hasNext = gmap_next(&(iterator->iterator));
	if (hasNext) {
		iterator->key = iterator->iterator.key.primitive.stringValue;
		iterator->value = iterator->iterator.value.primitive.stringValue;
	}
	return hasNext;
}

struct strmap_keyvalue_list strmap_getKeyValueList(struct gmap_map *map) {
	struct strmap_keyvalue_list kvlist = {
			.size = map->size,
			.keyValuePairs = (map->size == 0) ? NULL : malloc(sizeof(struct strmap_keyvalue) * map->size)
	};

	if (map->size > 0) {
		struct strmap_iterator iterator = strmap_iterator(map);
		size_t index = 0;
		while (strmap_next(&iterator)) {
			struct strmap_keyvalue kv = { .key = iterator.key, .value = iterator.value };
			kvlist.keyValuePairs[index++] = kv;
		}
	}

	return kvlist;
}

void strmap_freeKeyValueList(struct strmap_keyvalue_list kvlist) {
	if (kvlist.keyValuePairs != NULL) {
		free(kvlist.keyValuePairs);
	}
}

void strmap_each(struct gmap_map *map, void (*func)(char *, char *)) {
	struct strmap_iterator iter = strmap_iterator(map);
	while (strmap_next(&iter)) {
		func(iter.key, iter.value);
	}
}
