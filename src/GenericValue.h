#pragma once

#ifndef GENERIC_VALUE_H
#define GENERIC_VALUE_H

#include <stdbool.h>
#include <float.h>
#include <stdint.h>
#include <wchar.h>
//#include <uchar.h> 	// Should include char16_t and char32_t if your compiler supports it.

#define GVALUE_BUFFER_SIZE 100

/*******************************************************************************************/

// Data types.

enum gvalue_type_codes {
	GVALUE_TYPE_BOOL,
	GVALUE_TYPE_BYTE,
	GVALUE_TYPE_SHORT,
	GVALUE_TYPE_INT,
	GVALUE_TYPE_LONG,

	GVALUE_TYPE_UBYTE,
	GVALUE_TYPE_USHORT,
	GVALUE_TYPE_UINT,
	GVALUE_TYPE_ULONG,

	GVALUE_TYPE_FLOAT,
	GVALUE_TYPE_DOUBLE,

	GVALUE_TYPE_POINTER,

	GVALUE_TYPE_CHAR,
	GVALUE_TYPE_WCHAR,
	GVALUE_TYPE_STRING,
	GVALUE_TYPE_WSTRING,

	// Stores the total number of types. This is not a data type.
	GVALUE_TYPES_COUNT
};

// Strongly-typed enum.
struct gvalue_type {
	int code;
	char *name;
	char *cTypeName;
};

union gvalue_primitive {
	bool boolValue;
	int8_t byteValue;
	int16_t shortValue;
	int32_t intValue;
	int64_t longValue;

	uint8_t ubyteValue;
	uint16_t ushortValue;
	uint32_t uintValue;
	uint64_t ulongValue;

	float floatValue;
	double doubleValue;

	void *pointerValue;

	char charValue;
	wchar_t wcharValue;
	char *stringValue;
	wchar_t *wstringValue;
};

struct gvalue_value {
	const struct gvalue_type *type;
	union gvalue_primitive primitive;
};

// Pseudo class.
struct gvalue_class {

	// Type constants.
	const struct gvalue_type *boolType;
	const struct gvalue_type *byteType;
	const struct gvalue_type *shortType;
	const struct gvalue_type *intType;
	const struct gvalue_type *longType;

	const struct gvalue_type *ubyteType;
	const struct gvalue_type *ushortType;
	const struct gvalue_type *uintType;
	const struct gvalue_type *ulongType;

	const struct gvalue_type *floatType;
	const struct gvalue_type *doubleType;

	const struct gvalue_type *pointerType;

	const struct gvalue_type *charType;
	const struct gvalue_type *wcharType;
	const struct gvalue_type *stringType;
	const struct gvalue_type *wstringType;

	// Primitives.
	union gvalue_primitive (*getBoolPrimitive)(bool boolValue);
	union gvalue_primitive (*getBytePrimitive)(int8_t byteValue);
	union gvalue_primitive (*getShortPrimitive)(int16_t shortValue);
	union gvalue_primitive (*getIntPrimitive)(int32_t intValue);
	union gvalue_primitive (*getLongPrimitive)(int64_t outLongValue);
	union gvalue_primitive (*getUBytePrimitive)(uint8_t ubyteValue);
	union gvalue_primitive (*getUShortPrimitive)(uint16_t ushortValue);
	union gvalue_primitive (*getUIntPrimitive)(uint32_t uintValue);
	union gvalue_primitive (*getULongPrimitive)(uint64_t ulongValue);
	union gvalue_primitive (*getFloatPrimitive)(float floatValue);
	union gvalue_primitive (*getDoublePrimitive)(double outDoubleValue);
	union gvalue_primitive (*getPointerPrimitive)(void *pointerValue);
	union gvalue_primitive (*getCharPrimitive)(char charValue);
	union gvalue_primitive (*getWCharPrimitive)(wchar_t wcharValue);
	union gvalue_primitive (*getStringPrimitive)(char *stringValue);
	union gvalue_primitive (*getWStringPrimitive)(wchar_t *wstringValue);

	// GenericValue.
	struct gvalue_value (*getBool)(bool boolValue);
	struct gvalue_value (*getByte)(int8_t byteValue);
	struct gvalue_value (*getShort)(int16_t shortValue);
	struct gvalue_value (*getInt)(int32_t intValue);
	struct gvalue_value (*getLong)(int64_t outLongValue);
	struct gvalue_value (*getUByte)(uint8_t ubyteValue);
	struct gvalue_value (*getUShort)(uint16_t ushortValue);
	struct gvalue_value (*getUInt)(uint32_t uintValue);
	struct gvalue_value (*getULong)(uint64_t ulongValue);
	struct gvalue_value (*getFloat)(float floatValue);
	struct gvalue_value (*getDouble)(double outDoubleValue);
	struct gvalue_value (*getPointer)(void *pointerValue);
	struct gvalue_value (*getChar)(char charValue);
	struct gvalue_value (*getWChar)(wchar_t wcharValue);
	struct gvalue_value (*getString)(char *stringValue);
	struct gvalue_value (*getWString)(wchar_t *wstringValue);

	// Helpers.
	void (*print)(struct gvalue_value primitive);
	void (*fprint)(struct gvalue_value primitive, FILE *stream);
	void (*dump)(struct gvalue_value primitive);
	void (*fdump)(struct gvalue_value primitive, FILE *stream);
	bool (*tryGetLong)(struct gvalue_value primitive, int64_t *outLongValue);
	int64_t (*longValue)(struct gvalue_value primitive);
	bool (*tryGetDouble)(struct gvalue_value primitive, double *outDoubleValue);
	double (*doubleValue)(struct gvalue_value primitive);
	char *(*getAllocStringValue)(struct gvalue_value primitive);
	wchar_t *(*getAllocWStringValue)(struct gvalue_value primitive);
	bool (*free)(struct gvalue_value primitive);
	int (*cmp)(struct gvalue_value value1, struct gvalue_value value2);

	// Hash code.
	uint32_t (*hashInt)(uint32_t i);
	uint32_t (*hashDouble)(double d);
	uint32_t (*hashString)(char *string);
	uint32_t (*hashWString)(wchar_t *wstring);
	uint32_t (*hash)(struct gvalue_value primitive);

};

// OOP class object.
extern struct gvalue_class gvalue;

/*******************************************************************************************/

// GenericPrimitive constructors.

extern union gvalue_primitive gvalue_getBoolPrimitive(bool boolValue);
extern union gvalue_primitive gvalue_getBytePrimitive(int8_t byteValue);
extern union gvalue_primitive gvalue_getShortPrimitive(int16_t shortValue);
extern union gvalue_primitive gvalue_getIntPrimitive(int32_t intValue);
extern union gvalue_primitive gvalue_getLongPrimitive(int64_t outLongValue);
extern union gvalue_primitive gvalue_getUBytePrimitive(uint8_t ubyteValue);
extern union gvalue_primitive gvalue_getUShortPrimitive(uint16_t ushortValue);
extern union gvalue_primitive gvalue_getUIntPrimitive(uint32_t uintValue);
extern union gvalue_primitive gvalue_getULongPrimitive(uint64_t ulongValue);
extern union gvalue_primitive gvalue_getFloatPrimitive(float floatValue);
extern union gvalue_primitive gvalue_getDoublePrimitive(double outDoubleValue);
extern union gvalue_primitive gvalue_getPointerPrimitive(void *pointerValue);
extern union gvalue_primitive gvalue_getCharPrimitive(char charValue);
extern union gvalue_primitive gvalue_getWCharPrimitive(wchar_t wcharValue);
extern union gvalue_primitive gvalue_getStringPrimitive(char *stringValue);
extern union gvalue_primitive gvalue_getWStringPrimitive(wchar_t *wstringValue);

/*******************************************************************************************/

// GenericValue constructors.

extern struct gvalue_value gvalue_getBool(bool boolValue);
extern struct gvalue_value gvalue_getByte(int8_t byteValue);
extern struct gvalue_value gvalue_getShort(int16_t shortValue);
extern struct gvalue_value gvalue_getInt(int32_t intValue);
extern struct gvalue_value gvalue_getLong(int64_t outLongValue);
extern struct gvalue_value gvalue_getUByte(uint8_t ubyteValue);
extern struct gvalue_value gvalue_getUShort(uint16_t ushortValue);
extern struct gvalue_value gvalue_getUInt(uint32_t uintValue);
extern struct gvalue_value gvalue_getULong(uint64_t ulongValue);
extern struct gvalue_value gvalue_getFloat(float floatValue);
extern struct gvalue_value gvalue_getDouble(double outDoubleValue);
extern struct gvalue_value gvalue_getPointer(void *pointerValue);
extern struct gvalue_value gvalue_getChar(char charValue);
extern struct gvalue_value gvalue_getWChar(wchar_t wcharValue);
extern struct gvalue_value gvalue_getString(char *stringValue);
extern struct gvalue_value gvalue_getWString(wchar_t *wstringValue);

/*******************************************************************************************/

// Convenient helpers.

extern void gvalue_print(struct gvalue_value value);
extern void gvalue_fprint(struct gvalue_value value, FILE *stream);
extern void gvalue_dump(struct gvalue_value value);
extern void gvalue_fdump(struct gvalue_value value, FILE *stream);
extern bool gvalue_tryGetLong(struct gvalue_value value, int64_t *outLongValue);
extern int64_t gvalue_longValue(struct gvalue_value value);
extern bool gvalue_tryGetDouble(struct gvalue_value value, double *outDoubleValue);
extern double gvalue_doubleValue(struct gvalue_value value);
extern char *gvalue_getAllocStringValue(struct gvalue_value value);
extern wchar_t *gvalue_getAllocWStringValue(struct gvalue_value value);
extern bool gvalue_free(struct gvalue_value value);
extern int gvalue_cmp(struct gvalue_value value1, struct gvalue_value value2);

/*******************************************************************************************/

// Calculate hash code.

extern uint32_t gvalue_hashInt(uint32_t i);
extern uint32_t gvalue_hashLong(uint64_t i);
extern uint32_t gvalue_hashDouble(double d);
extern uint32_t gvalue_hashString(char *string);
extern uint32_t gvalue_hashWString(wchar_t *wstring);
extern uint32_t gvalue_hash(struct gvalue_value value);

/*******************************************************************************************/

#endif /* GENERIC_VALUE_H */
