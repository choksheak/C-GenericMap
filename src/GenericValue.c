/**
 * Implementation for a generic primitive.
 *
 * Such values generally fall into two different categories:
 *   1. non-pointer types (primitives)
 *   2. pointer types
 */

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "GenericValue.h"

/*******************************************************************************************/

// Global variables.

const struct gvalue_type private_bool_type = { .code = GVALUE_TYPE_BOOL, .name = "bool", .cTypeName = "bool" };
const struct gvalue_type private_byte_type = { .code = GVALUE_TYPE_BYTE, .name = "byte", .cTypeName = "int8_t" };
const struct gvalue_type private_short_type = { .code = GVALUE_TYPE_SHORT, .name = "short", .cTypeName = "int16_t" };
const struct gvalue_type private_int_type = { .code = GVALUE_TYPE_INT, .name = "int", .cTypeName = "int32_t" };
const struct gvalue_type private_long_type = { .code = GVALUE_TYPE_LONG, .name = "long", .cTypeName = "int64_t" };

const struct gvalue_type private_ubyte_type = { .code = GVALUE_TYPE_UBYTE, .name = "ubyte", .cTypeName = "uint8_t" };
const struct gvalue_type private_ushort_type = { .code = GVALUE_TYPE_USHORT, .name = "ushort", .cTypeName = "uint16_t" };
const struct gvalue_type private_uint_type = { .code = GVALUE_TYPE_UINT, .name = "uint", .cTypeName = "uint32_t" };
const struct gvalue_type private_ulong_type = { .code = GVALUE_TYPE_ULONG, .name = "ulong", .cTypeName = "uint64_t" };

const struct gvalue_type private_float_type = { .code = GVALUE_TYPE_FLOAT, .name = "float", .cTypeName = "float" };
const struct gvalue_type private_double_type = { .code = GVALUE_TYPE_DOUBLE, .name = "double", .cTypeName = "double" };

const struct gvalue_type private_pointer_type = { .code = GVALUE_TYPE_POINTER, .name = "pointer", .cTypeName = "void *" };

const struct gvalue_type private_char_type = { .code = GVALUE_TYPE_CHAR, .name = "char", .cTypeName = "char" };
const struct gvalue_type private_wchar_type = { .code = GVALUE_TYPE_WCHAR, .name = "wchar", .cTypeName = "wchar_t" };
const struct gvalue_type private_string_type = { .code = GVALUE_TYPE_STRING, .name = "string", .cTypeName = "char *" };
const struct gvalue_type private_wstring_type = { .code = GVALUE_TYPE_WSTRING, .name = "wstring", .cTypeName = "wchar_t *" };

// OOP class object.
struct gvalue_class gvalue = {

		// Type constants.
		.boolType = &private_bool_type,
		.byteType = &private_byte_type,
		.shortType = &private_short_type,
		.intType = &private_int_type,
		.longType = &private_long_type,

		.ubyteType = &private_ubyte_type,
		.ushortType = &private_ushort_type,
		.uintType = &private_uint_type,
		.ulongType = &private_ulong_type,

		.floatType = &private_float_type,
		.doubleType = &private_double_type,

		.pointerType = &private_pointer_type,

		.charType = &private_char_type,
		.wcharType = &private_wchar_type,
		.stringType = &private_string_type,
		.wstringType = &private_wstring_type,

		// Primitives.
		.getBoolPrimitive = gvalue_getBoolPrimitive,
		.getBytePrimitive = gvalue_getBytePrimitive,
		.getShortPrimitive = gvalue_getShortPrimitive,
		.getIntPrimitive = gvalue_getIntPrimitive,
		.getLongPrimitive = gvalue_getLongPrimitive,
		.getUBytePrimitive = gvalue_getUBytePrimitive,
		.getUShortPrimitive = gvalue_getUShortPrimitive,
		.getUIntPrimitive = gvalue_getUIntPrimitive,
		.getULongPrimitive = gvalue_getULongPrimitive,
		.getFloatPrimitive = gvalue_getFloatPrimitive,
		.getDoublePrimitive = gvalue_getDoublePrimitive,
		.getPointerPrimitive = gvalue_getPointerPrimitive,
		.getCharPrimitive = gvalue_getCharPrimitive,
		.getWCharPrimitive = gvalue_getWCharPrimitive,
		.getStringPrimitive = gvalue_getStringPrimitive,
		.getWStringPrimitive = gvalue_getWStringPrimitive,

		// GenericValue.
		.getBool = gvalue_getBool,
		.getByte = gvalue_getByte,
		.getShort = gvalue_getShort,
		.getInt = gvalue_getInt,
		.getLong = gvalue_getLong,
		.getUByte = gvalue_getUByte,
		.getUShort = gvalue_getUShort,
		.getUInt = gvalue_getUInt,
		.getULong = gvalue_getULong,
		.getFloat = gvalue_getFloat,
		.getDouble = gvalue_getDouble,
		.getPointer = gvalue_getPointer,
		.getChar = gvalue_getChar,
		.getWChar = gvalue_getWChar,
		.getString = gvalue_getString,
		.getWString = gvalue_getWString,

		// Helpers.
		.print = gvalue_print,
		.fprint = gvalue_fprint,
		.dump = gvalue_dump,
		.fdump = gvalue_fdump,
		.tryGetLong = gvalue_tryGetLong,
		.longValue = gvalue_longValue,
		.tryGetDouble = gvalue_tryGetDouble,
		.doubleValue = gvalue_doubleValue,
		.getAllocStringValue = gvalue_getAllocStringValue,
		.getAllocWStringValue = gvalue_getAllocWStringValue,
		.free = gvalue_free,
		.cmp = gvalue_cmp,

		// Hash code.
		.hashInt = gvalue_hashInt,
		.hashDouble = gvalue_hashDouble,
		.hashString = gvalue_hashString,
		.hashWString = gvalue_hashWString,
		.hash = gvalue_hash

};

/*******************************************************************************************/

// GenericPrimitive constructors.

union gvalue_primitive gvalue_getBoolPrimitive(bool boolValue) {
	union gvalue_primitive v = { .boolValue = boolValue };
	return v;
}

union gvalue_primitive gvalue_getBytePrimitive(int8_t byteValue) {
	union gvalue_primitive v = { .byteValue = byteValue };
	return v;
}

union gvalue_primitive gvalue_getShortPrimitive(int16_t shortValue) {
	union gvalue_primitive v = { .shortValue = shortValue };
	return v;
}

union gvalue_primitive gvalue_getIntPrimitive(int32_t intValue) {
	union gvalue_primitive v = { .intValue = intValue };
	return v;
}

union gvalue_primitive gvalue_getLongPrimitive(int64_t longValue) {
	union gvalue_primitive v = { .longValue = longValue };
	return v;
}

union gvalue_primitive gvalue_getUBytePrimitive(uint8_t ubyteValue) {
	union gvalue_primitive v = { .ubyteValue = ubyteValue };
	return v;
}

union gvalue_primitive gvalue_getUShortPrimitive(uint16_t ushortValue) {
	union gvalue_primitive v = { .ushortValue = ushortValue };
	return v;
}

union gvalue_primitive gvalue_getUIntPrimitive(uint32_t uintValue) {
	union gvalue_primitive v = { .uintValue = uintValue };
	return v;
}

union gvalue_primitive gvalue_getULongPrimitive(uint64_t ulongValue) {
	union gvalue_primitive v = { .ulongValue = ulongValue };
	return v;
}

union gvalue_primitive gvalue_getFloatPrimitive(float floatValue) {
	union gvalue_primitive v = { .floatValue = floatValue };
	return v;
}

union gvalue_primitive gvalue_getDoublePrimitive(double doubleValue) {
	union gvalue_primitive v = { .doubleValue = doubleValue };
	return v;
}

union gvalue_primitive gvalue_getPointerPrimitive(void *pointerValue) {
	union gvalue_primitive v = { .pointerValue = pointerValue };
	return v;
}

union gvalue_primitive gvalue_getCharPrimitive(char charValue) {
	union gvalue_primitive v = { .charValue = charValue };
	return v;
}

union gvalue_primitive gvalue_getWCharPrimitive(wchar_t wcharValue) {
	union gvalue_primitive v = { .wcharValue = wcharValue };
	return v;
}

union gvalue_primitive gvalue_getStringPrimitive(char *stringValue) {
	union gvalue_primitive v = { .stringValue = stringValue };
	return v;
}

union gvalue_primitive gvalue_getWStringPrimitive(wchar_t *wstringValue) {
	union gvalue_primitive v = { .wstringValue = wstringValue };
	return v;
}

/*******************************************************************************************/

// GenericValue constructors.

struct gvalue_value gvalue_getBool(bool boolValue) {
	struct gvalue_value v = { .type = &private_bool_type, .primitive = gvalue_getBoolPrimitive(boolValue) };
	return v;
}

struct gvalue_value gvalue_getByte(int8_t byteValue) {
	struct gvalue_value v = { .type = &private_byte_type, .primitive = gvalue_getBytePrimitive(byteValue) };
	return v;
}

struct gvalue_value gvalue_getShort(int16_t shortValue) {
	struct gvalue_value v = { .type = &private_short_type, .primitive = gvalue_getShortPrimitive(shortValue) };
	return v;
}

struct gvalue_value gvalue_getInt(int32_t intValue) {
	struct gvalue_value v = { .type = &private_int_type, .primitive = gvalue_getIntPrimitive(intValue) };
	return v;
}

struct gvalue_value gvalue_getLong(int64_t longValue) {
	struct gvalue_value v = { .type = &private_long_type, .primitive = gvalue_getLongPrimitive(longValue) };
	return v;
}

struct gvalue_value gvalue_getUByte(uint8_t ubyteValue) {
	struct gvalue_value v = { .type = &private_ubyte_type, .primitive = gvalue_getUBytePrimitive(ubyteValue) };
	return v;
}

struct gvalue_value gvalue_getUShort(uint16_t ushortValue) {
	struct gvalue_value v = { .type = &private_ushort_type, .primitive = gvalue_getUShortPrimitive(ushortValue) };
	return v;
}

struct gvalue_value gvalue_getUInt(uint32_t uintValue) {
	struct gvalue_value v = { .type = &private_uint_type, .primitive = gvalue_getUIntPrimitive(uintValue) };
	return v;
}

struct gvalue_value gvalue_getULong(uint64_t ulongValue) {
	struct gvalue_value v = { .type = &private_ulong_type, .primitive = gvalue_getULongPrimitive(ulongValue) };
	return v;
}

struct gvalue_value gvalue_getFloat(float floatValue) {
	struct gvalue_value v = { .type = &private_float_type, .primitive = gvalue_getFloatPrimitive(floatValue) };
	return v;
}

struct gvalue_value gvalue_getDouble(double doubleValue) {
	struct gvalue_value v = { .type = &private_double_type, .primitive = gvalue_getDoublePrimitive(doubleValue) };
	return v;
}

struct gvalue_value gvalue_getPointer(void *pointerValue) {
	struct gvalue_value v = { .type = &private_pointer_type, .primitive = gvalue_getPointerPrimitive(pointerValue) };
	return v;
}

struct gvalue_value gvalue_getChar(char charValue) {
	struct gvalue_value v = { .type = &private_char_type, .primitive = gvalue_getCharPrimitive(charValue) };
	return v;
}

struct gvalue_value gvalue_getWChar(wchar_t wcharValue) {
	struct gvalue_value v = { .type = &private_wchar_type, .primitive = gvalue_getWCharPrimitive(wcharValue) };
	return v;
}

struct gvalue_value gvalue_getString(char *stringValue) {
	struct gvalue_value v = { .type = &private_string_type, .primitive = gvalue_getStringPrimitive(stringValue) };
	return v;
}

struct gvalue_value gvalue_getWString(wchar_t *wstringValue) {
	struct gvalue_value v = { .type = &private_wstring_type, .primitive = gvalue_getWStringPrimitive(wstringValue) };
	return v;
}

/*******************************************************************************************/

// Convenient helpers.

void private_gvalue_unknownType(const struct gvalue_type *type, FILE *stream) {
	fprintf(stream, "Error: gvalue: Unknown type %i = %s", type->code, type->name);
}

void gvalue_print(struct gvalue_value value) {
	gvalue_fprint(value, stdout);
}

void gvalue_fprint(struct gvalue_value value, FILE *stream) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		fputs(value.primitive.boolValue == false ? "false" : "true", stream);
		break;
	case GVALUE_TYPE_BYTE:
		fprintf(stream, "%" PRIi8 , value.primitive.byteValue);
		break;
	case GVALUE_TYPE_SHORT:
		fprintf(stream, "%" PRIi16, value.primitive.shortValue);
		break;
	case GVALUE_TYPE_INT:
		fprintf(stream, "%" PRIi32, value.primitive.intValue);
		break;
	case GVALUE_TYPE_LONG:
		fprintf(stream, "%" PRIi64, value.primitive.longValue);
		break;

	case GVALUE_TYPE_UBYTE:
		fprintf(stream, "%" PRIu8, value.primitive.ubyteValue);
		break;
	case GVALUE_TYPE_USHORT:
		fprintf(stream, "%" PRIu16, value.primitive.ushortValue);
		break;
	case GVALUE_TYPE_UINT:
		fprintf(stream, "%" PRIu32, value.primitive.uintValue);
		break;
	case GVALUE_TYPE_ULONG:
		fprintf(stream, "%" PRIu64, value.primitive.ulongValue);
		break;

	case GVALUE_TYPE_FLOAT:
		fprintf(stream, "%f", value.primitive.floatValue);
		break;
	case GVALUE_TYPE_DOUBLE:
		fprintf(stream, "%lf", value.primitive.doubleValue);
		break;

	case GVALUE_TYPE_POINTER:
		fprintf(stream, "%p", value.primitive.pointerValue);
		break;

	case GVALUE_TYPE_CHAR:
		fputc(value.primitive.charValue, stream);
		break;
	case GVALUE_TYPE_WCHAR:
		fputwc(value.primitive.wcharValue, stream);
		break;
	case GVALUE_TYPE_STRING:
		fputs(value.primitive.stringValue, stream);
		break;
	case GVALUE_TYPE_WSTRING:
		fputws(value.primitive.wstringValue, stream);
		break;

	default:
		private_gvalue_unknownType(value.type, stream);
		break;
	}
}


void gvalue_dump(struct gvalue_value value) {
	gvalue_fdump(value, stdout);
}

void gvalue_fdump(struct gvalue_value value, FILE *stream) {
	fputs("{ (", stream);
	fputs(value.type->name, stream);
	fputs(") ", stream);
	gvalue_fprint(value, stream);
	fputs(" }", stream);
}

bool gvalue_tryGetLong(struct gvalue_value value, int64_t *outLongValue) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		*outLongValue = (int64_t) (value.primitive.boolValue);
		return true;
	case GVALUE_TYPE_BYTE:
		*outLongValue = (int64_t) (value.primitive.byteValue);
		return true;
	case GVALUE_TYPE_SHORT:
		*outLongValue = (int64_t) (value.primitive.shortValue);
		return true;
	case GVALUE_TYPE_INT:
		*outLongValue = (int64_t) (value.primitive.intValue);
		return true;
	case GVALUE_TYPE_LONG:
		*outLongValue = value.primitive.longValue;
		return true;

	case GVALUE_TYPE_UBYTE:
		*outLongValue = (int64_t) (value.primitive.ubyteValue);
		return true;
	case GVALUE_TYPE_USHORT:
		*outLongValue = (int64_t) (value.primitive.ushortValue);
		return true;
	case GVALUE_TYPE_UINT:
		*outLongValue = (int64_t) (value.primitive.uintValue);
		return true;
	case GVALUE_TYPE_ULONG:
		*outLongValue = (value.primitive.ulongValue > INT64_MAX) ? INT64_MAX : (long) (value.primitive.ulongValue);
		return (value.primitive.ulongValue <= INT64_MAX);

	case GVALUE_TYPE_FLOAT:
		*outLongValue = (int64_t) (value.primitive.floatValue);
		return ((double) *outLongValue) == floor(value.primitive.floatValue);
	case GVALUE_TYPE_DOUBLE:
		*outLongValue = (int64_t) (value.primitive.doubleValue);
		return ((double) *outLongValue) == floor(value.primitive.doubleValue);

	case GVALUE_TYPE_POINTER:
		*outLongValue = (int64_t) (value.primitive.pointerValue);
		return false;

	case GVALUE_TYPE_CHAR:
		*outLongValue = (int64_t) (value.primitive.charValue);
		return true;
	case GVALUE_TYPE_WCHAR:
		*outLongValue = (int64_t) (value.primitive.wcharValue);
		return true;
	case GVALUE_TYPE_STRING:
		if (value.primitive.stringValue == NULL) {
			*outLongValue = 0;
			return false;
		}

		*outLongValue = (int64_t) atol(value.primitive.stringValue);

		char buffer[21];
		sprintf(buffer, "%" PRIi64, *outLongValue);

		return strcmp(buffer, value.primitive.stringValue) == 0;
	case GVALUE_TYPE_WSTRING:
		if (value.primitive.wstringValue == NULL) {
			*outLongValue = 0;
			return false;
		}

		*outLongValue = (int64_t) wcstol(value.primitive.wstringValue, NULL, 10);

		wchar_t wbuffer[21];
		swprintf(wbuffer, 21, L"%" PRIi64, *outLongValue);

		return wcscmp(wbuffer, value.primitive.wstringValue) == 0;

	default:
		private_gvalue_unknownType(value.type, stdout);
		*outLongValue = 0;
		return false;
	}
}

int64_t gvalue_longValue(struct gvalue_value value) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		return (int64_t) (value.primitive.boolValue);
	case GVALUE_TYPE_BYTE:
		return (int64_t) (value.primitive.byteValue);
	case GVALUE_TYPE_SHORT:
		return (int64_t) (value.primitive.shortValue);
	case GVALUE_TYPE_INT:
		return (int64_t) (value.primitive.intValue);
	case GVALUE_TYPE_LONG:
		return value.primitive.longValue;

	case GVALUE_TYPE_UBYTE:
		return (int64_t) (value.primitive.ubyteValue);
	case GVALUE_TYPE_USHORT:
		return (int64_t) (value.primitive.ushortValue);
	case GVALUE_TYPE_UINT:
		return (int64_t) (value.primitive.uintValue);
	case GVALUE_TYPE_ULONG:
		return (value.primitive.ulongValue > INT64_MAX) ? INT64_MAX : (long) (value.primitive.ulongValue);

	case GVALUE_TYPE_FLOAT:
		return (int64_t) (value.primitive.floatValue);
	case GVALUE_TYPE_DOUBLE:
		return (int64_t) (value.primitive.doubleValue);

	case GVALUE_TYPE_POINTER:
		return (int64_t) (value.primitive.pointerValue);

	case GVALUE_TYPE_CHAR:
		return (int64_t) (value.primitive.charValue);
	case GVALUE_TYPE_WCHAR:
		return (int64_t) (value.primitive.wcharValue);
	case GVALUE_TYPE_STRING:
		return (int64_t) atol(value.primitive.stringValue);
	case GVALUE_TYPE_WSTRING:
		return (int64_t) wcstol(value.primitive.wstringValue, NULL, 10);

	default:
		private_gvalue_unknownType(value.type, stdout);
		return 0;
	}
}

bool gvalue_tryGetDouble(struct gvalue_value value, double *outDoubleValue) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		*outDoubleValue = (double) (value.primitive.boolValue);
		return true;
	case GVALUE_TYPE_BYTE:
		*outDoubleValue = (double) (value.primitive.byteValue);
		return true;
	case GVALUE_TYPE_SHORT:
		*outDoubleValue = (double) (value.primitive.shortValue);
		return true;
	case GVALUE_TYPE_INT:
		*outDoubleValue = (double) (value.primitive.intValue);
		return true;
	case GVALUE_TYPE_LONG:
		*outDoubleValue = value.primitive.longValue;
		return true;

	case GVALUE_TYPE_UBYTE:
		*outDoubleValue = (double) (value.primitive.ubyteValue);
		return true;
	case GVALUE_TYPE_USHORT:
		*outDoubleValue = (double) (value.primitive.ushortValue);
		return true;
	case GVALUE_TYPE_UINT:
		*outDoubleValue = (double) (value.primitive.uintValue);
		return true;
	case GVALUE_TYPE_ULONG:
		*outDoubleValue = (double) (value.primitive.ulongValue);
		return true;

	case GVALUE_TYPE_FLOAT:
		*outDoubleValue = (double) (value.primitive.floatValue);
		return true;
	case GVALUE_TYPE_DOUBLE:
		*outDoubleValue = value.primitive.doubleValue;
		return true;

	case GVALUE_TYPE_POINTER:
		*outDoubleValue = 0;
		return false;

	case GVALUE_TYPE_CHAR:
		*outDoubleValue = (double) (value.primitive.charValue);
		return true;
	case GVALUE_TYPE_WCHAR:
		*outDoubleValue = (double) (value.primitive.wcharValue);
		return true;
	case GVALUE_TYPE_STRING:
		if (value.primitive.stringValue == NULL) {
			*outDoubleValue = 0;
			return false;
		}

	    char *endPtr = 0;
	    *outDoubleValue = strtod(value.primitive.stringValue, &endPtr);

	    return (endPtr != value.primitive.stringValue) && (*endPtr == '\0');
	case GVALUE_TYPE_WSTRING:
		if (value.primitive.wstringValue == NULL) {
			*outDoubleValue = 0;
			return false;
		}

	    wchar_t *wideEndPtr = 0;
	    *outDoubleValue = wcstod(value.primitive.wstringValue, &wideEndPtr);

	    return (wideEndPtr != value.primitive.wstringValue) && (*wideEndPtr == '\0');

	default:
		private_gvalue_unknownType(value.type, stdout);
		*outDoubleValue = 0;
		return false;
	}
}

double gvalue_doubleValue(struct gvalue_value value) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		return (double) (value.primitive.boolValue);
	case GVALUE_TYPE_BYTE:
		return (double) (value.primitive.byteValue);
	case GVALUE_TYPE_SHORT:
		return (double) (value.primitive.shortValue);
	case GVALUE_TYPE_INT:
		return (double) (value.primitive.intValue);
	case GVALUE_TYPE_LONG:
		return value.primitive.longValue;

	case GVALUE_TYPE_UBYTE:
		return (double) (value.primitive.ubyteValue);
	case GVALUE_TYPE_USHORT:
		return (double) (value.primitive.ushortValue);
	case GVALUE_TYPE_UINT:
		return (double) (value.primitive.uintValue);
	case GVALUE_TYPE_ULONG:
		return (double) (value.primitive.ulongValue);

	case GVALUE_TYPE_FLOAT:
		return (double) (value.primitive.floatValue);
	case GVALUE_TYPE_DOUBLE:
		return value.primitive.doubleValue;

	case GVALUE_TYPE_POINTER:
		return 0;

	case GVALUE_TYPE_CHAR:
		return (double) (value.primitive.charValue);
	case GVALUE_TYPE_WCHAR:
		return (double) (value.primitive.wcharValue);
	case GVALUE_TYPE_STRING:
		return strtod(value.primitive.stringValue, NULL);
	case GVALUE_TYPE_WSTRING:
		return wcstod(value.primitive.wstringValue, NULL);

	default:
		private_gvalue_unknownType(value.type, stdout);
		return 0;
	}
}

char *private_allocStringToString(char *s) {
	size_t len = strlen(s);
	char *t = malloc(sizeof(char) * (len + 1));
	strcpy(t, s);
	return t;
}

// Converts a wide string into a regular string.
char *private_allocWStringToString(wchar_t *s) {
	size_t len = wcslen(s);
	char *t = malloc(sizeof(char) * (len + 1));
	while (*s != L'\0') {
		*t++ = (char) *s++;
	}
	return t;
}

wchar_t *private_allocStringToWString(char *s) {
	size_t len = strlen(s);
	wchar_t *t = malloc(sizeof(wchar_t) * (len + 1));
	while (*s != '\0') {
		*t++ = (wchar_t) *s++;
	}
	return t;
}

wchar_t *private_allocWStringToWString(wchar_t *s) {
	size_t len = wcslen(s);
	wchar_t *t = malloc(sizeof(wchar_t) * (len + 1));
	wcscpy(t, s);
	return t;
}

char *gvalue_getAllocStringValue(struct gvalue_value value) {
	char buffer[GVALUE_BUFFER_SIZE];

	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		return value.primitive.boolValue == false ? private_allocStringToString("false") : private_allocStringToString("true");
	case GVALUE_TYPE_BYTE:
		sprintf(buffer, "%" PRIi8, value.primitive.byteValue);
		break;
	case GVALUE_TYPE_SHORT:
		sprintf(buffer, "%" PRIi16, value.primitive.shortValue);
		break;
	case GVALUE_TYPE_INT:
		sprintf(buffer, "%" PRIi32, value.primitive.intValue);
		break;
	case GVALUE_TYPE_LONG:
		sprintf(buffer, "%" PRIi64, value.primitive.longValue);
		break;

	case GVALUE_TYPE_UBYTE:
		sprintf(buffer, "%" PRIu8, value.primitive.ubyteValue);
		break;
	case GVALUE_TYPE_USHORT:
		sprintf(buffer, "%" PRIu16, value.primitive.ushortValue);
		break;
	case GVALUE_TYPE_UINT:
		sprintf(buffer, "%" PRIu32, value.primitive.uintValue);
		break;
	case GVALUE_TYPE_ULONG:
		sprintf(buffer, "%" PRIu64, value.primitive.ulongValue);
		break;

	case GVALUE_TYPE_FLOAT:
		sprintf(buffer, "%f", value.primitive.floatValue);
		break;
	case GVALUE_TYPE_DOUBLE:
		sprintf(buffer, "%lf", value.primitive.doubleValue);
		break;

	case GVALUE_TYPE_POINTER:
		sprintf(buffer, "%p", value.primitive.pointerValue);
		break;

	case GVALUE_TYPE_CHAR:
		buffer[0] = value.primitive.charValue;
		buffer[1] = '\0';
		break;
	case GVALUE_TYPE_WCHAR:
		buffer[0] = value.primitive.wcharValue;
		buffer[1] = '\0';
		break;
	case GVALUE_TYPE_STRING:
		return private_allocStringToString(value.primitive.stringValue);
	case GVALUE_TYPE_WSTRING:
		return private_allocWStringToString(value.primitive.wstringValue);

	default:
		private_gvalue_unknownType(value.type, stdout);
		return private_allocStringToString("(unknown)");
	}

	return private_allocStringToString(buffer);
}

wchar_t *gvalue_getAllocWStringValue(struct gvalue_value value) {
	wchar_t buffer[GVALUE_BUFFER_SIZE];

	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		return value.primitive.boolValue == false ? private_allocStringToWString("false") : private_allocStringToWString("true");
	case GVALUE_TYPE_BYTE:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIi8, value.primitive.byteValue);
		break;
	case GVALUE_TYPE_SHORT:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIi16, value.primitive.shortValue);
		break;
	case GVALUE_TYPE_INT:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIi32, value.primitive.intValue);
		break;
	case GVALUE_TYPE_LONG:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIi64, value.primitive.longValue);
		break;

	case GVALUE_TYPE_UBYTE:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIu8, value.primitive.ubyteValue);
		break;
	case GVALUE_TYPE_USHORT:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIu16, value.primitive.ushortValue);
		break;
	case GVALUE_TYPE_UINT:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIu32, value.primitive.uintValue);
		break;
	case GVALUE_TYPE_ULONG:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%" PRIu64, value.primitive.ulongValue);
		break;

	case GVALUE_TYPE_FLOAT:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%f", value.primitive.floatValue);
		break;
	case GVALUE_TYPE_DOUBLE:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%lf", value.primitive.doubleValue);
		break;

	case GVALUE_TYPE_POINTER:
		swprintf(buffer, GVALUE_BUFFER_SIZE, L"%p", value.primitive.pointerValue);
		break;

	case GVALUE_TYPE_CHAR:
		buffer[0] = value.primitive.charValue;
		buffer[1] = '\0';
		break;
	case GVALUE_TYPE_WCHAR:
		buffer[0] = value.primitive.wcharValue;
		buffer[1] = '\0';
		break;
	case GVALUE_TYPE_STRING:
		return private_allocStringToWString(value.primitive.stringValue);
	case GVALUE_TYPE_WSTRING:
		return private_allocWStringToWString(value.primitive.wstringValue);

	default:
		private_gvalue_unknownType(value.type, stdout);
		return private_allocStringToWString("(unknown)");
	}

	return private_allocWStringToWString(buffer);
}

bool gvalue_free(struct gvalue_value value) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
	case GVALUE_TYPE_BYTE:
	case GVALUE_TYPE_SHORT:
	case GVALUE_TYPE_INT:
	case GVALUE_TYPE_LONG:

	case GVALUE_TYPE_UBYTE:
	case GVALUE_TYPE_USHORT:
	case GVALUE_TYPE_UINT:
	case GVALUE_TYPE_ULONG:

	case GVALUE_TYPE_FLOAT:
	case GVALUE_TYPE_DOUBLE:
		return false;

	case GVALUE_TYPE_POINTER:
		if (value.primitive.pointerValue != NULL) {
			free(value.primitive.pointerValue);
			return true;
		}
		return false;

	case GVALUE_TYPE_CHAR:
	case GVALUE_TYPE_WCHAR:
		return false;
	case GVALUE_TYPE_STRING:
		if (value.primitive.stringValue != NULL) {
			free(value.primitive.stringValue);
			return true;
		}
		return false;
	case GVALUE_TYPE_WSTRING:
		if (value.primitive.wstringValue != NULL) {
			free(value.primitive.wstringValue);
			return true;
		}
		return false;

	default:
		private_gvalue_unknownType(value.type, stdout);
		return false;
	}
}

int gvalue_cmp(struct gvalue_value value1, struct gvalue_value value2) {
	if (value1.type->code != value2.type->code) {
		return value1.type->code - value2.type->code;
	}

	switch (value1.type->code) {
	case GVALUE_TYPE_BOOL:
		return value1.primitive.boolValue - value2.primitive.boolValue;
	case GVALUE_TYPE_BYTE:
		return (int) (value1.primitive.byteValue) - (int) (value2.primitive.byteValue);
	case GVALUE_TYPE_SHORT:
		return (int) (value1.primitive.shortValue) - (int) (value2.primitive.shortValue);
	case GVALUE_TYPE_INT:
		return (value1.primitive.intValue < value2.primitive.intValue) ? -1 : (value1.primitive.intValue == value2.primitive.intValue) ? 0 : 1;
	case GVALUE_TYPE_LONG:
		return (value1.primitive.longValue < value2.primitive.longValue) ? -1 : (value1.primitive.longValue == value2.primitive.longValue) ? 0 : 1;

	case GVALUE_TYPE_UBYTE:
		return (value1.primitive.ubyteValue < value2.primitive.ubyteValue) ? -1 : (value1.primitive.ubyteValue == value2.primitive.ubyteValue) ? 0 : 1;
	case GVALUE_TYPE_USHORT:
		return (value1.primitive.ushortValue < value2.primitive.ushortValue) ? -1 : (value1.primitive.ushortValue == value2.primitive.ushortValue) ? 0 : 1;
	case GVALUE_TYPE_UINT:
		return (value1.primitive.uintValue < value2.primitive.uintValue) ? -1 : (value1.primitive.uintValue == value2.primitive.uintValue) ? 0 : 1;
	case GVALUE_TYPE_ULONG:
		return (value1.primitive.ulongValue < value2.primitive.ulongValue) ? -1 : (value1.primitive.ulongValue == value2.primitive.ulongValue) ? 0 : 1;

	case GVALUE_TYPE_FLOAT:
		return (value1.primitive.floatValue < value2.primitive.floatValue) ? -1 : (value1.primitive.floatValue == value2.primitive.floatValue) ? 0 : 1;
	case GVALUE_TYPE_DOUBLE:
		return (value1.primitive.doubleValue < value2.primitive.doubleValue) ? -1 : (value1.primitive.doubleValue == value2.primitive.doubleValue) ? 0 : 1;

	case GVALUE_TYPE_POINTER:
		return 0;

	case GVALUE_TYPE_CHAR:
		return (int) (value1.primitive.charValue) - (int) (value2.primitive.charValue);
	case GVALUE_TYPE_WCHAR:
		return (int) (value1.primitive.wcharValue) - (int) (value2.primitive.wcharValue);
	case GVALUE_TYPE_STRING:
		return strcmp(value1.primitive.stringValue, value2.primitive.stringValue);
	case GVALUE_TYPE_WSTRING:
		return wcscmp(value1.primitive.wstringValue, value2.primitive.wstringValue);

	default:
		private_gvalue_unknownType(value1.type, stdout);
		return 0;
	}
}

/*******************************************************************************************/

// Calculate hash code.

uint32_t gvalue_hashInt(uint32_t i) {
	return (i | 64) ^ ((i >> 15) | (i << 17));
}

uint32_t gvalue_hashLong(uint64_t i) {
	return gvalue_hashInt(((uint32_t) i) ^ ((uint32_t) (i >> 32)));
}

uint32_t gvalue_hashDouble(double d) {
	return gvalue_hashInt(((uint32_t) d + 5381) ^ ((uint32_t) (d * 72865789.0)));
}

uint32_t gvalue_hashString(char *string) {
	if (string == NULL) {
		return 0;
	}

    uint32_t hash = 5381;
    uint32_t c;

    for (int i = 0; string[i] != '\0'; i++) {
    	c = string[i];
    	if (c == 0) {
    		break;
    	}
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

uint32_t gvalue_hashWString(wchar_t *wstring) {
	if (wstring == NULL) {
		return 0;
	}

    uint32_t hash = 5381;
    uint32_t c;

    while (true) {
    	c = *wstring++;
    	if (c == 0) {
    		break;
    	}
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

uint32_t gvalue_hash(struct gvalue_value value) {
	switch (value.type->code) {
	case GVALUE_TYPE_BOOL:
		return value.primitive.boolValue;
	case GVALUE_TYPE_BYTE:
		return gvalue_hashInt((uint32_t) (value.primitive.byteValue));
	case GVALUE_TYPE_SHORT:
		return gvalue_hashInt((uint32_t) (value.primitive.shortValue));
	case GVALUE_TYPE_INT:
		return gvalue_hashInt(value.primitive.intValue);
	case GVALUE_TYPE_LONG:
		return gvalue_hashLong((uint64_t) (value.primitive.longValue));

	case GVALUE_TYPE_UBYTE:
		return gvalue_hashInt((uint32_t) (value.primitive.ubyteValue));
	case GVALUE_TYPE_USHORT:
		return gvalue_hashInt((uint32_t) (value.primitive.ushortValue));
	case GVALUE_TYPE_UINT:
		return gvalue_hashInt((uint32_t) (value.primitive.uintValue));
	case GVALUE_TYPE_ULONG:
		return gvalue_hashLong(value.primitive.ulongValue);

	case GVALUE_TYPE_FLOAT:
		return gvalue_hashDouble((double) (value.primitive.floatValue));
	case GVALUE_TYPE_DOUBLE:
		return gvalue_hashDouble(value.primitive.doubleValue);

	case GVALUE_TYPE_POINTER:
		return gvalue_hashLong((uint64_t) (value.primitive.pointerValue));

	case GVALUE_TYPE_CHAR:
		return gvalue_hashInt((int) (value.primitive.charValue));
	case GVALUE_TYPE_WCHAR:
		return gvalue_hashInt((int) (value.primitive.wcharValue));
	case GVALUE_TYPE_STRING:
		return gvalue_hashString(value.primitive.stringValue);
	case GVALUE_TYPE_WSTRING:
		return gvalue_hashWString(value.primitive.wstringValue);

	default:
		private_gvalue_unknownType(value.type, stdout);
		return -1;
	}
}
