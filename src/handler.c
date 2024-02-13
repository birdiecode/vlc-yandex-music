//
// Created by birdiecode on 13.02.24.
//

#include <vlc_common.h>
#include <vlc_stream_extractor.h>
#include <vlc_stream.h>
#include <vlc_input_item.h>
#include "handler.h"
#include "api.h"


static int
MetadataReadDir(stream_directory_t* p_directory, input_item_node_t* p_node)
{

    msg_Info(p_directory, "%s", p_node->p_item->psz_uri);

    struct Track* tracks = NULL;
    int res = users_playlists("", 1024, &tracks);

    struct vlc_readdir_helper rdh;
    vlc_readdir_helper_init(&rdh, p_directory, p_node);


    struct Track* tracksTemp = tracks;
    if (res == 0){
        while (tracksTemp != NULL) {
            int ret = vlc_readdir_helper_additem(
                    &rdh, "hdsafsad://fasdfas/dsfasd", strdup(tracksTemp->track_name), NULL, ITEM_TYPE_FILE, ITEM_LOCAL);
            if (ret != VLC_SUCCESS)
                msg_Warn(p_directory, "Failed to add %s", tracksTemp->track_name);

            tracksTemp = tracksTemp->next;
        }
    }

    freeTrackList(tracks);
    free(tracksTemp);
    vlc_readdir_helper_finish(&rdh, true);

    return VLC_SUCCESS;
}

int
handlerFilter(vlc_object_t* p_obj){
    stream_directory_t* p_directory = (stream_directory_t*) p_obj;

    if (!stream_IsMimeType(p_directory->source, "application/yandexmusic"))
        return VLC_EGENERIC;

    p_directory->pf_readdir = MetadataReadDir;

    return VLC_SUCCESS;
}