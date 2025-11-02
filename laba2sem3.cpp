#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

void shell(int* items, int count)
{
    int i, j, gap, k;
    int x, a[5];
    a[0] = 9; a[1] = 5; a[2] = 3; a[3] = 2; a[4] = 1;
    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (j >= 0) && (x < items[j]); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right)
{
    int i, j;
    int x, y;
    i = left; j = right;
    x = items[(left + right) / 2];
    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left)) j--;
        if (i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while (i <= j);
    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

void quick_sort_wrapper(int* items, int count) {
    qs(items, 0, count - 1);
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void generate_random(int* arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000;
}

void generate_ascending(int* arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
}

void generate_descending(int* arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = n - i - 1;
}

void generate_mixed(int* arr, int n) {
    int half = n / 2;
    for (int i = 0; i < half; i++)
        arr[i] = i;
    for (int i = half; i < n; i++)
        arr[i] = n - i + half - 1;
}

int is_sorted(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

void test_sort_algorithm(const char* name, void (*sort_func)(int*, int),
    int* original, int n, const char* data_type) {
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    memcpy(arr, original, n * sizeof(int));

    clock_t start = clock();
    sort_func(arr, n);
    clock_t end = clock();

    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  %-15s | %-12s | %9.6f сек | %s\n",
        name, data_type, time, is_sorted(arr, n) ? "OK" : "ERROR");

    free(arr);
}

void test_std_qsort(int* original, int n, const char* data_type) {
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    memcpy(arr, original, n * sizeof(int));

    clock_t start = clock();
    qsort(arr, n, sizeof(int), compare);
    clock_t end = clock();

    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  %-15s | %-12s | %9.6f сек | %s\n",
        "Std QSort", data_type, time, is_sorted(arr, n) ? "OK" : "ERROR");

    free(arr);
}

void run_sort_tests() {
    printf("\n=== ТЕСТИРОВАНИЕ АЛГОРИТМОВ СОРТИРОВКИ ===\n");

    const int sort_sizes[] = { 100, 200, 400, 1000, 2000 , 4000, 10000 };
    const int num_sort_sizes = sizeof(sort_sizes) / sizeof(sort_sizes[0]);

    printf("\nАЛГОРИТМ         | ТИП ДАННЫХ   | ВРЕМЯ       | РЕЗУЛЬТАТ\n");
    printf("-----------------|-------------|-------------|----------\n");

    for (int s = 0; s < num_sort_sizes; s++) {
        int n = sort_sizes[s];
        printf("\nРазмер массива: %d\n", n);

        int* arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("Ошибка выделения памяти для массива размера %d!\n", n);
            continue;
        }

        printf("--- Случайный массив ---\n");
        generate_random(arr, n);
        test_sort_algorithm("Shell Sort", shell, arr, n, "Случайный");
        test_sort_algorithm("Quick Sort", quick_sort_wrapper, arr, n, "Случайный");
        test_std_qsort(arr, n, "Случайный");

        printf("--- Возрастающий массив ---\n");
        generate_ascending(arr, n);
        test_sort_algorithm("Shell Sort", shell, arr, n, "Возрастающий");
        test_sort_algorithm("Quick Sort", quick_sort_wrapper, arr, n, "Возрастающий");
        test_std_qsort(arr, n, "Возрастающий");


        printf("--- Убывающий массив ---\n");
        generate_descending(arr, n);
        test_sort_algorithm("Shell Sort", shell, arr, n, "Убывающий");
        test_sort_algorithm("Quick Sort", quick_sort_wrapper, arr, n, "Убывающий");
        test_std_qsort(arr, n, "Убывающий");

        printf("--- Смешанный массив ---\n");
        generate_mixed(arr, n);
        test_sort_algorithm("Shell Sort", shell, arr, n, "Смешанный");
        test_sort_algorithm("Quick Sort", quick_sort_wrapper, arr, n, "Смешанный");
        test_std_qsort(arr, n, "Смешанный");

        free(arr);
    }
}

// ТЕСТИРОВАНИЕ УМНОЖЕНИЯ МАТРИЦ

void run_matrix_tests() {
    printf("\n=== ТЕСТИРОВАНИЕ УМНОЖЕНИЯ МАТРИЦ ===\n");

    int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n%-10s | %-12s | %-s\n", "Размер", "Время (сек)", "Результат");
    printf("-----------|------------|----------\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("%-10d | ", n);

        int** a = (int**)malloc(n * sizeof(int*));
        int** b = (int**)malloc(n * sizeof(int*));
        int** c = (int**)malloc(n * sizeof(int*));

        if (a == NULL || b == NULL || c == NULL) {
            printf("Ошибка выделения памяти для матриц размера %d!\n", n);
            free(a); free(b); free(c);
            continue;
        }

        int memory_error = 0;
        for (int i = 0; i < n; i++) {
            a[i] = (int*)malloc(n * sizeof(int));
            b[i] = (int*)malloc(n * sizeof(int));
            c[i] = (int*)malloc(n * sizeof(int));

            if (a[i] == NULL || b[i] == NULL || c[i] == NULL) {
                memory_error = 1;
                for (int j = 0; j <= i; j++) {
                    free(a[j]); free(b[j]); free(c[j]);
                }
                break;
            }
        }

        if (memory_error) {
            printf("Ошибка выделения памяти для строк матриц!\n");
            free(a); free(b); free(c);
            continue;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = rand() % 100 + 1;
                b[i][j] = rand() % 100 + 1;
            }
        }

        clock_t start, end;
        start = clock();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int elem_c = 0;
                for (int r = 0; r < n; r++) {
                    elem_c += a[i][r] * b[r][j];
                }
                c[i][j] = elem_c;
            }
        }

        end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("%-10.3f | ", cpu_time_used);

        int valid = 1;
        if (n > 0) {
            int non_zero_found = 0;
            for (int i = 0; i < n && i < 5; i++) {
                for (int j = 0; j < n && j < 5; j++) {
                    if (c[i][j] != 0) {
                        non_zero_found = 1;
                        break;
                    }
                }
                if (non_zero_found) break;
            }
            valid = non_zero_found;
        }

        printf("%s\n", valid ? "OK" : "ERROR");

        for (int i = 0; i < n; i++) {
            free(a[i]);
            free(b[i]);
            free(c[i]);
        }
        free(a);
        free(b);
        free(c);
    }
}

int main(void) {
    setlocale(LC_ALL, "Ru");
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    srand((unsigned int)time(NULL));

    int choice;
    do {
        printf("\nВЫБЕРИТЕ ТЕСТ:\n");
        printf("1 - Тестирование умножения матриц\n");
        printf("2 - Тестирование алгоритмов сортировки\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            run_matrix_tests();
            break;
        case 2:
            run_sort_tests();
            break;
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 0);

    return 0;
}
