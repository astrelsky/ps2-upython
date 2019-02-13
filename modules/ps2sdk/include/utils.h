/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Andrew J. Strelsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "py/obj.h"
#include "py/runtime.h"

#include <string.h>

// Hiding the warnings since they are being used in macros

STATIC __attribute__ ((unused)) const char *CUSTOM_TYPE_ERROR_MSG = " must be a(n) \n";
STATIC __attribute__ ((unused)) const char *FUNCTION_TYPE_ERROR_MSG = " must be the NAME of a defined function\n";
STATIC __attribute__ ((unused)) const char *INT_TYPE_ERROR_MSG = " must be an integer\n";
STATIC __attribute__ ((unused)) const char *IO_TYPE_ERROR_MSG = " must be an io object\n";
STATIC __attribute__ ((unused)) const char *LIST_TYPE_ERROR_MSG = " must be a list\n";

// This one is a bit silly
STATIC __attribute__ ((unused)) const char *OBJECT_STRING = " object\n";


#define CUSTOM_TYPE_CHECK(name, value, type_name, type_address) ({\
    if(!mp_obj_is_type(value, type_address))\
        mp_raise_TypeError(strcat(\
            strcat(name, CUSTOM_TYPE_ERROR_MSG),\
            strcat(type_name, OBJECT_STRING)));\
})

#define FUNCTION_TYPE_CHECK(name, value) ({\
    if(!mp_obj_is_fun(value))\
        mp_raise_TypeError(strcat(name, FUNCTION_TYPE_ERROR_MSG));\
})

#define INT_TYPE_CHECK(name, value) ({\
    if(!mp_obj_is_int(value))\
        mp_raise_TypeError(strcat(name, INT_TYPE_ERROR_MSG));\
})

#define IO_TYPE_CHECK(name, value) ({\
    if(!mp_obj_is_type(value, &mp_type_bytesio) && !mp_obj_is_type(value, &mp_type_stringio))\
        mp_raise_TypeError(strcat(name, IO_TYPE_ERROR_MSG));\
})

#define LIST_TYPE_CHECK(name, value) ({\
    if(!mp_obj_is_type(value, &mp_type_list))\
        mp_raise_TypeError(strcat(name, LIST_TYPE_ERROR_MSG));\
})

#endif // __UTILS_H__
