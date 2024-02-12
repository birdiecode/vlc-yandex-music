#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура с тремя строковыми полями
typedef struct {
    char *name;
    char *surname;
    char *address;
} Person;

// Функция, возвращающая структуру Person
Person getPerson() {
    Person person;

    // Инициализируем поля структуры
    person.name = strdup("Alice");
    person.surname = strdup("Smith");
    person.address = strdup("123 Main Street");

    return person;
}

int main() {
    Person p = getPerson();

    // Выводим поля структуры
    printf("Name: %s\n", p.name);
    printf("Surname: %s\n", p.surname);
    printf("Address: %s\n", p.address);

    // Освобождаем память из-под строковых полей
    free(p.name);
    free(p.surname);
    free(p.address);

    return 0;
}
