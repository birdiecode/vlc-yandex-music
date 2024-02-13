//
// Created by birdiecode on 13.02.24.
//

#include <vlc_common.h>
#include <vlc_stream.h>
#include <vlc_dialog.h>
#include <vlc_interface.h>
#include "interceptor.h"

struct intf_sys_t
{
    vlc_mutex_t      lock;
    vlc_timer_t      timer;

    vlc_value_t origin_audiotrack;
    vlc_value_t origin_subtrack;
    bool is_origin;

    int64_t          i_start_time;
    int64_t          i_end_time;

    vout_thread_t   *p_vout;
    input_thread_t  *p_input;
};


static ssize_t
MagnetMetadataRead(stream_t* p_access, void* p_buffer, size_t i_len)
{
    return 1;
}

static int
MagnetMetadataControl(stream_t* access, int query, va_list args)
{
    intf_thread_t *intf = (intf_thread_t *)access;
    msg_Info(intf, "8 Good bye YaM!");

    switch (query) {
        case STREAM_GET_PTS_DELAY:
            *va_arg(args, int64_t*) = DEFAULT_PTS_DELAY;
            break;
        case STREAM_CAN_SEEK:
            *va_arg(args, bool*) = false;
            break;
        case STREAM_CAN_PAUSE:
            *va_arg(args, bool*) = false;
            break;
        case STREAM_CAN_CONTROL_PACE:
            *va_arg(args, bool*) = true;
            break;
        case STREAM_GET_CONTENT_TYPE:
            *va_arg(args, char**) = strdup("application/yandexmusic");
            break;
        default:
            return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}

// проверяем относится ли ссылка к yandex music
int
InterceptorOpen(vlc_object_t* p_this)
{
    stream_t* p_access = (stream_t*) p_this;
    msg_Info(p_access, "[InterceptorOpen] check");

    if (strcmp(p_access->psz_name, "https")==0){
        msg_Info(p_access, "[InterceptorOpen] location %s", p_access->psz_location);
        if (strstr(p_access->psz_location, "music.yandex.ru") != 0){
            msg_Info(p_access, "[InterceptorOpen] ok");
        } else {
            return VLC_EGENERIC;
        }
    } else {
        return VLC_EGENERIC;
    }

    // если относиться то уставливаем эти штуки
    intf_sys_t *p_sys = malloc( sizeof( intf_sys_t ) );
    if ( unlikely(p_sys == NULL) )
        return VLC_ENOMEM;

    p_access->p_sys = p_sys;

    p_access->pf_read = MagnetMetadataRead;
    p_access->pf_control = MagnetMetadataControl;

    return VLC_SUCCESS;
}

void
InterceptorClose(vlc_object_t* p_this){

}
