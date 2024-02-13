//
// Created by birdiecode on 13.02.24.
//

#include "api.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

//Функция раширения памяти и записи результата
size_t write_callback(void *ptr, size_t size, size_t nmemb, char **data) {
    size_t data_size = size * nmemb;
    *data = realloc(*data, strlen(*data) + data_size + 1);
    if (*data) {
        strncat(*data, ptr, data_size);
    }
    return data_size;
}

//Функция создания запросов
CURLcode curl(char *url, char *token, char **result){
    CURL *curl;
    CURLcode res;
    char authorization[250];

    sprintf(authorization, "Authorization: OAuth %s", token);

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, authorization);
        chunk = curl_slist_append(chunk, "Accept-Language: ru");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    } else {
        res = CURLE_SEND_ERROR;
    }
    return res;
}

void pushTrack(struct Track** head_ref, const char* trName, const char* albName, int trId, int albId) {
    struct Track* new_node = (struct Track*)malloc(sizeof(struct Track));

    new_node->track_name = strdup(trName);
    new_node->album_name = strdup(albName);
    new_node->track_id = trId;
    new_node->album_id = albId;

    new_node->next = (*head_ref);

    // Обновление указателя на головной узел
    (*head_ref) = new_node;
}

// Освобождение памяти от треков
void freeTrackList(struct Track* node) {
    struct Track* current = node;
    struct Track* next;
    while (current != NULL) {
        next = current->next;
        free(current->track_name);
        free(current->album_name);
        free(current);
        current = next;
    }
}

// Получение треков
int users_playlists(char* user_name, int kind, struct Track** result)
{
    char *resp = malloc(1);
    resp[0] = '\0';

    CURLcode res = curl("https://api.music.yandex.net/users/r00tl0l/playlists/3", "y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ", &resp);

    if (res == CURLE_OK) {
        // Парсим json
        cJSON *root = cJSON_Parse(resp);
        if (root == NULL) {
            return 1;
        }

        cJSON *result_json = cJSON_GetObjectItem(root, "result");
        cJSON *tracks_array = cJSON_GetObjectItem(result_json, "tracks");
        if (tracks_array == NULL) {
            return 2;
        }

        cJSON *track = NULL;
        cJSON_ArrayForEach(track, tracks_array) {
            cJSON *track_info = cJSON_GetObjectItem(track, "track");
            if (track_info == NULL) {
                continue;
            }

            cJSON *track_title = cJSON_GetObjectItem(track_info, "title");
            cJSON *track_id = cJSON_GetObjectItem(track_info, "id");

            cJSON *albums = cJSON_GetObjectItem(track_info, "albums");
            if (albums == NULL) {
                continue;
            }
            cJSON *firstAlbum = cJSON_GetArrayItem(albums, 0);

            cJSON *album_title = cJSON_GetObjectItem(firstAlbum, "title");
            cJSON *album_id = cJSON_GetObjectItem(firstAlbum, "id");

            if (track_title != NULL && track_id != NULL && album_title != NULL && album_id != NULL) {
                // Добавляем треки в связный список
                pushTrack(
                        result, track_title->valuestring, album_title->valuestring,
                        atoi(track_id->valuestring), album_id->valueint
                );
            }
        }
        cJSON_Delete(root);
    }
    free(resp);
    return 0;
}

int
download_info(int trId, int alId, char **rresult){
    char url[100];
    sprintf(url, "https://api.music.yandex.net/tracks/%d:%d/download-info", trId, alId);

    char *resp = malloc(1);
    resp[0] = '\0';

    CURLcode res = curl(url, "y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ", &resp);

    if (res == CURLE_OK) {
        printf("Ответ на запрос:\n%s\n", resp);

        cJSON *root = cJSON_Parse(resp);
        if (root == NULL) {
            printf("Ошибка при парсинге JSON.\n");
            return 1;
        }

        cJSON *result = cJSON_GetObjectItem(root, "result");
        if (result && cJSON_IsArray(result) && cJSON_GetArraySize(result) > 0) {
            cJSON *firstResultObj = cJSON_GetArrayItem(result, 0);

            cJSON *downloadInfoUrl = cJSON_GetObjectItem(firstResultObj, "downloadInfoUrl");
            if (downloadInfoUrl && cJSON_IsString(downloadInfoUrl)) {
                char *new_link = strdup(downloadInfoUrl->valuestring);
                //strcpy(new_link, *rresult);
                free(*rresult); // Освобождаем память, выделенную для оригинальной строки
                *rresult = new_link;
            }
        }
        cJSON_Delete(root);
    } else {
        return 2;
    }
    return 0;
}

int parseXML(const char *xml_string, char **rresult) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadMemory(xml_string, strlen(xml_string), NULL, NULL, 0);
    if (doc == NULL) {
        printf("Ошибка при парсинге XML из строки.\n");
        return 1;
    }

    root_element = xmlDocGetRootElement(doc);

    xmlNode *cur_node = NULL;
    char *host = NULL;
    char *path = NULL;
    char *sign = NULL;
    char *ts = NULL;

    for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"host") == 0) {
                xmlNode *child_node = cur_node->children;
                host = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"path") == 0) {
                xmlNode *child_node = cur_node->children;
                path = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"s") == 0) {
                xmlNode *child_node = cur_node->children;
                sign = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"ts") == 0) {
                xmlNode *child_node = cur_node->children;
                ts = (char *)xmlNodeGetContent(child_node);
            }
        }
    }

    if (host && path) {
        sprintf(*rresult, "https://%s/get-mp3/%s/%s%s", host, sign, ts, path);
    }

    xmlFree(host);
    xmlFree(path);
    xmlFree(sign);
    xmlFree(ts);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int
download_link(int trId, int alId, char **rresult){
    char *rresult2 = malloc(1);
    rresult2[0] = '\0';

    download_info(trId, alId, &rresult2);

    char *resp = malloc(1);
    resp[0] = '\0';

    CURLcode res = curl(rresult2, "y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ", &resp);

    if (res == CURLE_OK) {
        printf("Ответ на запрос:\n%s\n", resp);

        parseXML(resp, rresult);
            } else {
        return 2;
    }
    return 0;
}


