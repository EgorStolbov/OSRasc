#include <stdio.h>
#include <stdlib.h>

// Считываем матрицу из файла и записываем ее размер (предполагается, что она квадратная)
double** readMatrix(const char* filename, int* size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return NULL;
    }

    *size = 0;
    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer), file)) {
        (*size)++;
    }
    rewind(file); // Переводим курсор в начало файла
    // Выделяем память
    double **matrix = (double**)malloc(*size * sizeof(double*));
    for (int i = 0; i < *size; i++) {
        matrix[i] = (double*)malloc(*size * sizeof(double));
    }
    // Заполняем выделенную память
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            if (fscanf(file, "%lf,", &matrix[i][j]) != 1) {
                printf("Ошибка чтения данных\n");
                fclose(file);
                return NULL;
            }
        }
    }
    
    fclose(file);
    return matrix;
}

// Функция для очистки матрицы
void freeMatrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Фунукция для создания минора
double** createMinor(double** matrix, int size, int row, int col) {
    int minor_size = size - 1;
    double** minor = (double**)malloc(minor_size * sizeof(double*));
    for (int i = 0; i < minor_size; i++) {
        minor[i] = (double*)malloc(minor_size * sizeof(double));
    }
    
    int minor_i = 0;
    for (int i = 0; i < size; i++) {
        if (i == row) continue;
        
        int minor_j = 0;
        for (int j = 0; j < size; j++) {
            if (j == col) continue;
            
            minor[minor_i][minor_j] = matrix[i][j];
            minor_j++;
        }
        minor_i++;
    }
    
    return minor;
}

// Рекурсивная функция вычисления детерминанта (определителя)
double determinant(double** matrix, int size) {
    if (size == 1) {
        return matrix[0][0];
    }
    
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    double det = 0;
    double sign = 1;
    
    for (int j = 0; j < size; j++) {
        double** minor = createMinor(matrix, size, 0, j);
        det += sign * matrix[0][j] * determinant(minor, size - 1);
        freeMatrix(minor, size - 1);
        sign = -sign;
    }
    
    return det;
}

int main() {
    int size;
    double** matrix = readMatrix("matrix.txt", &size);
    
    if (matrix != NULL) {
        printf("Матрица %dx%d:\n", size, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%.3lf ", matrix[i][j]);
            }
            printf("\n");
        }
        double det = determinant(matrix, size);
        printf("\nОпределитель матрицы: %lf\n", det);
        freeMatrix(matrix, size);
    }
    
    return 0;
}
