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
int users_playlists(char* user_name, int kind, struct Track** result);

#endif //YANDEX_MUSIC_PLUGIN_API_H
