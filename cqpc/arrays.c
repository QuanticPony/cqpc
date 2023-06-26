#ifndef _ARRAYS_C_INCLUDED
#define _ARRAYS_C_INCLUDED

#ifndef _ARRAYS_H_INCLUDED
#include "arrays.h"
#endif

#define SPACE  \



#define apply_macro(macro) ;\
_##macro##_(char);\
_##macro##_(short);\
_##macro##_(int);\
_##macro##_(long);\
_##macro##_(float);\
_##macro##_(double);


// Functions to empty arrays of different types
#define _empty_array_function_(typ) \
void cqpc_##typ##_array_empty(typ *array, size_t size) { \
    size_t i;\
    for (i = 0; i < size; i++)\
    {\
        array[i] = 0;\
    }\
} 

apply_macro(empty_array_function)

#undef _empty_array_function_


#define _set_value_array_function_(typ) \
void cqpc_##typ##_array_set_to(typ value, typ *array, size_t size) { \
    size_t i;\
    for (i = 0; i < size; i++)\
    {\
        array[i] = value;\
    }\
} 

apply_macro(set_value_array_function)

#undef _set_value_array_function_


// Functions to obtain max and min of an array of different types
#define get_value_function(typ, name, operation) \
typ cqpc_##typ##_array_get_##name##_value(typ *array, size_t size) { \
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
typ cqpc_##typ##_array_get_boundaries(typ *array, size_t size, typ *min, typ *max) { \
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


#define _boundary_functions_(typ) get_value_function(typ, min, <) get_value_function(typ, max, >) get_boundaries_function(typ)


apply_macro(boundary_functions)

#undef get_boundaries_function
#undef _boundary_functions_
#undef get_value_function




#define _copy_array_function_(typ) \
void cqpc_##typ##_array_copy_to(typ *from, typ *to, size_t size) { \
    size_t i;\
    for (i = 0; i < size; i++)\
    {\
        to[i] = from[i];\
    }\
} 

apply_macro(copy_array_function)

#undef _copy_array_function_


// MAP function over array

#define _map_array_(typ) \
void cqpc_map_##typ##_array(typ *array, size_t size, typ (*map_function)(typ)) {\
    size_t i;\
    for (i = 1; i < size; i++)\
    {\
        array[i] = map_function(array[i]);\
    }\
}


apply_macro(map_array)

#undef _map_array_

// END MAP




#endif //_ARRAYS_C_INCLUDED
