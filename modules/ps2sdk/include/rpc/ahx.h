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

#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

#ifndef __CLASS_AXH_RPC_H__
#define __CLASS_AXH_RPC_H__

typedef struct _ahx_rpc_obj_t {
    
    // Type Information
    mp_obj_base_t base;

    // AHX Attributes
    mp_obj_t boost;
    mp_obj_t subSongs;
    mp_obj_t volume;

} ahx_rpc_obj_t;

const mp_obj_type_t ahx_rpc_obj;

mp_obj_t ahx__init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args );

void ahx_rpc_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest);

mp_obj_t Pause(void);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(Pause_obj, Pause);

mp_obj_t Play(void);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(Play_obj, Play);

mp_obj_t SubSong(mp_obj_t self_in, mp_obj_t songNo);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(SubSong_obj, SubSong);

mp_obj_t ToggleOversampling(void);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(ToggleOversampling_obj, ToggleOversampling);

mp_obj_t ahx__del__(void);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(ahx__del___obj, ahx__del__);

#endif // __CLASS_AXH_RPC_H__
