#ifndef INTMAP_H
#define INTMAP_H

#include "GenericMap.h"

// Make this a specific type to avoid confusion with gmap_map.
struct intmap_map {
	struct gmap_map gmap;
};

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
	struct intmap_map *(*create)(void);
	struct intmap_map *(*create1)(struct gmap_config config);

	// Basic operations.
	bool (*put)(struct intmap_map *map, int32_t key, int32_t value);
	int32_t (*get)(struct intmap_map *map, int32_t key);
	int32_t (*getOrDefault)(struct intmap_map *map, int32_t key, int32_t defaultValue);
	bool (*containsKey)(struct intmap_map *map, int32_t key);
	bool (*remove)(struct intmap_map *map, int32_t key);
	void (*clear)(struct intmap_map *map);

	// More operations.
	struct intmap_iterator (*iterator)(struct intmap_map *map);
	bool (*next)(struct intmap_iterator *iterator);
	struct intmap_keyvalue_list (*getKeyValueList)(struct intmap_map *map);
	void (*freeKeyValueList)(struct intmap_keyvalue_list kvlist);
	void (*each)(struct intmap_map *map, void (*func)(int32_t, int32_t));
	void (*print)(struct intmap_map* map);
	void (*fprint)(struct intmap_map* map, FILE *stream);
	float (*hashDeviation)(struct intmap_map* map);

	// Destructor.
	void (*free)(struct intmap_map *map);

};

// OOP class object.
extern struct intmap_class intmap;

/*******************************************************************************************/

// Constructors.

extern struct intmap_map *intmap_create(void);
extern struct intmap_map *intmap_create1(struct gmap_config config);

/*******************************************************************************************/

// Basic operations.

extern bool intmap_put(struct intmap_map *map, int32_t key, int32_t value);
extern int32_t intmap_get(struct intmap_map *map, int32_t key);
extern int32_t intmap_getOrDefault(struct intmap_map *map, int32_t key, int32_t defaultValue);
extern bool intmap_containsKey(struct intmap_map *map, int32_t key);
extern bool intmap_remove(struct intmap_map *map, int32_t key);
extern void intmap_clear(struct intmap_map *map);

/*******************************************************************************************/

// More operations.

extern struct intmap_iterator intmap_iterator(struct intmap_map *map);
extern bool intmap_next(struct intmap_iterator *iterator);
extern struct intmap_keyvalue_list intmap_getKeyValueList(struct intmap_map *map);
extern void intmap_freeKeyValueList(struct intmap_keyvalue_list kvlist);
extern void intmap_each(struct intmap_map *map, void (*func)(int32_t, int32_t));
extern void intmap_print(struct intmap_map* map);
extern void intmap_fprint(struct intmap_map* map, FILE *stream);
extern float intmap_hashDeviation(struct intmap_map* map);

/*******************************************************************************************/

// Destructor.

extern void intmap_free(struct intmap_map *map);

/*******************************************************************************************/

#endif /* INTMAP_H */
