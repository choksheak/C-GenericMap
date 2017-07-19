/**
 * Implementation for generic key and generic value hashmaps.
 *
 * Optionally maintains a doubly-linked list among all nodes to preserve insertion order, which will also
 * speed up iteration, like a LinkedHashMap in Java.
 *
 * The key must be of a single type, while the value can be of any type. This is done to prevent equality
 * comparisons between keys of different types, which could become confusing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GenericMap.h"

// OOP class object.
struct gmap_class gmap = {

		// Constructors.
		.create = gmap_create,
		.create1 = gmap_create1,

		// Basic operations.
		.put = gmap_put,
		.put1 = gmap_put1,
		.get = gmap_get,
		.getOrDefault = gmap_getOrDefault,
		.containsKey = gmap_containsKey,
		.remove = gmap_remove,
		.clear = gmap_clear,

		// More operations.
		.iterator = gmap_iterator,
		.next = gmap_next,
		.getKeyValueList = gmap_getKeyValueList,
		.freeKeyValueList = gmap_freeKeyValueList,
		.each = gmap_each,
		.print = gmap_print,
		.hashDeviation = gmap_hashDeviation,

		// Destructor.
		.free = gmap_free

};

/*******************************************************************************************/

// Constructors.

struct gmap_map *gmap_create(const struct gvalue_type *keyType) {
	struct gmap_config config = { .keyType = keyType };
	return gmap_create1(config);
}

struct gmap_map *gmap_create1(struct gmap_config config) {
	if (config.keyType == NULL) {
		printf("Error: gmap: keyType is required\n");
		return NULL;
	}

	// Minimum capacity of 1 is to make sure we don't use calloc with a size of 0.
	// Otherwise we can allow for a minimum capacity of 0.
	if (config.capacity < 1) {
		config.capacity = 1;
	}

	if (config.loadFactorOverThousand < MAP_MIN_LOAD_FACTOR_OVER_THOUSAND
			|| config.loadFactorOverThousand > MAP_MAX_LOAD_FACTOR_OVER_THOUSAND) {
		config.loadFactorOverThousand = MAP_DEFAULT_LOAD_FACTOR_OVER_THOUSAND;
	}

	if (config.hashFunc == NULL) {
		config.hashFunc = gvalue_hash;
	}

	if (config.cmpFunc == NULL) {
		config.cmpFunc = gvalue_cmp;
	}

	if (config.freeFunc == NULL) {
		config.freeFunc = gvalue_free;
	}

	size_t allocSize = config.maintainInsertionOrder ? sizeof(struct gmap_ordered_map) : sizeof(struct gmap_map);
	struct gmap_map *map = (struct gmap_map *) malloc(allocSize);

	map->config = config;
	map->size = 0;
	map->revision = 0;
	map->table = calloc(sizeof(struct gmap_bucket *), config.capacity);

	if (config.maintainInsertionOrder) {
		struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
		m->firstInsertedBucket = NULL;
		m->lastInsertedBucket = NULL;
	}

	return map;
}

/*******************************************************************************************/

// Internal operations.
void private_map_grow(struct gmap_map *map) {
	uint32_t capacity = map->config.capacity;
	uint32_t newCapacity = (capacity <= 3) ? 7 : capacity * 2;
	struct gmap_bucket **newTable = calloc(sizeof(struct gmap_bucket *), newCapacity);

	uint32_t size = map->size;
	if (size > 0) {
		for (uint32_t slot = 0; slot < capacity && size > 0; slot++) {
			struct gmap_bucket *bucket = map->table[slot];
			while (bucket != NULL) {
				uint32_t newSlot = bucket->hashCode % newCapacity;
				struct gmap_bucket *thisNode = bucket;
				bucket = bucket->next;

				thisNode->next = newTable[newSlot];
				newTable[newSlot] = thisNode;

				size--;
			}
		}
	}

	free(map->table);
	map->table = newTable;
	map->config.capacity = newCapacity;
}

void private_freeKeyAndValueIfNeeded(struct gmap_map *map, struct gmap_bucket *node) {
	if (node->freeKeyOnRemove) {
		map->config.freeFunc(node->key);
	}

	if (node->freeValueOnRemove) {
		map->config.freeFunc(node->value);
	}
}

/*******************************************************************************************/

// Basic operations.

bool private_checkKeyType(struct gvalue_value givenKey, const struct gvalue_type *keyType) {
	if (givenKey.type != keyType) {
		printf("Error: gmap: Wrong key type. Expected=%s, Actual=%s\n", keyType->name, givenKey.type->name);
		return false;
	}
	return true;
}

// Returns on success.
bool gmap_put(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value) {
	return gmap_put1(map, key, value, false, false);
}

bool gmap_put1(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value,
		bool freeKeyOnRemove, bool freeValueOnRemove) {

	if (private_checkKeyType(key, map->config.keyType) == false) {
		return false;
	}

	if (map->config.restrictValueToType != NULL && value.type != map->config.restrictValueToType) {
		printf("Error: gmap: Wrong value type. Expected=%s, Actual=%s\n", map->config.restrictValueToType->name, value.type->name);
		return false;
	}

	uint32_t newLoadFactorOverThousand = ((map->size + 1) * 1000) / map->config.capacity;
	if (newLoadFactorOverThousand >= map->config.loadFactorOverThousand) {
		private_map_grow(map);
	}

	uint32_t hashCode = map->config.hashFunc(key);
	uint32_t slot = hashCode % map->config.capacity;
	struct gmap_bucket **addToNode = &(map->table[slot]);

	while (*addToNode != NULL) {
		if (map->config.cmpFunc((*addToNode)->key, key) == 0) {

			private_freeKeyAndValueIfNeeded(map, (*addToNode));

			(*addToNode)->key = key;
			(*addToNode)->value = value;
			(*addToNode)->hashCode = hashCode;
			(*addToNode)->freeKeyOnRemove = freeKeyOnRemove;
			(*addToNode)->freeValueOnRemove = freeValueOnRemove;

			// Move this bucket to the last position.
			if (map->config.maintainInsertionOrder) {
				struct gmap_ordered_bucket *b = (struct gmap_ordered_bucket *) (*addToNode);

				if (b->next != NULL) {
					struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
					struct gmap_ordered_bucket *prev = b->prev;
					struct gmap_ordered_bucket *next = b->next;

					if (prev != NULL) {
						prev->next = next;
					}
					else {
						m->firstInsertedBucket = next;
					}

					next->prev = prev;

					if (m->lastInsertedBucket->prev != NULL) {
						m->lastInsertedBucket->prev->next = b;
					}

					b->prev = m->lastInsertedBucket;
					m->lastInsertedBucket = b;
					b->next = NULL;
				}
			}

			map->revision++;
			return false;
		}

		addToNode = &((*addToNode)->next);
	}

	size_t allocSize = map->config.maintainInsertionOrder ? sizeof(struct gmap_ordered_bucket) : sizeof(struct gmap_bucket);
	struct gmap_bucket *list = malloc(allocSize);
	list->key = key;
	list->value = value;
	list->hashCode = hashCode;
	list->freeKeyOnRemove = freeKeyOnRemove;
	list->freeValueOnRemove = freeValueOnRemove;
	list->next = NULL;

	if (map->config.maintainInsertionOrder) {
		struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
		struct gmap_ordered_bucket *b = (struct gmap_ordered_bucket *) list;

		if (m->firstInsertedBucket == NULL) {
			m->firstInsertedBucket = b;
			b->prev = NULL;
		}
		else {
			b->prev = m->lastInsertedBucket;
			m->lastInsertedBucket->next = b;
		}

		b->next = NULL;
		m->lastInsertedBucket = b;
	}

	*addToNode = list;
	map->size++;
	map->revision++;
	return true;
}

// Returns NULL if key is not found.
struct gvalue_value *gmap_get(struct gmap_map *map, struct gvalue_value key) {
	if (private_checkKeyType(key, map->config.keyType) == false) {
		return NULL;
	}

	if (map->size == 0) {
		return NULL;
	}

	uint32_t hashCode = map->config.hashFunc(key);
	uint32_t slot = hashCode % map->config.capacity;
	struct gmap_bucket *bucket = map->table[slot];

	while (bucket != NULL) {
		if (map->config.cmpFunc(bucket->key, key) == 0) {
			return &(bucket->value);
		}
		bucket = bucket->next;
	}

	return NULL;
}

struct gvalue_value gmap_getOrDefault(struct gmap_map *map, struct gvalue_value key, struct gvalue_value defaultValue) {
	struct gvalue_value *value = gmap_get(map, key);
	return (value != NULL) ? *value : defaultValue;
}

bool gmap_containsKey(struct gmap_map *map, struct gvalue_value key) {
	return gmap_get(map, key) != NULL;
}

// Returns true if key was removed.
bool gmap_remove(struct gmap_map *map, struct gvalue_value key) {
	if (private_checkKeyType(key, map->config.keyType) == false) {
		return false;
	}

	if (map->size == 0) {
		return false;
	}

	uint32_t hashCode = map->config.hashFunc(key);
	uint32_t slot = hashCode % map->config.capacity;
	struct gmap_bucket **removeFromNode = &(map->table[slot]);

	while (*removeFromNode != NULL) {
		if (map->config.cmpFunc((*removeFromNode)->key, key) == 0) {
			struct gmap_bucket *removedNode = *removeFromNode;
			*removeFromNode = (*removeFromNode)->next;

			if (map->config.maintainInsertionOrder) {
				struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;

				if (map->size == 1) {
					m->firstInsertedBucket = NULL;
					m->lastInsertedBucket = NULL;
				}
				else {
					struct gmap_ordered_bucket *b = (struct gmap_ordered_bucket *) removedNode;
					struct gmap_ordered_bucket *prev = b->prev;
					struct gmap_ordered_bucket *next = b->next;

					if (prev != NULL) {
						prev->next = next;
					}
					else {
						m->firstInsertedBucket = next;
					}

					if (next != NULL) {
						next->prev = prev;
					}
					else {
						m->lastInsertedBucket = prev;
					}
				}
			}

			private_freeKeyAndValueIfNeeded(map, removedNode);
			free(removedNode);

			map->size--;
			map->revision++;
			return true;
		}

		removeFromNode = &((*removeFromNode)->next);
	}

	return false;
}

void gmap_clear(struct gmap_map *map) {
	uint32_t size = map->size;

	if (size == 0) {
		return;
	}

	if (map->config.maintainInsertionOrder) {
		struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
		struct gmap_ordered_bucket *b = m->firstInsertedBucket;

		do {
			struct gmap_ordered_bucket *next = b->next;

			private_freeKeyAndValueIfNeeded(map, (struct gmap_bucket *) b);

			free(b);
			b = next;
		}
		while (b != NULL);

		map->size = 0;
		map->revision = 0;
		m->firstInsertedBucket = NULL;
		m->lastInsertedBucket = NULL;

		for (size_t i = 0, j = map->config.capacity - 1; i < j; i++) {
			map->table[i] = NULL;
		}

		return;
	}

	for (uint32_t slot = 0; slot < map->config.capacity; slot++) {
		struct gmap_bucket *bucket = map->table[slot];
		map->table[slot] = NULL;

		while (bucket != NULL) {
			struct gmap_bucket *next = bucket->next;

			private_freeKeyAndValueIfNeeded(map, bucket);

			free(bucket);
			bucket = next;

			size--;
			if (size == 0) {
				map->size = 0;
				map->revision = 0;
				return;
			}
		}
	}
}

/*******************************************************************************************/

// More operations.

// Iterators do not allocate any memory and hence are very lightweight.
struct gmap_iterator gmap_iterator(struct gmap_map *map) {
	struct gmap_iterator iter;
	iter.map = map;
	iter.mapRevision = map->revision;

	if (map->config.maintainInsertionOrder) {
		struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
		iter.currentSlot = -1;
		iter.nextBucket = (struct gmap_bucket *) (m->firstInsertedBucket);
	}
	else {
		iter.currentSlot = 1;
		iter.nextBucket = map->table[0];
	}

	return iter;
}

// Returns true if a next key-value is available.
// If you modify the map during iteration, the program will print an error and return false.
//
//   struct gmap_iterator iter = gmap.iterator(map);
//   while (gmap.next(&iter)) {
//      struct gvalue_value key = iter.key;
//      struct gvalue_value value = iter.value;
//   }
//   // nothing to free after the end of iteration
//
bool gmap_next(struct gmap_iterator *iterator) {
	if (iterator->mapRevision != iterator->map->revision) {
		puts("Map modified while iterating\n");
		return false;
	}

	if (iterator->map->config.maintainInsertionOrder) {
		if (iterator->nextBucket == NULL) {
			return false;
		}

		struct gmap_ordered_bucket *b = (struct gmap_ordered_bucket *) (iterator->nextBucket);

		iterator->key = iterator->nextBucket->key;
		iterator->value = iterator->nextBucket->value;
		iterator->nextBucket = (struct gmap_bucket *) (b->next);

		return true;
	}

	while (iterator->nextBucket == NULL && iterator->currentSlot < iterator->map->config.capacity) {
		iterator->nextBucket = iterator->map->table[iterator->currentSlot++];
	}

	if (iterator->nextBucket == NULL) {
		return false;
	}

	iterator->key = iterator->nextBucket->key;
	iterator->value = iterator->nextBucket->value;
	iterator->nextBucket = iterator->nextBucket->next;
	return true;
}

// Gets a snapshot copy of the entire map, which requires memory allocation.
// Use iterators if you don't need a snapshot copy of the entire map in memory.
//
//   struct gmap_keyvalue_list kvlist = gmap.getKeyValueList(map);
//   for (int i = 0; i < kvlist.size; i++) {
//       struct gmap_keyvalue pair = kvlist.keyValuePairs[i];
//       struct gval key = pair.key;
//       struct gval value = pair.value;
//   }
//	 gmap.freeKeyValueList(kvlist);
//
struct gmap_keyvalue_list gmap_getKeyValueList(struct gmap_map *map) {
	struct gmap_keyvalue_list list;
	uint32_t size = map->size;
	list.size = size;

	if (size == 0) {
		list.keyValuePairs = NULL;
	}
	else {
		list.keyValuePairs = malloc(sizeof(struct gmap_keyvalue) * size);

		if (map->config.maintainInsertionOrder) {
			struct gmap_ordered_map *m = (struct gmap_ordered_map *) map;
			struct gmap_ordered_bucket *b = m->firstInsertedBucket;

			for (int i = 0; i < size; i++) {
				struct gmap_keyvalue pair = { .key = b->bucket.key, .value = b->bucket.value };
				list.keyValuePairs[i] = pair;
				b = b->next;
			}
		}
		else {
			for (uint32_t slot = 0, index = 0; slot < map->config.capacity && size > 0; slot++) {
				struct gmap_bucket *bucket = map->table[slot];
				while (bucket != NULL) {
					struct gmap_keyvalue pair = { .key = bucket->key, .value = bucket->value };
					list.keyValuePairs[index++] = pair;
					bucket = bucket->next;
					size--;
				}
			}
		}
	}

	return list;
}

void gmap_freeKeyValueList(struct gmap_keyvalue_list kvlist) {
	if (kvlist.keyValuePairs != NULL) {
		free(kvlist.keyValuePairs);
	}
}

void gmap_each(struct gmap_map *map, void (*func)(struct gvalue_value, struct gvalue_value)) {
	struct gmap_iterator iter = gmap_iterator(map);
	while (gmap_next(&iter)) {
		func(iter.key, iter.value);
	}
}

void gmap_print(struct gmap_map *map) {
	gmap_fprint(map, stdout);
}

void gmap_fprint(struct gmap_map *map, FILE *stream) {
	if (map->size == 0) {
		fputs("{}", stream);
		return;
	}

	int notFirst = 0;
	fputs("{ ", stream);

	for (int slot = map->config.capacity - 1; slot >= 0; slot--) {
		struct gmap_bucket *listNode = map->table[slot];
		while (listNode != NULL) {
			if (notFirst) {
				fputc(' ', stream);
			}
			else {
				notFirst = 1;
			}

			fputc('{', stream);
			gvalue_fprint(listNode->key, stream);
			fputc('=', stream);
			gvalue_fprint(listNode->value, stream);
			fputc('}', stream);

			listNode = listNode->next;
		}
	}

	fputs(" }", stream);
}

// Lower score is better.
float gmap_hashDeviation(struct gmap_map *map) {
	if (map->size == 0) {
		return 0;
	}

	float average = map->size / map->config.capacity;
	float score = 0;

	for (int slot = map->config.capacity - 1; slot >= 0; slot--) {
		struct gmap_bucket *listNode = map->table[slot];
		int listSize = 0;
		while (listNode != NULL) {
			listSize++;
			listNode = listNode->next;
		}

		// Only overages are counted. Underages are good.
		float overage = ((float)listSize) - average;

		if (overage > 0) {
			score += overage;
		}
	}

	return score / map->config.capacity;
}

/*******************************************************************************************/

// Destructor.

void gmap_free(struct gmap_map *map) {
	gmap_clear(map);
	free(map->table);
	free(map);
}
