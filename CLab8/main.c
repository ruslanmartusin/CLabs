#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Функция для проверки корректности ввода
int input(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        char b;
        if (scanf("%d%c", &value, &b) == 2 && b == '\n') {
            break;
        } else {
            printf("Incorrect input. Try again\n");
            while (getchar() != '\n'); // Очистка буфера ввода
        }
    }
    return value;
}

// Функция для проверки, является ли число простым
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Функция для замены знака числа на противоположный
void negate_numbers(const char *filename, int count) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int number;
    for (int i = 0; i < count; i++) {
        fread(&number, sizeof(int), 1, file);
        number = -number;
        fseek(file, -sizeof(int), SEEK_CUR);
        fwrite(&number, sizeof(int), 1, file);
    }

    fclose(file);
}

// Функция для сортировки отрицательных чисел по убыванию
void sort_negatives(const char *filename, int count) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int *numbers = malloc(count * sizeof(int));
    fread(numbers, sizeof(int), count, file);

    // Сортировка отрицательных чисел по убыванию
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0) {
            for (int j = i + 1; j < count; j++) {
                if (numbers[j] < 0 && numbers[j] > numbers[i]) {
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }
            }
        }
    }

    fseek(file, 0, SEEK_SET);
    fwrite(numbers, sizeof(int), count, file);

    free(numbers);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int count;

    // Ввод количества чисел с проверкой
    count = input("Enter the number of integers: ");

    // Запись чисел в файл
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("Enter %d integers:\n", count);
    for (int i = 0; i < count; i++) {
        int number = input(""); // Ввод каждого числа с проверкой
        fwrite(&number, sizeof(int), 1, file);
    }

    fclose(file);

    // Вывод данных из файла
    file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("Numbers in the file:\n");
    int number;
    while (fread(&number, sizeof(int), 1, file)) {
        printf("%d ", number);
    }
    printf("\n");

    fclose(file);

    // Подсчет простых чисел
    file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    int prime_count = 0;
    while (fread(&number, sizeof(int), 1, file)) {
        if (is_prime(number)) {
            prime_count++;
        }
    }

    printf("Number of prime numbers: %d\n", prime_count);

    fclose(file);

    // Замена знака каждого числа на противоположный
    negate_numbers(filename, count);

    // Вывод данных после замены знака
    file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("Numbers after negating:\n");
    while (fread(&number, sizeof(int), 1, file)) {
        printf("%d ", number);
    }
    printf("\n");

    fclose(file);

    // Сортировка отрицательных чисел по убыванию
    sort_negatives(filename, count);

    // Вывод данных после сортировки
    file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("Numbers after sorting negatives:\n");
    while (fread(&number, sizeof(int), 1, file)) {
        printf("%d ", number);
    }
    printf("\n");

    fclose(file);

    return EXIT_SUCCESS;
}