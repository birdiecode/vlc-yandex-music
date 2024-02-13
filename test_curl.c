#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// gcc test_curl.c -lcurl -lcjson -o foo

// Функция обратного вызова для записи данных в переменную
size_t write_callback(void *ptr, size_t size, size_t nmemb, char **data) {
    size_t data_size = size * nmemb;
    *data = realloc(*data, strlen(*data) + data_size + 1);
    if (*data) {
        strncat(*data, ptr, data_size);
    }
    return data_size;
}

int account_status(){
    CURL *curl;
    CURLcode res;
    char url[] = "https://api.music.yandex.net/account/status";
    char *response = malloc(1);
    response[0] = '\0';
    int userID = 0;

    curl = curl_easy_init();
    if (curl) {

        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, "Authorization: OAuth y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ");
        chunk = curl_slist_append(chunk, "Accept-Language: ru");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            printf("Ответ на запрос:\n%s\n", response);

            cJSON *root = cJSON_Parse(response);
            if (root == NULL) {
                printf("Ошибка при парсинге JSON.\n");
                return 1;
            }

            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result == NULL) {
                printf("Отсутствует поле 'result' в JSON.\n");
                cJSON_Delete(root);
                return 1;
            }

            cJSON *account = cJSON_GetObjectItem(result, "account");
            if (account == NULL) {
                printf("Отсутствует поле 'account' в JSON.\n");
                cJSON_Delete(root);
                return 1;
            }

            cJSON *uid = cJSON_GetObjectItem(account, "uid");
            if (cJSON_IsNumber(uid)) {
                printf("Значение поля 'uid': %d\n", uid->valueint);
                userID = uid->valueint;

            } else {
                printf("Поле 'uid' отсутствует или не является числом.\n");
            }

            // Освобождаем память
            cJSON_Delete(root);
        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }
    return userID;
}

void like_track(int user_id){


    char base_url[] = "https://api.music.yandex.net/users/";
    char additional_url[] = "/likes/tracks";
    char url[100]; // Создаем массив для хранения итоговой строки

    // Конкатенация строк с использованием sprintf
    sprintf(url, "%s%d%s", base_url, user_id, additional_url);


    CURL *curl;
    CURLcode res;
    char *response = malloc(1);
    response[0] = '\0';

    curl = curl_easy_init();
    if (curl) {

        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, "Authorization: OAuth y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ");
        chunk = curl_slist_append(chunk, "Accept-Language: ru");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            printf("Ответ на запрос:\n%s\n", response);

            cJSON *root = cJSON_Parse(response);
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
            printf("Идентификаторы треков %d:\n", cJSON_GetArraySize(tracks));
            for (int i = 0; i < cJSON_GetArraySize(tracks); i++) {
                cJSON *track = cJSON_GetArrayItem(tracks, i);
                cJSON *track_id = cJSON_GetObjectItem(track, "id");
                cJSON *track_album_id = cJSON_GetObjectItem(track, "albumId");

                if (track_id != NULL && track_album_id != NULL) {
                    printf("%s\t%s\n", track_id->valuestring, track_album_id->valuestring);
                }
            }

            // Освобождаем память
            cJSON_Delete(root);


        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }
   // return 0;
}

typedef struct {
    char *title;
    char *album;
} TrackSt;

TrackSt fetch_track(int trId, int alId){
    CURL *curl;
    CURLcode res;
    char url[] = "https://api.music.yandex.net/tracks";
    char *response = malloc(1);
    response[0] = '\0';
    TrackSt trst;

    char trackids[25];

    char base_url[] = "track-ids=";

    // Конкатенация строк с использованием sprintf
    sprintf(trackids, "%s%d:%d", base_url, trId, alId);



    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, "Authorization: OAuth y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ");
        chunk = curl_slist_append(chunk, "Accept-Language: ru");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, trackids);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            printf("Ответ на запрос:\n%s\n", response);


            cJSON *root = cJSON_Parse(response);
            if (root == NULL) {
                printf("Ошибка при парсинге JSON.\n");
                return trst;
            }

            // Извлечение значения "title"
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result && cJSON_IsArray(result) && cJSON_GetArraySize(result) > 0) {
                cJSON *firstAlbum = cJSON_GetArrayItem(result, 0);
                cJSON *title = cJSON_GetObjectItem(firstAlbum, "title");
                if (title && cJSON_IsString(title)) {
                    printf("Значение поля 'title': %s\n", title->valuestring);
                    trst.title = strdup(title->valuestring);
                }
            }

            // Извлечение названия первого альбома
            if (result && cJSON_IsArray(result) && cJSON_GetArraySize(result) > 0) {
                cJSON *firstAlbum = cJSON_GetArrayItem(result, 0);
                cJSON *albums = cJSON_GetObjectItem(firstAlbum, "albums");
                if (albums && cJSON_IsArray(albums) && cJSON_GetArraySize(albums) > 0) {
                    cJSON *firstAlbumObject = cJSON_GetArrayItem(albums, 0);
                    cJSON *albumTitle = cJSON_GetObjectItem(firstAlbumObject, "title");
                    if (albumTitle && cJSON_IsString(albumTitle)) {
                        printf("Название первого альбома: %s\n", albumTitle->valuestring);
                        trst.album = strdup(albumTitle->valuestring);
                    }
                }
            }

            // Очистка памяти
            cJSON_Delete(root);



        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }


        curl_easy_cleanup(curl);
        free(response);
    }
    return trst;
}

void download_info(int trId, int alId){
    char base_url[] = "https://api.music.yandex.net/tracks/";
    char additional_url[] = "/download-info";
    char url[100]; // Создаем массив для хранения итоговой строки

    // Конкатенация строк с использованием sprintf
    sprintf(url, "%s%d:%d%s", base_url, trId, alId, additional_url);


    CURL *curl;
    CURLcode res;
    char *response = malloc(1);
    response[0] = '\0';

    curl = curl_easy_init();
    if (curl) {

        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, "Authorization: OAuth y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ");
        chunk = curl_slist_append(chunk, "Accept-Language: ru");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);


        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            printf("Ответ на запрос:\n%s\n", response);

            cJSON *root = cJSON_Parse(response);
            if (root == NULL) {
                printf("Ошибка при парсинге JSON.\n");
                return;
            }

            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result && cJSON_IsArray(result) && cJSON_GetArraySize(result) > 0) {
                cJSON *firstResultObj = cJSON_GetArrayItem(result, 0);

                // Извлекаем значение параметра downloadInfoUrl
                cJSON *downloadInfoUrl = cJSON_GetObjectItem(firstResultObj, "downloadInfoUrl");
                if (downloadInfoUrl && cJSON_IsString(downloadInfoUrl)) {
                    char *value = strdup(downloadInfoUrl->valuestring);
                    printf("запроса: %s\n", value);
                }
            }


            // Освобождаем память
            cJSON_Delete(root);


        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }
}

int main() {
    TrackSt trc = fetch_track(atoi("28423051"), atoi("3400046"));
    printf("Surname: %s\n", trc.title);
    printf("Address: %s\n", trc.album);
    download_info(atoi("28423051"), atoi("3400046"));

    // Освобождаем память из-под строковых полей
    free(trc.title);
    free(trc.album);
    // int ret = account_status();
    // if (ret != 0){
    //     printf("Useer ID: %d\n", ret);
    //     like_track(ret);
    // }
    return 0;
}
