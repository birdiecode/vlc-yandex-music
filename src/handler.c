//
// Created by birdiecode on 13.02.24.
//

#include <vlc_common.h>
#include <vlc_stream_extractor.h>
#include <vlc_stream.h>
#include <vlc_input_item.h>
#include <regex.h>
#include "handler.h"
#include "api.h"

void extractFields(char *url, char **users, char **playlists) {
    regex_t regex;
    regmatch_t matches[3];
    regcomp(&regex, "/users/([^/]+)/playlists/([^/]+)", REG_EXTENDED);

    if (regexec(&regex, url, 3, matches, 0) == 0) {
        // Извлекаем значение для поля 'users'
        size_t username_length = matches[1].rm_eo - matches[1].rm_so;
        *users = (char*)malloc(username_length + 1);
        strncpy(*users, url + matches[1].rm_so, username_length);
        (*users)[username_length] = '\0';

        // Извлекаем значение для поля 'playlists'
        size_t playlist_length = matches[2].rm_eo - matches[2].rm_so;
        *playlists = (char*)malloc(playlist_length + 1);
        strncpy(*playlists, url + matches[2].rm_so, playlist_length);
        (*playlists)[playlist_length] = '\0';
    }

    regfree(&regex);
}

static int
MetadataReadDir(stream_directory_t* p_directory, input_item_node_t* p_node)
{

    msg_Info(p_directory, "%s", p_node->p_item->psz_uri);

    if (strstr(p_node->p_item->psz_uri, "yandextrack") != 0)
    {
        char *input_string = p_node->p_item->psz_uri;
        regex_t regex;
        regmatch_t match[3]; // Массив для хранения совпадений

        // Компилируем регулярное выражение для поиска чисел
        regcomp(&regex, "yandextrack://([0-9]+)/([0-9]+)", REG_EXTENDED);

        int track_id;
        int album_id;

        if (regexec(&regex, input_string, 3, match, 0) == 0) {
            char first_number_str[20];

            int i = 0;
            for (size_t j = match[1].rm_so; j < match[1].rm_eo; j++) {
                first_number_str[i++] = input_string[j];
            }
            first_number_str[i] = '\0';
            track_id = atoi(first_number_str);

            // Получаем второе число и преобразуем его в int
            char second_number_str[20];
            i = 0;
            for (size_t j = match[2].rm_so; j < match[2].rm_eo; j++) {
                second_number_str[i++] = input_string[j];
            }
            second_number_str[i] = '\0';
            album_id = atoi(second_number_str);

            char *rresult = malloc(400);
            rresult[0] = '\0';

            download_link(track_id, album_id, &rresult);

            struct vlc_readdir_helper rdh;
            vlc_readdir_helper_init(&rdh, p_directory, p_node);
            int ret = vlc_readdir_helper_additem(
                    &rdh, strdup(rresult), p_node->p_item->psz_name, NULL, ITEM_TYPE_FILE, ITEM_LOCAL);
            vlc_readdir_helper_finish(&rdh, true);
            free(rresult);
            return VLC_SUCCESS;
        } else {
            printf("Числа не найдены\n");
        }

        regfree(&regex);

        return 0;
    }
    else if (strstr(p_node->p_item->psz_uri, "https://music.yandex.ru") != 0)
    {

        char *users;
        char *playlists;

        extractFields(p_node->p_item->psz_uri, &users, &playlists);

        if (users && playlists) {
            struct Track* tracks = NULL;
            int res = users_playlists(users, atoi(playlists), &tracks);

            struct vlc_readdir_helper rdh;
            vlc_readdir_helper_init(&rdh, p_directory, p_node);


            struct Track* tracksTemp = tracks;
            if (res == 0){
                while (tracksTemp != NULL) {

                    char authorization[350];
                    sprintf(authorization, "yandextrack://%d/%d", tracksTemp->track_id, tracksTemp->album_id);

                    int ret = vlc_readdir_helper_additem(
                            &rdh, strdup(authorization), strdup(tracksTemp->track_name), NULL, ITEM_TYPE_FILE, ITEM_LOCAL);
                    if (ret != VLC_SUCCESS)
                        msg_Warn(p_directory, "Failed to add %s", tracksTemp->track_name);

                    tracksTemp = tracksTemp->next;
                }
            }

            freeTrackList(tracks);
            free(tracksTemp);
            vlc_readdir_helper_finish(&rdh, true);


            free(users);
            free(playlists);
            return VLC_SUCCESS;
        } else {
            msg_Warn(p_directory, "Не удалось извлечь поля из URL\n");
        }
    }

    return VLC_EGENERIC;
}

int
handlerFilter(vlc_object_t* p_obj){
    stream_directory_t* p_directory = (stream_directory_t*) p_obj;

    if (!stream_IsMimeType(p_directory->source, "application/yandexmusic"))
        return VLC_EGENERIC;

    p_directory->pf_readdir = MetadataReadDir;

    return VLC_SUCCESS;
}