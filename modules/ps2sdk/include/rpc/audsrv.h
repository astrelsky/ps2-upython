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

#ifndef __AUDSRV_RPC_H__
#define __AUDSRV_RPC_H__

#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

#include <audsrv.h>

// This really shouldn't be static
extern mp_obj_t fun_bc_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args);

#define ERROR_CHECK() ({\
    if (audsrv_get_error())\
        mp_raise_msg(&mp_type_Exception, audsrv_get_error_string());\
})

typedef struct _audsrv_rpc_obj_t {
    mp_obj_base_t base;
    mp_obj_t CdPos;
    mp_obj_t CdStatus;
    mp_obj_t CdType;
    mp_obj_t Format;
    mp_obj_t NumTracks;
    mp_obj_t OnFillBuf;
    mp_obj_t OnCddaStop;
    mp_obj_t Status;
    mp_obj_t TrackOffset;
    mp_obj_t TrackPos;
    mp_obj_t Volume;
} audsrv_rpc_obj_t;

typedef struct audsrv_fmt_obj_t {
    mp_obj_base_t base;
    audsrv_fmt_t fmt;
} audsrv_fmt_obj_t;

typedef struct audsrv_callback_obj_t {
    mp_obj_base_t base;
    mp_obj_t cb;
    mp_obj_t args;
    mp_obj_t num_kw;
    mp_obj_t amount;
} audsrv_callback_obj_t;

typedef struct audsrv_adpcm_obj_t
{
    mp_obj_base_t base;
    audsrv_adpcm_t adpcm;
} audsrv_adpcm_obj_t;

// Class and Object Tables
STATIC const mp_rom_map_elem_t audsrv_rpc_locals_dict_table[];
STATIC const mp_rom_map_elem_t audsrv_format_locals_dict_table[];
STATIC const mp_rom_map_elem_t audsrv_callback_locals_dict_table[];

// Class and Object Prototypes
const mp_obj_type_t audsrv_rpc_obj;
STATIC const mp_obj_type_t audsrv_format_obj;
STATIC const mp_obj_type_t audsrv_callback_obj;
STATIC const mp_obj_type_t audsrv_adpcm_obj;

// Constructors
mp_obj_t audsrv__init__(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t adpcm__init__(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t callback__init__(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t format__init__(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

// Property Attributes
void adpcm_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest);
void audsrv_rpc_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest);
void callback_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest);
void format_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest);

// Function Prototypes
int execute_Callback(audsrv_rpc_obj_t *self);
mp_obj_t ChPlayAdpcm(mp_obj_t self_in, mp_obj_t ch_in, mp_obj_t adpcm_in);
mp_obj_t LoadAdpcm(mp_obj_t self_in, mp_obj_t adpcm_in);
mp_obj_t PlayAdpcm(mp_obj_t self_in, mp_obj_t adpcm_in);
mp_obj_t PlayAudio(mp_obj_t self_in, mp_obj_t chunk_in, mp_obj_t bytes);
mp_obj_t PlayCd(mp_obj_t self_in, mp_obj_t track);
mp_obj_t PlaySectors(mp_obj_t self_in, mp_obj_t start_in, mp_obj_t end_in);
mp_obj_t PauseCd(mp_obj_t self_in);
mp_obj_t ResumeCd(mp_obj_t self_in);
mp_obj_t StopAudio(mp_obj_t self_in);
mp_obj_t StopCd(mp_obj_t self_in);
mp_obj_t WaitAudio(mp_obj_t self_in, mp_obj_t bytes);
mp_obj_t audsrv__del__(mp_obj_t self_in);

// Function Objects
STATIC MP_DEFINE_CONST_FUN_OBJ_3(ChPlayAdpcm_obj, ChPlayAdpcm);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(LoadAdpcm_obj, LoadAdpcm);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(PlayAdpcm_obj, PlayAdpcm);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(PlayAudio_obj, PlayAudio);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(PlayCd_obj, PlayCd);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(PlaySectors_obj, PlaySectors);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(PauseCd_obj, PauseCd);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(ResumeCd_obj, ResumeCd);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(StopAudio_obj, StopAudio);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(StopCd_obj, StopCd);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(WaitAudio_obj, WaitAudio);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(audsrv__del___obj, audsrv__del__);

#endif // __AUDSRV_RPC_H__
