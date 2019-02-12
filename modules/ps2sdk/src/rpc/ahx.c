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

#include "mpconfigport.h"
#include "py/nlr.h"
#include "py/obj.h"
#include "py/objstr.h"
#include "py/objstringio.h"
#include "py/runtime.h"
#include "py/binary.h"
#include "rpc/ahx.h"

#include <ahx_rpc.h>
#include <string.h>

// creating the table of global members
STATIC const mp_rom_map_elem_t ahx_rpc_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_Boost), MP_ROM_QSTR(MP_QSTR_Boost) },
    { MP_ROM_QSTR(MP_QSTR_SubSongs), MP_ROM_QSTR(MP_QSTR_SubSongs) },
    { MP_ROM_QSTR(MP_QSTR_Volume), MP_ROM_QSTR(MP_QSTR_Volume) },
    { MP_ROM_QSTR(MP_QSTR_Pause), (mp_obj_t)&Pause_obj },
    { MP_ROM_QSTR(MP_QSTR_Play), (mp_obj_t)&Play_obj },
    { MP_ROM_QSTR(MP_QSTR_SubSong), (mp_obj_t)&SubSong_obj },
    { MP_ROM_QSTR(MP_QSTR_ToggleOversampling), (mp_obj_t)&ToggleOversampling_obj },
    { MP_ROM_QSTR(MP_QSTR___del__), (mp_obj_t)&__del___obj },
    { MP_ROM_QSTR(MP_QSTR___enter__), (mp_obj_t)&__init__ },
    { MP_ROM_QSTR(MP_QSTR___exit__), (mp_obj_t)&__del___obj }
};

STATIC MP_DEFINE_CONST_DICT(ahx_rpc_locals_dict,
                            ahx_rpc_locals_dict_table);

// create the class-object itself
const mp_obj_type_t ahx_rpc_obj = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_AHX,
     // give it a constructor
    .make_new = __init__,
     // give it some properties
    .attr = ahx_rpc_property_attr,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&ahx_rpc_locals_dict,
};

mp_obj_t __init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    ahx_rpc_obj_t *self = m_new_obj(ahx_rpc_obj_t);
    // give it a type
    self->base.type = &ahx_rpc_obj;
    AHX_Init();
    if (MP_OBJ_IS_STR(args[0]))
    {
        mp_obj_str_t *input = args[0];
        self->subSongs = mp_obj_new_int(AHX_LoadSong((char *)input->data));
    }
    else // need to add check for BytesIO type
    {
        mp_obj_stringio_t *io = args[0];
        self->subSongs = mp_obj_new_int(AHX_LoadSongBuffer(io->vstr->buf, io->vstr->len));
    }

    self->volume = mp_obj_new_int(0); // Not sure what the start volume is.
    self->boost = self->volume; // Tiny bit less overhead
    
    return MP_OBJ_FROM_PTR(self);
}

STATIC void ahx_rpc_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        // load attribute
        ahx_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case(MP_QSTR_Boost):
                dest[0] = self->boost;
                break;
            case(MP_QSTR_SubSongs):
                dest[0] = self->subSongs;
                break;
            case(MP_QSTR_Volume):
                dest[0] = self->volume;
                break;
            default:
                // Automatically Raises Attribute Exception
                break;
        }
        return;
    }
    else if(dest[0] == MP_OBJ_SENTINEL && dest[1] != MP_OBJ_NULL)
    {
        ahx_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
        int error;
        switch(attr) {
            case(MP_QSTR_Boost):
                self->boost = dest[1];
                error = AHX_SetBoost(mp_obj_get_int(dest[1]));
                dest[0] = MP_OBJ_NULL;
                break;
            case(MP_QSTR_Volume):
                self->volume = dest[1];
                error = AHX_SetVolume(mp_obj_get_int(dest[1]));
                // Acknowlege Attribute Found
                dest[0] = MP_OBJ_NULL;
                break;
        }
        return;
    }
    return;
}

mp_obj_t Pause(void) {
    AHX_Pause();
    return mp_const_none;
}

mp_obj_t Play(void) {
    AHX_Play();
    return mp_const_none;
}

mp_obj_t SubSong(mp_obj_t self_in, mp_obj_t songNo) {
    AHX_SubSong(mp_obj_get_int(songNo));
    return mp_const_none;
}

mp_obj_t ToggleOversampling(void) {
    AHX_ToggleOversampling();
    return mp_const_none;
}

mp_obj_t __del__(void) {
    AHX_Quit();
    return mp_const_none;
}
