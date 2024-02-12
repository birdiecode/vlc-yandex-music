#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>


// gcc -o json_parser json_parser.c -lcjson
int main() {
    char json[] = "{\"invocationInfo\":{\"req-id\":\"1707684127492065-10016213430992908568\",\"hostname\":\"music-stable-back-sas-76.sas.yp-c.yandex.net\",\"exec-duration-millis\":25},\"result\":{\"library\":{\"uid\":730767098,\"revision\":3502,\"playlistUuid\":\"c2688b54-a05d-7f01-969d-66ba9dc2ae8d\",\"tracks\":[{\"id\":\"115711467\",\"albumId\":\"26681176\",\"timestamp\":\"2024-02-10T10:51:01+00:00\"},{\"id\":\"120273812\",\"albumId\":\"28652093\",\"timestamp\":\"2024-02-10T10:51:01+00:00\"},{\"id\":\"120624907\",\"albumId\":\"28802628\",\"timestamp\":\"2024-02-10T10:48:25+00:00\"},{\"id\":\"26029231\",\"albumId\":\"3087807\",\"timestamp\":\"2018-11-17T14:22:37+00:00\"},{\"id\":\"26901555\",\"albumId\":\"3398240\",\"timestamp\":\"2018-10-04T22:30:18+00:00\"}]}}}";

    // Парсинг JSON
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        printf("Ошибка при парсинге JSON.\n");
        return 1;
    }

    // Находим массив треков
    cJSON *tracks = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "result"), "library"), "tracks");
    if (tracks == NULL) {
        printf("Массив треков не найден.\n");
        cJSON_Delete(root);
        return 1;
    }

    // Выводим идентификаторы треков на консоль
    printf("Идентификаторы треков:\n");
    for (int i = 0; i < cJSON_GetArraySize(tracks); i++) {
        cJSON *track = cJSON_GetArrayItem(tracks, i);
        cJSON *track_id = cJSON_GetObjectItem(track, "id");
        cJSON *track_album_id = cJSON_GetObjectItem(track, "albumId");

        if (track_id != NULL) {
            printf("%s\t%s\n", track_id->valuestring, track_album_id->valuestring);
        }
    }

    // Освобождаем память
    cJSON_Delete(root);

    return 0;
}
