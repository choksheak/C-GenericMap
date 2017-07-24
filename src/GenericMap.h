#ifndef GENERICMAP_H
#define GENERICMAP_H

#include "GenericValue.h"

/*******************************************************************************************/

// Constants.

#define MAP_DEFAULT_INITIAL_CAPACITY 			4
#define MAP_DEFAULT_LOAD_FACTOR_OVER_THOUSAND	600
#define MAP_MIN_LOAD_FACTOR_OVER_THOUSAND	    100
#define MAP_MAX_LOAD_FACTOR_OVER_THOUSAND	    1000

/*******************************************************************************************/

// Data types.

struct gmap_bucket {
	struct gvalue_value key;
	struct gvalue_value value;
	uint32_t hashCode;
	bool freeKeyOnRemove;
	bool freeValueOnRemove;
	struct gmap_bucket *next;
};

struct gmap_ordered_bucket {
	struct gmap_bucket bucket;
	struct gmap_ordered_bucket *prev;
	struct gmap_ordered_bucket *next;
};

// For use in the constructor, like in the Builder pattern.
// Only keyType is required. The rest are optional.
struct gmap_config {
	const struct gvalue_type *keyType;
	uint32_t capacity;
	uint32_t loadFactorOverThousand;
	const struct gvalue_type *restrictValueToType;
	bool maintainInsertionOrder;
	uint32_t (*hashFunc)(struct gvalue_value);
	int (*cmpFunc)(struct gvalue_value, struct gvalue_value);
	bool (*freeFunc)(struct gvalue_value);
};

struct gmap_map {
	struct gmap_config config;
	uint32_t size;
	uint32_t revision;
	struct gmap_bucket **table;
};

struct gmap_ordered_map {
	struct gmap_map map;
	struct gmap_ordered_bucket *firstInsertedBucket;
	struct gmap_ordered_bucket *lastInsertedBucket;
};

struct gmap_iterator {
	struct gmap_map *map;
	struct gvalue_value key;
	struct gvalue_value value;
	uint32_t mapRevision;
	uint32_t currentSlot;
	struct gmap_bucket *nextBucket;
};

struct gmap_keyvalue {
	struct gvalue_value key;
	struct gvalue_value value;
};

struct gmap_keyvalue_list {
	uint32_t size;
	struct gmap_keyvalue *keyValuePairs;
};

// Pseudo class.
struct gmap_class {

	// Constructors.
	struct gmap_map *(*create)(const struct gvalue_type *keyType);
	struct gmap_map *(*create1)(struct gmap_config config);

	// Basic operations.
	bool (*put)(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value);
	bool (*put1)(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value, bool freeKeyOnRemove, bool freeValueOnRemove);
	struct gvalue_value *(*get)(struct gmap_map *map, struct gvalue_value key);
	struct gvalue_value (*getOrDefault)(struct gmap_map *map, struct gvalue_value key, struct gvalue_value defaultValue);
	bool (*containsKey)(struct gmap_map *map, struct gvalue_value key);
	bool (*remove)(struct gmap_map *map, struct gvalue_value key);
	void (*clear)(struct gmap_map *map);

	// More operations.
	struct gmap_iterator (*iterator)(struct gmap_map *map);
	bool (*next)(struct gmap_iterator *iterator);
	struct gmap_keyvalue_list (*getKeyValueList)(struct gmap_map *map);
	void (*freeKeyValueList)(struct gmap_keyvalue_list kvlist);
	void (*each)(struct gmap_map *map, void (*func)(struct gvalue_value, struct gvalue_value));
	void (*print)(struct gmap_map* map);
	void (*fprint)(struct gmap_map* map, FILE *stream);
	float (*hashDeviation)(struct gmap_map* map);

	// Destructor.
	void (*free)(struct gmap_map *map);

};

// OOP class object.
extern struct gmap_class gmap;

/*******************************************************************************************/

// Constructors.

extern struct gmap_map *gmap_create(const struct gvalue_type *keyType);
extern struct gmap_map *gmap_create1(struct gmap_config config);

/*******************************************************************************************/

// Basic operations.

extern bool gmap_put(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value);
extern bool gmap_put1(struct gmap_map *map, struct gvalue_value key, struct gvalue_value value, bool freeKeyOnRemove, bool freeValueOnRemove);
extern struct gvalue_value *gmap_get(struct gmap_map *map, struct gvalue_value key);
extern struct gvalue_value gmap_getOrDefault(struct gmap_map *map, struct gvalue_value key, struct gvalue_value defaultValue);
extern bool gmap_containsKey(struct gmap_map *map, struct gvalue_value key);
extern bool gmap_remove(struct gmap_map *map, struct gvalue_value key);
extern void gmap_clear(struct gmap_map *map);

/*******************************************************************************************/

// More operations.

extern struct gmap_iterator gmap_iterator(struct gmap_map *map);
extern bool gmap_next(struct gmap_iterator *iterator);
extern struct gmap_keyvalue_list gmap_getKeyValueList(struct gmap_map *map);
extern void gmap_freeKeyValueList(struct gmap_keyvalue_list kvlist);
extern void gmap_each(struct gmap_map *map, void (*func)(struct gvalue_value, struct gvalue_value));
extern void gmap_print(struct gmap_map* map);
extern void gmap_fprint(struct gmap_map* map, FILE *stream);
extern float gmap_hashDeviation(struct gmap_map* map);

/*******************************************************************************************/

// Destructor.

extern void gmap_free(struct gmap_map *map);

/*******************************************************************************************/

#endif /* GENERICMAP_H */
