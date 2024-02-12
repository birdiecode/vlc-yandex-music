#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

// gcc test_curl.c -lcurl -o foo

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
        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }
    return 0;
}

void dd(int user_id){


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
        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }
   // return 0;
}





int main() {
    account_status();
    dd(730767098);
    return 0;
}
