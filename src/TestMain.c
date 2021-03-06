/**
 * Test program for GenericMap.
 */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GenericMap.h"
#include "IntMap.h"
#include "StrMap.h"
#include "GenericList.h"
#include "GenericSet.h"

void print_gmap_keyvalue(struct gvalue_value key, struct gvalue_value value) {
	printf("{");
	gvalue.print(key);
	printf("=");
	gvalue.print(value);
	printf("}");
}

// Note: strdup is not standard C and it actually returns a bad ref when I used the default one in cygwin gcc!
char *my_strdup(const char *s) {
	size_t len = strlen(s) + 1;
	char *t = malloc(len);
	if (t != NULL) {
		memcpy(t, s, len);
	}
	return t;
}

void test_classIsComplete(void *classPointer, void *lastPointer) {
	assert(classPointer != NULL);
	assert(lastPointer != NULL);
	while (classPointer != lastPointer) {
		assert(*((void **)classPointer) != NULL);
		classPointer = (&classPointer)[1];
	}
}

void test_gmap_class_complete(void) {
	test_classIsComplete(&gmap, &(gmap.free));
}

void test_gmap_ordered(bool maintainInsertionOrder) {
	printf("Start test_gmap_ordered %s\n", maintainInsertionOrder ? "true" : "false");

	struct gmap_config config1 = {
			.keyType = gvalue.intType,
			.maintainInsertionOrder = maintainInsertionOrder
	};
	struct gmap_map *map = gmap.create1(config1);

	char *s1 = "hi";
	char *s2 = "ha";
	char *s3 = "ho";

	gmap.put(map, gvalue.getInt(1), gvalue.getString(s1));
	gmap.put(map, gvalue.getInt(2), gvalue.getString(s2));
	gmap.remove(map, gvalue.getInt(3));
	gmap.put(map, gvalue.getInt(4), gvalue.getString(s3));
	gmap.put(map, gvalue.getInt(4), gvalue.getString(s1));
	gmap.put(map, gvalue.getInt(5), gvalue.getString(s2));

	printf("(Ignore this error) ");
	struct gvalue_value *value = gmap.get(map, gvalue.getString(s1));
	assert(value == NULL);

	value = gmap.get(map, gvalue.getInt(4));
	assert(value->primitive.stringValue == s1);

	gmap.remove(map, gvalue.getInt(2));

	value = gmap.get(map, gvalue.getInt(2));
	assert(value == NULL);

	struct gvalue_value val = gmap.getOrDefault(map, gvalue.getInt(2), gvalue.getString(""));
	assert(strlen(val.primitive.stringValue) == 0);

	value = gmap.get(map, gvalue.getInt(1));
	assert(value != NULL);

	printf("Value: ");
	gvalue.dump(*value);
	puts("");

	gmap.remove(map, gvalue.getInt(1));
	assert(gmap.get(map, gvalue.getInt(1)) == NULL);

	printf("Print map: ");
	gmap.print(map);
	puts("");

	gmap.clear(map);

	for (int i = 0; i < 1000; i++) {
		assert(map->size == i);
		gmap.put(map, gvalue.getInt(i), gvalue.getString("A"));
		assert(gmap.containsKey(map, gvalue.getInt(i)) == true);
	}

	printf("Hash deviation: %f\n", gmap.hashDeviation(map));

	for (int i = 0; i < 1000; i += 2) {
		int size = map->size;
		gmap.remove(map, gvalue.getInt(i));
		assert(gmap.containsKey(map, gvalue.getInt(i)) == false);
		assert(map->size == size - 1);
	}

	gmap.free(map);

	struct gmap_config config2 = {
			.keyType = gvalue.intType,
			.restrictValueToType = gvalue.intType,
			.maintainInsertionOrder = maintainInsertionOrder
	};

	map = gmap.create1(config2);

	struct gmap_iterator iter1 = gmap.iterator(map);
	assert(gmap.next(&iter1) == false);

	struct gmap_keyvalue_list kvlist1 = gmap.getKeyValueList(map);
	assert(kvlist1.size == 0);
	gmap.freeKeyValueList(kvlist1);

	printf("Capacity: ");
	for (int i = 0; i < 10; i++) {
		gmap.put(map, gvalue.getInt(i * 23), gvalue.getInt(i * 12));
		printf("{%u=%u=%f} ", map->size, map->config.capacity, gmap.hashDeviation(map));
	}
	puts("");

	printf("Map1: ");
	gmap.print(map);
	puts("");

	struct gmap_iterator iter2 = gmap.iterator(map);
	int32_t lastKey = -1;
	while (gmap.next(&iter2)) {
		if (maintainInsertionOrder) {
			assert(iter2.key.primitive.intValue > lastKey);
			lastKey = iter2.key.primitive.intValue;
		}
	}

	printf("Map each: ");
	gmap.each(map, print_gmap_keyvalue);
	puts("");

	struct gmap_keyvalue_list kvlist2 = gmap.getKeyValueList(map);

	lastKey = -1;
	for (size_t i = 0; i < kvlist2.size; i++) {
		struct gmap_keyvalue pair = kvlist2.keyValuePairs[i];
		if (maintainInsertionOrder) {
			assert(pair.key.primitive.intValue > lastKey);
			lastKey = pair.key.primitive.intValue;
		}
		gmap.remove(map, pair.key);
	}
	gmap.freeKeyValueList(kvlist2);

	assert(map->size == 0);

	gmap.free(map);

	struct gmap_config config3 = {
			.keyType = gvalue.stringType,
			.restrictValueToType = gvalue.stringType,
			.maintainInsertionOrder = maintainInsertionOrder
	};

	map = gmap.create1(config3);

	gmap.put1(map, gvalue.getString(my_strdup("k1")), gvalue.getString(my_strdup("v1")), true, true);
	gmap.put1(map, gvalue.getString(my_strdup("k2")), gvalue.getString(my_strdup("v2")), true, true);
	gmap.remove(map, gvalue.getString("k1"));
	gmap.remove(map, gvalue.getString("k2"));
	gmap.free(map);

	printf("Done test_gmap_ordered %s\n\n", maintainInsertionOrder ? "true" : "false");
}

void test_gmap(void) {
	test_gmap_class_complete();
	test_gmap_ordered(false);
	test_gmap_ordered(true);
}

void print_intmap_keyvalue(int32_t key, int32_t value) {
	printf("[%i=%i]", key, value);
}

void test_intmap_class_complete(void) {
	test_classIsComplete(&intmap, &(intmap.free));
}

void test_intmap(void) {
	puts("Start test_intmap");

	test_intmap_class_complete();

	struct intmap_map *map = intmap.create();
	intmap.put(map, 11, 15);
	intmap.put(map, 22, 44);
	intmap.put(map, 33, 67);
	intmap.put(map, 55, 88);

	assert(map->gmap.size == 4);

	assert(intmap.get(map, 11) == 15);
	assert(intmap.get(map, 13) == 0);
	assert(intmap.get(map, 55) == 88);
	assert(intmap.getOrDefault(map, 777, -100) == -100);

	printf("Iterate: ");
	struct intmap_iterator iter = intmap.iterator(map);
	while (intmap.next(&iter) == true) {
		printf("{%i=%i}", iter.key, iter.value);
	}
	puts("");

	printf("KV list: ");
	struct intmap_keyvalue_list kvlist = intmap.getKeyValueList(map);
	for (size_t i = 0; i < kvlist.size; i++) {
		printf("(%i=%i)", kvlist.keyValuePairs[i].key, kvlist.keyValuePairs[i].value);
	}
	intmap.freeKeyValueList(kvlist);
	puts("");

	printf("Each: ");
	intmap.each(map, print_intmap_keyvalue);
	puts("");

	assert(intmap.containsKey(map, 22) == true);
	intmap.remove(map, 22);
	assert(intmap.containsKey(map, 22) == false);
	intmap.remove(map, 999);

	intmap.free(map);

	puts("Done test_intmap\n");
}

void print_strmap_keyvalue(char *key, char *value) {
	printf("[%s=%s]", key, value);
}

void test_strmap_class_complete(void) {
	test_classIsComplete(&strmap, &(strmap.free));
}

void test_strmap(void) {
	puts("Start test_strmap");

	test_strmap_class_complete();

	struct strmap_map *map = strmap.create();
	strmap.put(map, "aa", "ab");
	strmap.put(map, "bb", "de");
	strmap.put(map, "cc", "fg");
	strmap.put(map, "ee", "hh");

	assert(map->gmap.size == 4);

	assert(strcmp(strmap.get(map, "aa"), "ab") == 0);
	assert(strmap.get(map, "zz") == NULL);
	assert(strcmp(strmap.get(map, "ee"), "hh") == 0);
	assert(strcmp(strmap.getOrDefault(map, "ggg", "-zzz"), "-zzz") == 0);

	printf("Iterate: ");
	struct strmap_iterator iter = strmap.iterator(map);
	while (strmap.next(&iter) == true) {
		printf("{%s=%s}", iter.key, iter.value);
	}
	puts("");

	printf("KV list: ");
	struct strmap_keyvalue_list kvlist = strmap.getKeyValueList(map);
	for (size_t i = 0; i < kvlist.size; i++) {
		printf("(%s=%s)", kvlist.keyValuePairs[i].key, kvlist.keyValuePairs[i].value);
	}
	strmap.freeKeyValueList(kvlist);
	puts("");

	printf("Each: ");
	strmap.each(map, print_strmap_keyvalue);
	puts("");

	assert(strmap.containsKey(map, "bb") == true);
	strmap.remove(map, "bb");
	assert(strmap.containsKey(map, "bb") == false);
	strmap.remove(map, "zzz");

	strmap.put1(map, my_strdup("FreeMe"), my_strdup("MeToo"), true, true);

	strmap.free(map);

	puts("Done test_strmap\n");
}

void test_glist_print(struct gvalue_value v) {
	printf("(%" PRIi32 ")", v.primitive.intValue);
}

void test_glist_class_complete(void) {
	test_classIsComplete(&glist, &(glist.free));
}

void test_glist(void) {
	puts("Start test_glist");

	test_glist_class_complete();

	struct glist_list *list = glist.create(gvalue.intType);

	assert(glist.contains(list, gvalue.getInt(2)) == false);
	glist.add(list, gvalue.getInt(4));
	assert(glist.contains(list, gvalue.getInt(4)) == true);

	for (uint32_t i = 1; i < 100; i++) {
		assert(list->size == i);
		glist.add(list, gvalue.getInt(i * 3));
	}

	printf("glist: ");
	glist.print(list);
	puts("");

	uint32_t index;
	assert(glist.tryGetIndex(list, gvalue.getInt(3), &index) == true);
	assert(index == 1);

	assert(glist.contains(list, gvalue.getInt(42)) == true);

	uint32_t size = list->size;
	glist.remove(list, gvalue.getInt(30));
	assert(list->size == size - 1);

	while (list->size > 2) {
		glist.removeIndex(list, 1);
	}
	assert(list->size == 2);

	printf("each: ");
	glist.each(list, test_glist_print);
	puts("");

	glist.clear(list);
	assert(list->size == 0);

	glist.free(list);

	puts("Done test_glist\n");
}

void test_gset_print(struct gvalue_value v) {
	printf("(%" PRIi32 ")", v.primitive.intValue);
}

void test_gset_class_complete(void) {
	test_classIsComplete(&gset, &(gset.free));
}

void test_gset(void) {
	puts("Start test_gset");

	test_gset_class_complete();

	struct gset_set *set = gset.create(gvalue.intType);

	assert(gset.contains(set, gvalue.getInt(2)) == false);
	gset.put(set, gvalue.getInt(4));
	assert(gset.contains(set, gvalue.getInt(4)) == true);

	for (uint32_t i = 1; i < 100; i++) {
		assert(set->size == i);
		gset.put(set, gvalue.getInt(i * 3));
		gset.put(set, gvalue.getInt(i * 3));
	}

	printf("gset: ");
	gset.print(set);
	puts("");

	assert(gset.contains(set, gvalue.getInt(42)) == true);

	uint32_t size = set->size;
	gset.remove(set, gvalue.getInt(30));
	assert(set->size == size - 1);

	for (uint32_t i = 3; i < 100; i++) {
		gset.remove(set, gvalue.getInt(i * 3));
	}
	assert(set->size == 3);

	printf("each: ");
	gset.each(set, test_gset_print);
	puts("");

	printf("value list: ");
	struct gset_value_list list = gset.getValueList(set);
	for (uint32_t i = 0; i < list.size; i++) {
		printf("[%" PRIi32 "]", list.values[i].primitive.intValue);
	}
	gset.freeValueList(list);
	puts("");

	gset.clear(set);
	assert(set->size == 0);

	gset.free(set);

	puts("Done test_gset\n");
}

int main(void) {
	test_gmap();
	test_intmap();
	test_strmap();
	test_glist();
	test_gset();
	return EXIT_SUCCESS;
}
