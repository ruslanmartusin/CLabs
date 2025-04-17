/*
Задание:
Структура содержит информацию об автомобилях: стоимость (число), марку (указатель),
вложенную структуру – тип кузова (строка фиксированной длины) и объём двигателя.
Найти автомобили со стоимостью больше заданной. Удалить автомобили заданной марки.

Фамилия студента: [Мартусин]
Дата написания кода: [15.04.25]
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


void print_twos_complement() {
    char num;
    printf("Enter a char number: ");
    scanf("%hhd", &num);

    printf("Two's complement: ");
    for (int i = CHAR_BIT - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

typedef struct {
    char body_type[20];
    float engine_volume;
} CarDetails;

typedef struct {
    int price;
    char *brand;
    CarDetails details;
} Car;

void input_car_basic(Car *car) {
    printf("Enter car price: ");
    while (scanf("%d", &car->price) != 1 || car->price <= 0) {
        printf("Error! Enter a positive number for price: ");
        while (getchar() != '\n'); // Clear buffer
    }

    printf("Enter car brand: ");
    char buffer[100];
    scanf("%s", buffer);
    car->brand = (char *)malloc(strlen(buffer) + 1);
    if (car->brand == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    strcpy(car->brand, buffer);
}

void print_car_basic(const Car *car) {
    printf("Brand: %s, Price: %d\n", car->brand, car->price);
}

// Function for complete car data input
void input_car(Car *car) {
    input_car_basic(car);

    printf("Enter body type: ");
    scanf("%s", car->details.body_type);

    printf("Enter engine volume: ");
    while (scanf("%f", &car->details.engine_volume) != 1 || car->details.engine_volume <= 0) {
        printf("Error! Enter a positive number for engine volume: ");
        while (getchar() != '\n');
    }
}

void print_car(const Car *car) {
    printf("Brand: %s, Price: %d, Body type: %s, Engine volume: %.1f\n",
           car->brand, car->price, car->details.body_type, car->details.engine_volume);
}

void find_cars_above_price(const Car *cars, int count, int min_price) {
    printf("Cars more expensive than %d:\n", min_price);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (cars[i].price > min_price) {
            print_car(&cars[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No cars more expensive than the given price.\n");
    }
}

int remove_cars_by_brand(Car *cars, int count, const char *brand) {
    int new_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(cars[i].brand, brand) != 0) {
            cars[new_count] = cars[i];
            new_count++;
        } else {
            free(cars[i].brand);
        }
    }
    return new_count;
}

void add_car(Car **cars, int *count) {
    *cars = (Car *)realloc(*cars, (*count + 1) * sizeof(Car));
    if (*cars == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    input_car(&(*cars)[*count]);
    (*count)++;
}

void car_menu() {
    Car *cars = NULL;
    int count = 0;
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. View all cars\n");
        printf("2. Add a car\n");
        printf("3. Find cars more expensive than given price\n");
        printf("4. Remove cars by brand\n");
        printf("0. Exit\n");
        printf("Choose an action: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (count == 0) {
                    printf("No cars in the database.\n");
                } else {
                    printf("All cars:\n");
                    for (int i = 0; i < count; i++) {
                        print_car(&cars[i]);
                    }
                }
                break;

            case 2:
                add_car(&cars, &count);
                break;

            case 3: {
                int min_price;
                printf("Enter minimum price: ");
                scanf("%d", &min_price);
                find_cars_above_price(cars, count, min_price);
                break;
            }

            case 4: {
                char brand[100];
                printf("Enter brand to remove: ");
                scanf("%s", brand);
                int new_count = remove_cars_by_brand(cars, count, brand);
                if (new_count == count) {
                    printf("No cars of the specified brand found.\n");
                } else {
                    printf("Removed %d cars of brand %s.\n", count - new_count, brand);
                    count = new_count;
                }
                break;
            }

            case 0:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);


    for (int i = 0; i < count; i++) {
        free(cars[i].brand);
    }
    free(cars);
}

int main() {
    print_twos_complement();

    car_menu();

    return 0;
}