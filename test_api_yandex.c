#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

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

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "X-Yandex-Music-Client: YandexMusicAndroid/24023621");
        chunk = curl_slist_append(chunk, "Authorization: OAuth y0_AgAAAAArjp76AAG8XgAAAAD6t4__AABAmqc_jQVIYb0oViv72HnhkSl5QQ");
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



int main() {
    char *resp = malloc(1);
    resp[0] = '\0';

    CURLcode res = curl("https://api.music.yandex.net/account/status", "", &resp);
    if (res == CURLE_OK){
        printf("Ответ на запрос:\n%s\n", resp);
    }
    free(resp);


    return 0;
}
