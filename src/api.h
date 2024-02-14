//
// Created by birdiecode on 13.02.24.
//

#ifndef YANDEX_MUSIC_PLUGIN_API_H
#define YANDEX_MUSIC_PLUGIN_API_H

struct Track {
    char* track_name;
    char* album_name;
    int track_id;
    int album_id;
    struct Track* next;
};

void freeTrackList(struct Track* node);

// Получение треков
int users_playlists(char* user_name, int kind, char* tkn, struct Track** result);

int download_info(int trId, int alId, char* tkn, char **rresult);

int download_link(int trId, int alId, char* tkn, char **rresult);
#endif //YANDEX_MUSIC_PLUGIN_API_H
