#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

//gcc test_api_yandex.c -lcurl -lcjson -o test_yandex_api

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



int account_status(){
    char *resp = malloc(1);
    resp[0] = '\0';

    CURLcode res = curl("https://api.music.yandex.net/account/status", "y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ", &resp);

    int userID = 0;

    if (res == CURLE_OK)
    {
        cJSON *root = cJSON_Parse(resp);
        if (root == NULL) {
            printf("Ошибка при парсинге JSON.\n");
            return -1;
        }

        cJSON *result = cJSON_GetObjectItem(root, "result");
        if (result == NULL) {
            printf("Отсутствует поле 'result' в JSON.\n");
            cJSON_Delete(root);
            return -2;
        }

        cJSON *account = cJSON_GetObjectItem(result, "account");
        if (account == NULL) {
            printf("Отсутствует поле 'account' в JSON.\n");
            cJSON_Delete(root);
            return -3;
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
    }
    else
    {
        printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
    }

    free(resp);

    return userID;
}



int main() {

    int gh = account_status();
    printf("ss %d", gh);
    return 0;
}
