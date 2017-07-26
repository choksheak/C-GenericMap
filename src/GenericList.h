#ifndef GENERICLIST_H
#define GENERICLIST_H

#include "GenericValue.h"

/*******************************************************************************************/

// Constants.

#define GLIST_DEFAULT_INITIAL_CAPACITY 		4

/*******************************************************************************************/

// Data types.

// For use in the constructor, like in the Builder pattern.
// Only dataType is required. The rest are optional.
struct glist_config {
	const struct gvalue_type *dataType;
	uint32_t capacity;
	int (*cmpFunc)(struct gvalue_value, struct gvalue_value);
	bool (*freeFunc)(struct gvalue_value);
};

struct glist_node {
	struct gvalue_value value;
	bool freeOnRemove;
};

struct glist_list {
	uint32_t size;
	struct glist_config config;
	struct glist_node *nodes;
};

// Pseudo class.
struct glist_class {

	// Constructors.
	struct glist_list *(*create)(const struct gvalue_type *dataType);
	struct glist_list *(*create1)(struct glist_config config);

	// Basic operations.
	void (*add)(struct glist_list *list, struct gvalue_value value);
	void (*add1)(struct glist_list *list, struct gvalue_value value, bool freeOnRemove);
	struct gvalue_value *(*get)(struct glist_list *list, uint32_t index);
	bool (*tryGetIndex)(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex);
	bool (*tryGetLastIndex)(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex);
	bool (*contains)(struct glist_list *list, struct gvalue_value value);
	bool (*removeIndex)(struct glist_list *list, uint32_t index);
	bool (*remove)(struct glist_list *list, struct gvalue_value value);
	void (*clear)(struct glist_list *list);

	// More operations.
	void (*each)(struct glist_list *list, void (*func)(struct gvalue_value));
	void (*print)(struct glist_list* list);
	void (*fprint)(struct glist_list* list, FILE *stream);
	struct glist_list (*getSlice)(struct glist_list *list, uint32_t fromIndex, uint32_t toIndex);

	// Destructor.
	void (*free)(struct glist_list *list);

};

// OOP class object.
extern struct glist_class glist;

/*******************************************************************************************/

// Constructors.

extern struct glist_list *glist_create(const struct gvalue_type *dataType);
extern struct glist_list *glist_create1(struct glist_config config);

/*******************************************************************************************/

// Basic operations.

extern void glist_add(struct glist_list *list, struct gvalue_value value);
extern void glist_add1(struct glist_list *list, struct gvalue_value value, bool freeOnRemove);
extern struct gvalue_value *glist_get(struct glist_list *list, uint32_t index);
extern bool glist_tryGetIndex(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex);
extern bool glist_tryGetLastIndex(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex);
extern bool glist_contains(struct glist_list *list, struct gvalue_value value);
extern bool glist_removeIndex(struct glist_list *list, uint32_t index);
extern bool glist_remove(struct glist_list *list, struct gvalue_value value);
extern void glist_clear(struct glist_list *list);

/*******************************************************************************************/

// More operations.

extern void glist_each(struct glist_list *list, void (*func)(struct gvalue_value));
extern void glist_print(struct glist_list* list);
extern void glist_fprint(struct glist_list* list, FILE *stream);
extern struct glist_list glist_getSlice(struct glist_list *list, uint32_t fromIndex, uint32_t toIndex);

/*******************************************************************************************/

// Destructor.

extern void glist_free(struct glist_list *list);

/*******************************************************************************************/

#endif /* GENERICLIST_H */
