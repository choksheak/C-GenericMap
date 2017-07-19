#pragma once

#ifndef INTMAP_H
#define INTMAP_H

#include "GenericMap.h"

struct intmap_iterator {
	struct gmap_iterator iterator;
	int32_t key;
	int32_t value;
};

struct intmap_keyvalue {
	int32_t key;
	int32_t value;
};

struct intmap_keyvalue_list {
	uint32_t size;
	struct intmap_keyvalue *keyValuePairs;
};

// Pseudo class.
struct intmap_class {

	// Constructors.
	struct gmap_map *(*create)();
	struct gmap_map *(*create1)(struct gmap_config config);

	// Basic operations.
	bool (*put)(struct gmap_map *map, int32_t key, int32_t value);
	int32_t (*get)(struct gmap_map *map, int32_t key);
	int32_t (*getOrDefault)(struct gmap_map *map, int32_t key, int32_t defaultValue);
	bool (*containsKey)(struct gmap_map *map, int32_t key);
	bool (*remove)(struct gmap_map *map, int32_t key);
	void (*clear)(struct gmap_map *map);

	// More operations.
	struct intmap_iterator (*iterator)(struct gmap_map *map);
	bool (*next)(struct intmap_iterator *iterator);
	struct intmap_keyvalue_list (*getKeyValueList)(struct gmap_map *map);
	void (*freeKeyValueList)(struct intmap_keyvalue_list kvlist);
	void (*each)(struct gmap_map *map, void (*func)(int32_t, int32_t));
	void (*print)(struct gmap_map* map);
	void (*fprint)(struct gmap_map* map, FILE *stream);
	float (*hashDeviation)(struct gmap_map* map);

	// Destructor.
	void (*free)(struct gmap_map *map);

};

// OOP class object.
extern struct intmap_class intmap;

/*******************************************************************************************/

// Constructors.

extern struct gmap_map *intmap_create();
extern struct gmap_map *intmap_create1(struct gmap_config config);

/*******************************************************************************************/

// Basic operations.

extern bool intmap_put(struct gmap_map *map, int32_t key, int32_t value);
extern int32_t intmap_get(struct gmap_map *map, int32_t key);
extern int32_t intmap_getOrDefault(struct gmap_map *map, int32_t key, int32_t defaultValue);
extern bool intmap_containsKey(struct gmap_map *map, int32_t key);
extern bool intmap_remove(struct gmap_map *map, int32_t key);

/*******************************************************************************************/

// More operations.

extern struct intmap_iterator intmap_iterator(struct gmap_map *map);
extern bool intmap_next(struct intmap_iterator *iterator);
extern struct intmap_keyvalue_list intmap_getKeyValueList(struct gmap_map *map);
extern void intmap_freeKeyValueList(struct intmap_keyvalue_list kvlist);
extern void intmap_each(struct gmap_map *map, void (*func)(int32_t, int32_t));

/*******************************************************************************************/

#endif /* INTMAP_H */
