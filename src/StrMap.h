#pragma once

#ifndef STRMAP_H
#define STRMAP_H

#include "GenericMap.h"

struct strmap_iterator {
	struct gmap_iterator iterator;
	char *key;
	char *value;
};

struct strmap_keyvalue {
	char *key;
	char *value;
};

struct strmap_keyvalue_list {
	uint32_t size;
	struct strmap_keyvalue *keyValuePairs;
};

// Pseudo class.
struct strmap_class {

	// Constructors.
	struct gmap_map *(*create)(void);
	struct gmap_map *(*create1)(struct gmap_config config);

	// Basic operations.
	bool (*put)(struct gmap_map *map, char *key, char *value);
	bool (*put1)(struct gmap_map *map, char *key, char *value, bool freeKeyOnRemove, bool freeValueOnRemove);
	char *(*get)(struct gmap_map *map, char *key);
	char *(*getOrDefault)(struct gmap_map *map, char *key, char *defaultValue);
	bool (*containsKey)(struct gmap_map *map, char *key);
	bool (*remove)(struct gmap_map *map, char *key);
	void (*clear)(struct gmap_map *map);

	// More operations.
	struct strmap_iterator (*iterator)(struct gmap_map *map);
	bool (*next)(struct strmap_iterator *iterator);
	struct strmap_keyvalue_list (*getKeyValueList)(struct gmap_map *map);
	void (*freeKeyValueList)(struct strmap_keyvalue_list kvlist);
	void (*each)(struct gmap_map *map, void (*func)(char *, char *));
	void (*print)(struct gmap_map* map);
	void (*fprint)(struct gmap_map* map, FILE *stream);
	float (*hashDeviation)(struct gmap_map* map);

	// Destructor.
	void (*free)(struct gmap_map *map);

};

// OOP class object.
extern struct strmap_class strmap;

/*******************************************************************************************/

// Constructors.

extern struct gmap_map *strmap_create(void);
extern struct gmap_map *strmap_create1(struct gmap_config config);

/*******************************************************************************************/

// Basic operations.

extern bool strmap_put(struct gmap_map *map, char *key, char *value);
extern bool strmap_put1(struct gmap_map *map, char *key, char *value, bool freeKeyOnRemove, bool freeValueOnRemove);
extern char *strmap_get(struct gmap_map *map, char *key);
extern char *strmap_getOrDefault(struct gmap_map *map, char *key, char *defaultValue);
extern bool strmap_containsKey(struct gmap_map *map, char *key);
extern bool strmap_remove(struct gmap_map *map, char *key);

/*******************************************************************************************/

// More operations.

extern struct strmap_iterator strmap_iterator(struct gmap_map *map);
extern bool strmap_next(struct strmap_iterator *iterator);
extern struct strmap_keyvalue_list strmap_getKeyValueList(struct gmap_map *map);
extern void strmap_freeKeyValueList(struct strmap_keyvalue_list kvlist);
extern void strmap_each(struct gmap_map *map, void (*func)(char *, char *));

/*******************************************************************************************/

#endif /* STRMAP_H */
