/**
 * @file hello.c
 * @brief Hello world interface VLC module example
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define DOMAIN  "vlc-myplugin"
#define _(str)  dgettext(DOMAIN, str)
#define N_(str) (str)

#define DLDIR_CONFIG "bittorrent-download-path"
#define KEEP_CONFIG "bittorrent-keep-files"
#define ACCOUNT_LOGIN_CONFIG "yandex-music-login"
#define ACCOUNT_PASSWORD_CONFIG "yandex-music-password"

#include <stdlib.h>
/* VLC core API headers */
#include <vlc_common.h>
#include <vlc_access.h>
#include <vlc_demux.h>
#include <vlc_dialog.h>
#include <vlc_es.h>
#include <vlc_fs.h>
#include <vlc_input.h>
#include <vlc_input_item.h>
#include <vlc_interface.h>
#include <vlc_interrupt.h>
#include <vlc_plugin.h>
#include <vlc_stream.h>
#include <vlc_stream_extractor.h>
#include <vlc_threads.h>
#include <vlc_url.h>
#include <vlc_variables.h>

#include <string.h>

/* Forward declarations */
static int MetadataOpen(vlc_object_t* p_this);
static int MagnetMetadataOpen(vlc_object_t* p_this);
static void MagnetMetadataClose(vlc_object_t* p_this);

vlc_module_begin()
    set_shortname("Yandex Music")
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_STREAM_FILTER)
    set_description("Yandex Music")
    set_capability("stream_directory", 99)
    set_callbacks(MetadataOpen, NULL)

    add_string(ACCOUNT_LOGIN_CONFIG, NULL, "Login","Login you account.", false)
    add_password(ACCOUNT_PASSWORD_CONFIG, NULL, "Password","Password you account.", false)

    add_submodule()
        set_description("Bittorrent magnet metadata access")
        set_capability("access", 60)
        add_shortcut("http", "https")
        set_callbacks(MagnetMetadataOpen, MagnetMetadataClose)
vlc_module_end()

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
    intf_thread_t *intf = (intf_thread_t *)p_access;
    msg_Info(intf, "7 Good bye YaM!");
    return (ssize_t) 0;
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
        *va_arg(args, char**) = strdup("application/x-bittorrent");
        break;
    default:
        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}


int
MetadataOpen(vlc_object_t* p_this)
{
    intf_thread_t *intf = (intf_thread_t *)p_this;
    msg_Info(intf, "9 Good bye YaM!");
    return VLC_EGENERIC;
}

int
MagnetMetadataOpen(vlc_object_t* p_this)
{
    intf_thread_t *intf = (intf_thread_t *)p_this;
    msg_Info(intf, "2 Good bye YaM!");

    stream_t* p_access = (stream_t*) p_this;
    msg_Info(intf, "5 Good bye YaM! %s", p_access->psz_name);

    if (strcmp(p_access->psz_name, "http")==0){
         msg_Info(intf, "4 Good bye YaM! %s", p_access->psz_location);

    } else if (strcmp(p_access->psz_name, "https")==0){
         msg_Info(intf, "4 Good bye YaM! %s", p_access->psz_location);


    } else {
        return VLC_EGENERIC;
    }

    intf_sys_t *p_sys = malloc( sizeof( intf_sys_t ) );
    if ( unlikely(p_sys == NULL) )
        return VLC_ENOMEM;

    p_access->p_sys;
    p_access->pf_read = MagnetMetadataRead;
    p_access->pf_control = MagnetMetadataControl;

    return VLC_SUCCESS;
}

void
MagnetMetadataClose(vlc_object_t* p_this)
{
    intf_thread_t *intf = (intf_thread_t *)p_this;
    msg_Info(intf, "3 Good bye YaM!");

}
