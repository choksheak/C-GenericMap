/**
 * Implementation for generic hash set based on GenericMap.
 *
 * The value must be of a single type.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GenericSet.h"

// OOP class object.
struct gset_class gset = {

		// Constructors.
		.create = gset_create,
		.create1 = gset_create1,

		// Basic operations.
		.put = gset_put,
		.put1 = gset_put1,
		.contains = gset_contains,
		.remove = gset_remove,
		.clear = gset_clear,

		// More operations.
		.iterator = gset_iterator,
		.next = gset_next,
		.getValueList = gset_getValueList,
		.freeValueList = gset_freeValueList,
		.each = gset_each,
		.print = gset_print,
		.fprint = gset_fprint,
		.hashDeviation = gset_hashDeviation,

		// Destructor.
		.free = gset_free

};

/*******************************************************************************************/

// Constructors.

struct gset_set *gset_create(const struct gvalue_type *dataType) {
	struct gset_config config = { .dataType = dataType };
	return gset_create1(config);
}

struct gset_set *gset_create1(struct gset_config config) {
	if (config.dataType == NULL) {
		printf("Error: gset: dataType is required\n");
		return NULL;
	}

	struct gmap_config map_config = {
			.keyType = config.dataType,
			.capacity = config.capacity,
			.loadFactorOverThousand = config.loadFactorOverThousand,
			.maintainInsertionOrder = config.maintainInsertionOrder,
			.hashFunc = config.hashFunc,
			.cmpFunc = config.cmpFunc,
			.freeFunc = config.freeFunc
	};

	struct gmap_map *map = gmap_create1(map_config);
	if (map == NULL) {
		return NULL;
	}

	struct gset_set *set = (struct gset_set *) malloc(sizeof(struct gset_set));
	set->size = 0;
	set->map = map;

	return set;
}

/*******************************************************************************************/

// Basic operations.

bool private_gset_checkDataType(struct gvalue_value givenValue, const struct gvalue_type *dataType) {
	if (givenValue.type != dataType) {
		printf("Error: gset: Wrong data type. Expected=%s, Actual=%s\n", dataType->name, givenValue.type->name);
		return false;
	}
	return true;
}

// Returns on success.
bool gset_put(struct gset_set *set, struct gvalue_value value) {
	return gset_put1(set, value, false);
}

bool gset_put1(struct gset_set *set, struct gvalue_value value, bool freeOnRemove) {
	if (private_gset_checkDataType(value, set->map->config.keyType) == false) {
		return false;
	}

	// Always put int 0 as hashmap value.
	if (gmap_put1(set->map, value, gvalue_getInt(0), freeOnRemove, false)) {
		set->size = set->map->size;
		return true;
	}

	return false;
}

bool gset_contains(struct gset_set *set, struct gvalue_value value) {
	return gmap_containsKey(set->map, value);
}

// Returns true if value was removed.
bool gset_remove(struct gset_set *set, struct gvalue_value value) {
	if (private_gset_checkDataType(value, set->map->config.keyType) == false) {
		return false;
	}

	if (gmap_remove(set->map, value)) {
		set->size = set->map->size;
		return true;
	}

	return false;
}

void gset_clear(struct gset_set *set) {
	gmap_clear(set->map);
	set->size = 0;
}

/*******************************************************************************************/

// More operations.

// Iterators do not allocate any memory and hence are very lightweight.
struct gset_iterator gset_iterator(struct gset_set *set) {
	struct gset_iterator iterator = {
			.set = set,
			.mapIterator = gmap_iterator(set->map)
	};
	return iterator;
}

// Returns true if a next key-value is available.
// If you modify the set during iteration, the program will print an error and return false.
//
//   struct gset_iterator iterator = gset.iterator(set);
//   while (gset.next(&iterator)) {
//      struct gvalue_value value = iterator.value;
//   }
//   // nothing to free after the end of iteration
//
bool gset_next(struct gset_iterator *iterator) {
	if (iterator->mapIterator.mapRevision != iterator->set->map->revision) {
		printf("Error: gset: Set modified while iterating\n");
		return false;
	}

	if (gmap_next(&(iterator->mapIterator))) {
		iterator->value = iterator->mapIterator.key;
		return true;
	}

	return false;
}

// Gets a snapshot copy of the entire set, which requires memory allocation.
// Use iterators if you don't need a snapshot copy of the entire set in memory.
//
//   struct gset_value_list list = gset.getValueList(set);
//   for (int i = 0; i < list.size; i++) {
//       struct gvalue_value value = list.values[i];
//   }
//	 gset.freeValueList(list);
//
struct gset_value_list gset_getValueList(struct gset_set *set) {
	struct gset_value_list list = {
			.size = set->size
	};

	if (set->size == 0) {
		list.values = NULL;
		return list;
	}

	list.values = malloc(sizeof(struct gvalue_value) * set->size);

	struct gmap_iterator iterator = gmap_iterator(set->map);
	uint32_t i = 0;
	while (gmap_next(&iterator)) {
		list.values[i++] = iterator.key;
	}

	return list;
}

void gset_freeValueList(struct gset_value_list list) {
	if (list.values != NULL) {
		free(list.values);
	}
}

void gset_each(struct gset_set *set, void (*func)(struct gvalue_value)) {
	struct gmap_iterator iterator = gmap_iterator(set->map);
	while (gmap_next(&iterator)) {
		func(iterator.key);
	}
}

void gset_print(struct gset_set *set) {
	gset_fprint(set, stdout);
}

void gset_fprint(struct gset_set *set, FILE *stream) {
	if (set->size == 0) {
		fputs("()", stream);
		return;
	}

	struct gmap_iterator iterator = gmap_iterator(set->map);
	bool isFirst = true;

	while (gmap_next(&iterator)) {
		if (isFirst) {
			isFirst = false;
		}
		else {
			fputs(", ", stream);
		}

		gvalue_fprint(iterator.key, stream);
	}

	fputs(" )", stream);
}

// Lower score is better.
float gset_hashDeviation(struct gset_set *set) {
	return gmap_hashDeviation(set->map);
}

/*******************************************************************************************/

// Destructor.

void gset_free(struct gset_set *set) {
	gmap_free(set->map);
	free(set);
}
