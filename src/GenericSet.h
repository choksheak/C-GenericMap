#ifndef GENERICSET_H
#define GENERICSET_H

#include "GenericMap.h"

/*******************************************************************************************/

// Data types.

// For use in the constructor, like in the Builder pattern.
// Only dataType is required. The rest are optional.
struct gset_config {
	const struct gvalue_type *dataType;
	uint32_t capacity;
	uint32_t loadFactorOverThousand;
	bool maintainInsertionOrder;
	uint32_t (*hashFunc)(struct gvalue_value);
	int (*cmpFunc)(struct gvalue_value, struct gvalue_value);
	bool (*freeFunc)(struct gvalue_value);
};

// A generic set relies on a generic map as the underlying storage.
struct gset_set {
	uint32_t size;
	struct gmap_map *map;
};

struct gset_iterator {
	struct gset_set *set;
	struct gvalue_value value;
	struct gmap_iterator mapIterator;
};

struct gset_value_list {
	uint32_t size;
	struct gvalue_value *values;
};

// Pseudo class.
struct gset_class {

	// Constructors.
	struct gset_set *(*create)(const struct gvalue_type *dataType);
	struct gset_set *(*create1)(struct gset_config config);

	// Basic operations.
	bool (*put)(struct gset_set *set, struct gvalue_value value);
	bool (*put1)(struct gset_set *set, struct gvalue_value value, bool freeOnRemove);
	bool (*contains)(struct gset_set *set, struct gvalue_value value);
	bool (*remove)(struct gset_set *set, struct gvalue_value value);
	void (*clear)(struct gset_set *set);

	// More operations.
	struct gset_iterator (*iterator)(struct gset_set *set);
	bool (*next)(struct gset_iterator *iterator);
	struct gset_value_list (*getValueList)(struct gset_set *set);
	void (*freeValueList)(struct gset_value_list list);
	void (*each)(struct gset_set *set, void (*func)(struct gvalue_value));
	void (*print)(struct gset_set* set);
	void (*fprint)(struct gset_set* set, FILE *stream);
	float (*hashDeviation)(struct gset_set* set);

	// Destructor.
	void (*free)(struct gset_set *set);

};

// OOP class object.
extern struct gset_class gset;

/*******************************************************************************************/

// Constructors.

extern struct gset_set *gset_create(const struct gvalue_type *dataType);
extern struct gset_set *gset_create1(struct gset_config config);

/*******************************************************************************************/

// Basic operations.

extern bool gset_put(struct gset_set *set, struct gvalue_value value);
extern bool gset_put1(struct gset_set *set, struct gvalue_value value, bool freeOnRemove);
extern bool gset_contains(struct gset_set *set, struct gvalue_value value);
extern bool gset_remove(struct gset_set *set, struct gvalue_value value);
extern void gset_clear(struct gset_set *set);

/*******************************************************************************************/

// More operations.

extern struct gset_iterator gset_iterator(struct gset_set *set);
extern bool gset_next(struct gset_iterator *iterator);
extern struct gset_value_list gset_getValueList(struct gset_set *set);
extern void gset_freeValueList(struct gset_value_list list);
extern void gset_each(struct gset_set *set, void (*func)(struct gvalue_value));
extern void gset_print(struct gset_set* set);
extern void gset_fprint(struct gset_set* set, FILE *stream);
extern float gset_hashDeviation(struct gset_set* set);

/*******************************************************************************************/

// Destructor.

extern void gset_free(struct gset_set *set);

/*******************************************************************************************/

#endif /* GENERICSET_H */
