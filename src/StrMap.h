#ifndef STRMAP_H
#define STRMAP_H

#include "GenericMap.h"

// Make this a specific type to avoid confusion with gmap_map.
struct strmap_map {
	struct gmap_map gmap;
};

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
	struct strmap_map *(*create)(void);
	struct strmap_map *(*create1)(struct gmap_config config);

	// Basic operations.
	bool (*put)(struct strmap_map *map, char *key, char *value);
	bool (*put1)(struct strmap_map *map, char *key, char *value, bool freeKeyOnRemove, bool freeValueOnRemove);
	char *(*get)(struct strmap_map *map, char *key);
	char *(*getOrDefault)(struct strmap_map *map, char *key, char *defaultValue);
	bool (*containsKey)(struct strmap_map *map, char *key);
	bool (*remove)(struct strmap_map *map, char *key);
	void (*clear)(struct strmap_map *map);

	// More operations.
	struct strmap_iterator (*iterator)(struct strmap_map *map);
	bool (*next)(struct strmap_iterator *iterator);
	struct strmap_keyvalue_list (*getKeyValueList)(struct strmap_map *map);
	void (*freeKeyValueList)(struct strmap_keyvalue_list kvlist);
	void (*each)(struct strmap_map *map, void (*func)(char *, char *));
	void (*print)(struct strmap_map* map);
	void (*fprint)(struct strmap_map* map, FILE *stream);
	float (*hashDeviation)(struct strmap_map* map);

	// Destructor.
	void (*free)(struct strmap_map *map);

};

// OOP class object.
extern struct strmap_class strmap;

/*******************************************************************************************/

// Constructors.

extern struct strmap_map *strmap_create(void);
extern struct strmap_map *strmap_create1(struct gmap_config config);

/*******************************************************************************************/

// Basic operations.

extern bool strmap_put(struct strmap_map *map, char *key, char *value);
extern bool strmap_put1(struct strmap_map *map, char *key, char *value, bool freeKeyOnRemove, bool freeValueOnRemove);
extern char *strmap_get(struct strmap_map *map, char *key);
extern char *strmap_getOrDefault(struct strmap_map *map, char *key, char *defaultValue);
extern bool strmap_containsKey(struct strmap_map *map, char *key);
extern bool strmap_remove(struct strmap_map *map, char *key);
extern void strmap_clear(struct strmap_map *map);

/*******************************************************************************************/

// More operations.

extern struct strmap_iterator strmap_iterator(struct strmap_map *map);
extern bool strmap_next(struct strmap_iterator *iterator);
extern struct strmap_keyvalue_list strmap_getKeyValueList(struct strmap_map *map);
extern void strmap_freeKeyValueList(struct strmap_keyvalue_list kvlist);
extern void strmap_each(struct strmap_map *map, void (*func)(char *, char *));
extern void strmap_print(struct strmap_map* map);
extern void strmap_fprint(struct strmap_map* map, FILE *stream);
extern float strmap_hashDeviation(struct strmap_map* map);

/*******************************************************************************************/

// Destructor.

extern void strmap_free(struct strmap_map *map);

/*******************************************************************************************/

#endif /* STRMAP_H */
