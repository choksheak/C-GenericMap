/**
 * Implementation of a generic value array list.
 *
 * The nodes must all be of the same data type.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GenericList.h"

// OOP class object.
struct glist_class glist = {

		// Constructors.
		.create = glist_create,
		.create1 = glist_create1,

		// Basic operations.
		.add = glist_add,
		.add1 = glist_add1,
		.get = glist_get,
		.tryGetIndex = glist_tryGetIndex,
		.tryGetLastIndex = glist_tryGetLastIndex,
		.contains = glist_contains,
		.removeIndex = glist_removeIndex,
		.remove = glist_remove,
		.clear = glist_clear,

		// More operations.
		.each = glist_each,
		.print = glist_print,
		.fprint = glist_fprint,
		.getSlice = glist_getSlice,

		// Destructor.
		.free = glist_free

};

/*******************************************************************************************/

// Constructors.

struct glist_list *glist_create(const struct gvalue_type *dataType) {
	struct glist_config config = { .dataType = dataType };
	return glist_create1(config);
}

struct glist_list *glist_create1(struct glist_config config) {
	if (config.dataType == NULL) {
		printf("Error: glist: dataType is required\n");
		return NULL;
	}

	// Minimum capacity of 1 is to make sure we don't use calloc with a size of 0.
	// Otherwise we can allow for a minimum capacity of 0.
	if (config.capacity < 1) {
		config.capacity = 1;
	}

	if (config.cmpFunc == NULL) {
		config.cmpFunc = gvalue_cmp;
	}

	if (config.freeFunc == NULL) {
		config.freeFunc = gvalue_free;
	}

	struct glist_list *list = (struct glist_list *) malloc(sizeof(struct glist_list));

	list->config = config;
	list->size = 0;
	list->nodes = calloc(sizeof(struct glist_node), config.capacity);

	return list;
}

/*******************************************************************************************/

// Basic operations.

void private_glist_grow(struct glist_list *list) {
	uint32_t oldCapacity = list->config.capacity;
	uint32_t newCapacity = oldCapacity * 2;

	list->nodes = realloc(list->nodes, sizeof(struct glist_node) * newCapacity);
	list->config.capacity = newCapacity;
}

bool private_glist_checkDataType(struct gvalue_value givenValue, const struct gvalue_type *dataType) {
	if (givenValue.type != dataType) {
		printf("Error: glist: Wrong data type. Expected=%s, Actual=%s\n", dataType->name, givenValue.type->name);
		return false;
	}
	return true;
}

bool private_glist_checkBounds(uint32_t size, uint32_t index) {
	if (index >= size) {
		printf("Error: glist: Index out of bounds. Given=%" PRIu32 ", Size=%" PRIu32 "\n", index, size);
		return false;
	}
	return true;
}

void glist_add(struct glist_list *list, struct gvalue_value value) {
	glist_add1(list, value, false);
}

void glist_add1(struct glist_list *list, struct gvalue_value value, bool freeOnRemove) {
	if (private_glist_checkDataType(value, list->config.dataType) == false) {
		return;
	}

	if (list->size == list->config.capacity) {
		private_glist_grow(list);
	}

	list->nodes[list->size].value = value;
	list->nodes[list->size].freeOnRemove = freeOnRemove;
	list->size++;
}

struct gvalue_value *glist_get(struct glist_list *list, uint32_t index) {
	if (private_glist_checkBounds(list->size, index) == false) {
		return NULL;
	}

	return &(list->nodes[index].value);
}

bool glist_tryGetIndex(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex) {
	if (list->size == 0) {
		return false;
	}

	for (uint32_t i = 0; i < list->size; i++) {
		if (list->config.cmpFunc(value, list->nodes[i].value) == 0) {
			*outIndex = i;
			return true;
		}
	}

	return false;
}

bool glist_tryGetLastIndex(struct glist_list *list, struct gvalue_value value, uint32_t *outIndex) {
	if (list->size == 0) {
		return false;
	}

	uint32_t i = list->size;
	do {
		i--;
		if (list->config.cmpFunc(value, list->nodes[i].value) == 0) {
			*outIndex = i;
			return true;
		}
	}
	while (i > 0);

	return false;
}

bool glist_contains(struct glist_list *list, struct gvalue_value value) {
	uint32_t i;
	return glist_tryGetIndex(list, value, &i);
}

bool glist_removeIndex(struct glist_list *list, uint32_t index) {
	if (private_glist_checkBounds(list->size, index) == false) {
		return false;
	}

	if (list->nodes[index].freeOnRemove) {
		list->config.freeFunc(list->nodes[index].value);
	}

	for (uint32_t i = index; i < list->size - 1; i++) {
		list->nodes[i] = list->nodes[i + 1];
	}

	list->size--;
	return true;
}

bool glist_remove(struct glist_list *list, struct gvalue_value value) {
	if (list->size == 0) {
		return false;
	}

	for (uint32_t i = 0; i < list->size; i++) {
		if (list->config.cmpFunc(value, list->nodes[i].value) == 0) {
			return glist_removeIndex(list, i);
		}
	}

	return false;
}

void glist_clear(struct glist_list *list) {
	for (uint32_t i = 0; i < list->size; i++) {
		if (list->nodes[i].freeOnRemove) {
			list->config.freeFunc(list->nodes[i].value);
		}
	}

	list->size = 0;
}

/*******************************************************************************************/

// More operations.

void glist_each(struct glist_list *list, void (*func)(struct gvalue_value)) {
	for (uint32_t i = 0; i < list->size; i++) {
		func(list->nodes[i].value);
	}
}

void glist_print(struct glist_list* list) {
	glist_fprint(list, stdout);
}

void glist_fprint(struct glist_list* list, FILE *stream) {
	fputs("[ ", stream);
	for (uint32_t i = 0; i < list->size; i++) {
		if (i > 0) {
			fputs(", ", stream);
		}
		gvalue_fprint(list->nodes[i].value, stream);
	}
	fputs(" ]", stream);
}

// Returned list points to the same memory location as the original list.
// If original list changes, then don't use the returned list anymore.
// We will not add any additional checks for misuse of returned slices.
struct glist_list glist_getSlice(struct glist_list *list, uint32_t fromIndex, uint32_t toIndex) {
	struct glist_list slice = *list;

	if (fromIndex > toIndex) {
		slice.size = 0;
	}
	else if ((private_glist_checkBounds(list->size, fromIndex) == true)
			&& (private_glist_checkBounds(list->size, toIndex) == true)) {
		slice.nodes = &(slice.nodes[fromIndex]);
		slice.size = toIndex - fromIndex + 1;
	}

	return slice;
}

/*******************************************************************************************/

// Destructor.

void glist_free(struct glist_list *list) {
	glist_clear(list);
	free(list->nodes);
	free(list);
}
