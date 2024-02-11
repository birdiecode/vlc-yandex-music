#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

// Функция обратного вызова для записи данных в переменную
size_t write_callback(void *ptr, size_t size, size_t nmemb, char **data) {
    size_t data_size = size * nmemb;
    *data = realloc(*data, strlen(*data) + data_size + 1);
    if (*data) {
        strncat(*data, ptr, data_size);
    }
    return data_size;
}

int main() {
    CURL *curl;
    CURLcode res;
    char url[] = "https://jsonplaceholder.typicode.com/posts/1";
    char *response = malloc(1); // Выделяем память под один байт для начала
    response[0] = '\0'; // Устанавливаем нулевой символ в начале строки

    curl = curl_easy_init();
    if (curl) {
        // Устанавливаем URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Определяем функцию обратного вызова для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Выполняем запрос
        res = curl_easy_perform(curl);

        // Выводим ответ на экран
        if (res == CURLE_OK) {
            printf("Ответ на запрос:\n%s\n", response);
        } else {
            printf("Ошибка при выполнении запроса: %s\n", curl_easy_strerror(res));
        }

        // Освобождаем ресурсы
        curl_easy_cleanup(curl);
        free(response); // Освобождаем выделенную память
    }

    return 0;
}
