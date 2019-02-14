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
#include "py/objstringio.h"
#include "py/objint.h"
#include "py/objfun.h"
#include "py/runtime.h"
#include "py/binary.h"

#include "rpc/audsrv.h"
#include "utils.h"

#include <audsrv.h>

// creating the table of global members
STATIC const mp_rom_map_elem_t audsrv_rpc_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_CdPos),     MP_ROM_QSTR(MP_QSTR_CdPos) },
    { MP_ROM_QSTR(MP_QSTR_CdStatus),  MP_ROM_QSTR(MP_QSTR_CdStatus) },
    { MP_ROM_QSTR(MP_QSTR_CdType),  MP_ROM_QSTR(MP_QSTR_CdType) },
    { MP_ROM_QSTR(MP_QSTR_Format), MP_ROM_QSTR(MP_QSTR_Format) },
    { MP_ROM_QSTR(MP_QSTR_NumTracks), MP_ROM_QSTR(MP_QSTR_NumTracks) },
    { MP_ROM_QSTR(MP_QSTR_OnFillBuf), MP_ROM_QSTR(MP_QSTR_OnFillBuf) },
    { MP_ROM_QSTR(MP_QSTR_OnCddaStop),  MP_ROM_QSTR(MP_QSTR_OnCddaStop) },
    { MP_ROM_QSTR(MP_QSTR_Status), MP_ROM_QSTR(MP_QSTR_Status) },
    { MP_ROM_QSTR(MP_QSTR_TrackOffset), MP_ROM_QSTR(MP_QSTR_TrackOffset) },
    { MP_ROM_QSTR(MP_QSTR_TrackPos),  MP_ROM_QSTR(MP_QSTR_TrackPos) },
    { MP_ROM_QSTR(MP_QSTR_Volume), MP_ROM_QSTR(MP_QSTR_Volume) },
    { MP_ROM_QSTR(MP_QSTR_ChPlayAdpcm), (mp_obj_t)&ChPlayAdpcm_obj },
    { MP_ROM_QSTR(MP_QSTR_LoadAdpcm), (mp_obj_t)&LoadAdpcm_obj },
    { MP_ROM_QSTR(MP_QSTR_PlayAdpcm),   (mp_obj_t)&PlayAdpcm_obj },
    { MP_ROM_QSTR(MP_QSTR_PlayAudio), (mp_obj_t)&PlayAudio_obj },
    { MP_ROM_QSTR(MP_QSTR_PlayCd),    (mp_obj_t)&PlayCd_obj    },
    { MP_ROM_QSTR(MP_QSTR_PlaySectors), (mp_obj_t)&PlaySectors_obj },
    { MP_ROM_QSTR(MP_QSTR_PauseCd),   (mp_obj_t)&PauseCd_obj },
    { MP_ROM_QSTR(MP_QSTR_ResumeCd),  (mp_obj_t)&ResumeCd_obj },
    { MP_ROM_QSTR(MP_QSTR_StopAudio), (mp_obj_t)&StopAudio_obj },
    { MP_ROM_QSTR(MP_QSTR_StopCd),    (mp_obj_t)&StopCd_obj    },
    { MP_ROM_QSTR(MP_QSTR_WaitAudio), (mp_obj_t)&WaitAudio_obj },
    { MP_ROM_QSTR(MP_QSTR___del__), (mp_obj_t)&audsrv__del___obj },
};

STATIC const mp_rom_map_elem_t audsrv_format_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_freq),     MP_ROM_QSTR(MP_QSTR_freq)     },
    { MP_ROM_QSTR(MP_QSTR_bits),     MP_ROM_QSTR(MP_QSTR_bits)     },
    { MP_ROM_QSTR(MP_QSTR_channels), MP_ROM_QSTR(MP_QSTR_channels) },
};

STATIC const mp_rom_map_elem_t audsrv_callback_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_cb),     MP_ROM_QSTR(MP_QSTR_cb)     },
    { MP_ROM_QSTR(MP_QSTR_args),     MP_ROM_QSTR(MP_QSTR_args)     },
    { MP_ROM_QSTR(MP_QSTR_num_kw), MP_ROM_QSTR(MP_QSTR_num_kw) },
    { MP_ROM_QSTR(MP_QSTR_amount), MP_ROM_QSTR(MP_QSTR_amount) },
};

STATIC const mp_rom_map_elem_t audsrv_adpcm_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_pitch),     MP_ROM_QSTR(MP_QSTR_pitch)     },
    { MP_ROM_QSTR(MP_QSTR_loop),     MP_ROM_QSTR(MP_QSTR_loop)     },
    { MP_ROM_QSTR(MP_QSTR_channels), MP_ROM_QSTR(MP_QSTR_channels) },
    { MP_ROM_QSTR(MP_QSTR_buffer), MP_ROM_QSTR(MP_QSTR_buffer) },
    { MP_ROM_QSTR(MP_QSTR_size), MP_ROM_QSTR(MP_QSTR_size) },
};

STATIC MP_DEFINE_CONST_DICT(audsrv_rpc_locals_dict,
                            audsrv_rpc_locals_dict_table);

STATIC MP_DEFINE_CONST_DICT(audsrv_format_locals_dict,
                            audsrv_format_locals_dict_table);

STATIC MP_DEFINE_CONST_DICT(audsrv_callback_locals_dict,
                            audsrv_callback_locals_dict_table);

STATIC MP_DEFINE_CONST_DICT(audsrv_adpcm_locals_dict,
                            audsrv_adpcm_locals_dict_table);

// create the class-object itself
const mp_obj_type_t audsrv_rpc_obj = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_audsrv,
     // give it a constructor
    .make_new = audsrv__init__,
    // Attributes
    .attr = audsrv_rpc_property_attr,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&audsrv_rpc_locals_dict,
};

STATIC const mp_obj_type_t audsrv_format_obj = {
    // "inherit" the type "type"
    { &mp_type_type },
    .name = MP_QSTR_Format,
    .make_new = format__init__,
    .attr = format_property_attr,
    .locals_dict = (mp_obj_dict_t *)&audsrv_format_locals_dict,
};

STATIC const mp_obj_type_t audsrv_callback_obj = {
    { &mp_type_type },
    .name = MP_QSTR_Callback,
    .make_new = callback__init__,
    .attr = callback_property_attr,
    .locals_dict = (mp_obj_dict_t *)&audsrv_callback_locals_dict,
};

STATIC const mp_obj_type_t audsrv_adpcm_obj = {
    { &mp_type_type },
    .name = MP_QSTR_adpcm,
    .make_new = adpcm__init__,
    .attr = adpcm_property_attr,
    .locals_dict = (mp_obj_dict_t *)&audsrv_adpcm_locals_dict,
};

mp_obj_t audsrv__init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    audsrv_rpc_obj_t *self = m_new_obj(audsrv_rpc_obj_t);
    // give it a type
    self->base.type = &audsrv_rpc_obj;
    audsrv_init();
    
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t adpcm__init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 0, 5, true);
    // create a new object of our C-struct type
    audsrv_adpcm_obj_t *self = m_new_obj(audsrv_adpcm_obj_t);
    // give it a type
    self->base.type = &audsrv_adpcm_obj;
    
    switch (n_args) {
        case 4:
            INT_TYPE_CHECK("pitch", args[3]);
            self->adpcm.pitch = mp_obj_get_int(args[3]);
        case 3:
            INT_TYPE_CHECK("loop", args[2]);
            self->adpcm.loop = mp_obj_get_int(args[2]);
        case 2:
            INT_TYPE_CHECK("channels", args[1]);
            self->adpcm.channels = mp_obj_get_int(args[1]);
        case 1:
            IO_TYPE_CHECK("buffer", args[0]);
            mp_obj_stringio_t *io = args[0];                
            self->adpcm.buffer = io->vstr->buf;
            self->adpcm.size = io->vstr->len;
        default:
            break;
    }
    
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t callback__init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 0, 4, false);
    // create a new object of our C-struct type
    audsrv_callback_obj_t *self = m_new_obj(audsrv_callback_obj_t);
    // give it a type
    self->base.type = &audsrv_callback_obj;

    switch(n_args) {
        case 4:
            INT_TYPE_CHECK("num_kw", args[3]);
            self->num_kw = MP_OBJ_FROM_PTR(mp_obj_get_int(args[3]));
        case 3:
            LIST_TYPE_CHECK("args", args[2]);
            self->args = MP_OBJ_FROM_PTR(mp_obj_get_int(args[2]));
        case 2:
            FUNCTION_TYPE_CHECK("Callback function", args[1]);
            self->cb = MP_OBJ_FROM_PTR(mp_obj_get_int(args[1]));
        case 1:
            INT_TYPE_CHECK("amount", args[0]);
            self->amount = MP_OBJ_FROM_PTR(mp_obj_get_int(args[0]));
            break;
        default:
            self->amount = mp_const_none;
            self->args = mp_const_none;
            self->cb = mp_const_none;
    }
    
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t format__init__( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 0, 3, true);
    // create a new object of our C-struct type
    audsrv_fmt_obj_t *self = m_new_obj(audsrv_fmt_obj_t);
    // give it a type
    self->base.type = &audsrv_format_obj;
    
    switch(n_args) {
        case(3):
            INT_TYPE_CHECK("freq", args[2]);
            self->fmt.freq = mp_obj_get_int(args[2]);
        case(2):
            INT_TYPE_CHECK("bits", args[1]);
            self->fmt.bits = mp_obj_get_int(args[1]);
        case(1):
            INT_TYPE_CHECK("channels", args[0]);
            self->fmt.channels = mp_obj_get_int(args[0]);
            break;
        default:
            break;
    }
    
    return MP_OBJ_FROM_PTR(self);
}

void format_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        audsrv_adpcm_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_pitch:
                dest[0] = MP_OBJ_NEW_SMALL_INT(self->adpcm.pitch);
                break;
            case MP_QSTR_loop:
                dest[0] = MP_OBJ_NEW_SMALL_INT(self->adpcm.loop);
                break;
            case MP_QSTR_channels:
                dest[0] = MP_OBJ_NEW_SMALL_INT(self->adpcm.channels);
                break;
            case MP_QSTR_buffer:
                dest[0] = self->adpcm.buffer;
                break;
            case MP_QSTR_size:
                dest[0] = MP_OBJ_NEW_SMALL_INT(self->adpcm.size);
                break;
        }
        return;
    }
    else if(dest[0] == MP_OBJ_SENTINEL && dest[1] != MP_OBJ_NULL)
    {
        audsrv_adpcm_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_pitch:
                INT_TYPE_CHECK("pitch", dest[1]);
                self->adpcm.pitch = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_loop:
                INT_TYPE_CHECK("loop", dest[1]);
                self->adpcm.loop = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_channels:
                INT_TYPE_CHECK("channels", dest[1]);
                self->adpcm.channels = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_buffer:
                IO_TYPE_CHECK("buffer", dest[1]);
                mp_obj_stringio_t *io = dest[1];                
                self->adpcm.buffer = io->vstr->buf;
                self->adpcm.size = io->vstr->len;
                dest[0] = MP_OBJ_NULL;
                break;
            default :
                break;
        }
        return;
    }
    return;
}

void audsrv_rpc_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        // load attribute
        audsrv_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_CdPos:
                dest[0] = self->CdPos;
                break;
            case MP_QSTR_CdStatus:
                dest[0] = self->CdStatus;
                break;
            case MP_QSTR_CdType:
                dest[0] = self->CdType;
                break;
            case MP_QSTR_Format:
                dest[0] = self->Format;
                break;
            case MP_QSTR_NumTracks:
                dest[0] = self->NumTracks;
                break;
            case MP_QSTR_OnFillBuf:
                dest[0] = self->OnFillBuf;
                break;
            case MP_QSTR_OnCddaStop:
                dest[0] = self->OnCddaStop;
                break;
            case MP_QSTR_Status:
                dest[0] = self->Status;
                break;
            case MP_QSTR_TrackOffset:
                dest[0] = self->TrackOffset;
                break;
            case MP_QSTR_TrackPos:
                dest[0] = self->TrackPos;
                break;
            case MP_QSTR_Volume:
                dest[0] = self->Volume;
                break;
            default:
                // Automatically Raises Attribute Exception
                break;
        }
        return;
    }
    else if(dest[0] == MP_OBJ_SENTINEL && dest[1] != MP_OBJ_NULL)
    {
        audsrv_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case(MP_QSTR_Format):
                CUSTOM_TYPE_CHECK("Format", dest[1], "Format", &audsrv_format_obj);
                audsrv_fmt_obj_t *format = MP_OBJ_TO_PTR(dest[1]);
                audsrv_set_format(&format->fmt);
                ERROR_CHECK();
                self->Format = MP_OBJ_FROM_PTR(format);
                dest[0] = MP_OBJ_NULL;
                break;
            case(MP_QSTR_OnFillBuf):
                CUSTOM_TYPE_CHECK("OnFillBuf", dest[1], "Callback", &audsrv_callback_obj);
                audsrv_callback_obj_t *callback = MP_OBJ_TO_PTR(dest[1]);
                self->OnFillBuf = dest[1];
                audsrv_on_fillbuf(mp_obj_get_int(callback->amount), (audsrv_callback_t)execute_Callback, self);
                dest[0] = MP_OBJ_NULL;
                break;
            case(MP_QSTR_OnCddaStop):
                CUSTOM_TYPE_CHECK("OnCddaStop", dest[1], "Callback", &audsrv_callback_obj);
                self->OnCddaStop = dest[1];
                audsrv_on_cdda_stop((audsrv_callback_t)execute_Callback, self);
                dest[0] = MP_OBJ_NULL;
                break;
            case(MP_QSTR_Volume):
                INT_TYPE_CHECK("Volume", dest[1]);
                audsrv_adpcm_set_volume(-1, mp_obj_get_int(self->Volume));
                ERROR_CHECK();
                self->Volume = dest[1];
                dest[0] = MP_OBJ_NULL;
                break;
            default:
                // Automatically Raises Attribute Exception
                break;
        }
        return;
    }
    return;
}

void callback_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        audsrv_callback_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_cb:
                dest[0] = self->cb;
                break;
            case MP_QSTR_args:
                dest[0] = self->args;
                break;
            case MP_QSTR_num_kw:
                dest[0] = self->num_kw;
                break;
            case MP_QSTR_amount:
                dest[0] = self->amount;
            default:
                break;
        }
        return;
    }
    else if(dest[0] == MP_OBJ_SENTINEL && dest[1] != MP_OBJ_NULL) {
        audsrv_callback_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_cb:
                FUNCTION_TYPE_CHECK("cb", dest[1]);
                self->cb = dest[1];
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_args:
                LIST_TYPE_CHECK("args", dest[1]);
                self->args = dest[1];
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_num_kw:
                INT_TYPE_CHECK("num_kw", dest[1]);
                self->num_kw = dest[1];
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_amount:
                INT_TYPE_CHECK("amount", dest[1]);
                self->amount = dest[1];
                dest[0] = MP_OBJ_NULL;
                break;
            default:
                break;
        }
        return;
    }
    return;
}

void adpcm_property_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] == MP_OBJ_NULL) {
        audsrv_fmt_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_freq:
                dest[0] = mp_obj_new_int(self->fmt.freq);
                break;
            case MP_QSTR_bits:
                dest[0] = mp_obj_new_int(self->fmt.bits);
                break;
            case MP_QSTR_channels:
                dest[0] = mp_obj_new_int(self->fmt.channels);
                break;
            default:
                break;
        }
        return;
    }
    else if(dest[0] == MP_OBJ_SENTINEL && dest[1] != MP_OBJ_NULL) {
        audsrv_fmt_obj_t *self = MP_OBJ_TO_PTR(self_in);
        switch(attr) {
            case MP_QSTR_freq:
                INT_TYPE_CHECK("freq", dest[1]);
                self->fmt.freq = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_bits:
                INT_TYPE_CHECK("bits", dest[1]);
                self->fmt.bits = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_channels:
                INT_TYPE_CHECK("channels", dest[1]);
                self->fmt.channels = mp_obj_get_int(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
        }
    }
}

int execute_Callback(audsrv_rpc_obj_t *self) {
    audsrv_callback_obj_t *callbackObj = MP_OBJ_TO_PTR(self->OnFillBuf);
    mp_obj_fun_bc_t *callee = MP_OBJ_TO_PTR(callbackObj->cb);
    mp_obj_list_t *args = MP_OBJ_TO_PTR(callbackObj->args);
    size_t num_kw = mp_obj_get_int(callbackObj->num_kw);
    mp_call_fun_t fun = MICROPY_MAKE_POINTER_CALLABLE((void*)callee->bytecode);
    fun(callee, args->len, num_kw, MP_OBJ_FROM_PTR(args));
    //fun_bc_call(MP_OBJ_FROM_PTR(callee), args->len, num_kw, MP_OBJ_FROM_PTR(args));
    return 0;
}

mp_obj_t ChPlayAdpcm(mp_obj_t self_in, mp_obj_t ch_in, mp_obj_t adpcm_in) {
    int ch = mp_obj_get_int(ch_in);
    audsrv_adpcm_obj_t *adpcm = MP_OBJ_TO_PTR(adpcm_in);
    audsrv_ch_play_adpcm(ch, &adpcm->adpcm);
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t LoadAdpcm(mp_obj_t self_in, mp_obj_t adpcm_in) {
    audsrv_adpcm_obj_t *adpcm = MP_OBJ_TO_PTR(adpcm_in);
    audsrv_load_adpcm(&adpcm->adpcm, adpcm->adpcm.buffer, adpcm->adpcm.size);
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t PlayAdpcm(mp_obj_t self_in, mp_obj_t adpcm_in) {
    audsrv_adpcm_obj_t *adpcm = MP_OBJ_TO_PTR(adpcm_in);
    audsrv_ch_play_adpcm(-1, &adpcm->adpcm);
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t PlayAudio(mp_obj_t self_in, mp_obj_t chunk_in, mp_obj_t bytes) {
    mp_obj_stringio_t *chunk = MP_OBJ_TO_PTR(chunk_in);
    audsrv_play_audio(chunk->vstr->buf, mp_obj_get_int(bytes));
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t PlayCd(mp_obj_t self_in, mp_obj_t track) {
    audsrv_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self->Status = mp_obj_new_int(audsrv_play_cd(mp_obj_get_int(track)));
    return mp_const_none;
}

mp_obj_t PlaySectors(mp_obj_t self_in, mp_obj_t start_in, mp_obj_t end_in) {
    audsrv_play_sectors(mp_obj_get_int(start_in),
                                  mp_obj_get_int(end_in));
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t PauseCd(mp_obj_t self_in) {
    audsrv_pause_cd();
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t ResumeCd(mp_obj_t self_in) {
    audsrv_resume_cd();
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t StopAudio(mp_obj_t self_in) {
    audsrv_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self->Status = mp_obj_new_int(audsrv_stop_audio());
    return mp_const_none;
}

mp_obj_t StopCd(mp_obj_t self_in) {
    audsrv_rpc_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self->Status = mp_obj_new_int(audsrv_stop_cd());
    return mp_const_none;
}

mp_obj_t WaitAudio(mp_obj_t self_in, mp_obj_t bytes) {
    audsrv_wait_audio(mp_obj_get_int(bytes));
    ERROR_CHECK();
    return mp_const_none;
}

mp_obj_t audsrv__del__(mp_obj_t self_in) {
    if(audsrv_quit())
        mp_raise_msg(&mp_type_Exception, "audsrv failed to shutdown\n");
    return mp_const_none;
}
