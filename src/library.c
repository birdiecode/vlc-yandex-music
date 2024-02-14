#define N_(str) (str)

/* VLC core API headers */
#include <vlc_common.h>
#include <vlc_plugin.h>

#include <string.h>
#include "interceptor.h"
#include "handler.h"
#include "config.h"


vlc_module_begin()
    set_shortname(N_("Yandex Music"))
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_STREAM_FILTER)
    set_description(N_("Yandex Music"))
    set_capability("stream_directory", 99)
    set_callbacks(handlerFilter, NULL)

    add_string(ACCOUNT_TOKEN_CONFIG, NULL, "Token", "Token you account.", false)

    add_submodule()
    set_description("Bittorrent magnet metadata access")
    set_capability("access", 60)
    add_shortcut("yandextrack", "https")
    set_callbacks(InterceptorOpen, NULL)
vlc_module_end()
