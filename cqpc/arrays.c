#ifndef _ARRAYS_C_INCLUDED
#define _ARRAYS_C_INCLUDED

#ifndef _ARRAYS_H_INCLUDED
#include "arrays.h"
#endif

// Functions to empty arrays of different types
#define empty_array_function(typ) void empty_##typ##_array(typ *array, size_t size) { \
    size_t i;\
    for (i = 0; i < size; i++)\
    {\
        array[i] = 0;\
    }\
} 

empty_array_function(char);
empty_array_function(short);
empty_array_function(int);
empty_array_function(long);
empty_array_function(float);
empty_array_function(double);

#undef empty_array_function


#define set_value_array_function(typ) \
void set_##typ##_array_to(typ value, typ *array, size_t size) { \
    size_t i;\
    for (i = 0; i < size; i++)\
    {\
        array[i] = value;\
    }\
} 

set_value_array_function(char);
set_value_array_function(short);
set_value_array_function(int);
set_value_array_function(long);
set_value_array_function(float);
set_value_array_function(double);

#undef set_value_array_function


// Functions to obtain max and min of an array of different types
#define get_value_function(typ, name, operation) \
typ get_##name##_##typ##_value(typ *array, size_t size) { \
    size_t i;\
    typ tmp;\
    tmp = array[0];\
    for (i = 1; i < size; i++)\
    {\
        if (array[i] operation tmp) {\
            tmp = array[i];\
        }\
    }\
    return tmp;\
} 
#define get_boundaries_function(typ) \
typ get_boundaries_##typ##_value(typ *array, size_t size, typ *min, typ *max) { \
    size_t i;\
    typ *val, tmp_min, tmp_max;\
    tmp_min = tmp_max = array[0];\
    for (i = 1; i < size; i++)\
    {\
        val = array + i;\
        if (*val < tmp_min) {\
            tmp_min = *val;\
        } else if (*val > tmp_max) { \
            tmp_max = *val;\
        }\
    }\
    *min = tmp_min;\
    *max = tmp_max;\
} 


#define boundary_functions(typ) get_value_function(typ, min, <) get_value_function(typ, max, >) get_boundaries_function(typ)


boundary_functions(char);
boundary_functions(short);
boundary_functions(int);
boundary_functions(long);
boundary_functions(float);
boundary_functions(double);


#undef get_boundaries_function
#undef boundary_functions
#undef get_value_function

#endif //_ARRAYS_C_INCLUDED
