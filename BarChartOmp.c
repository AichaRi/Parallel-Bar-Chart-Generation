#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// testing push & pull

int main() {
    int size, num_threads;
    printf("Enter the dataset size: ");
    scanf("%d", &size);

    int* data = (int*)malloc(size * sizeof(int));
    printf("Enter the dataset values (separated by spaces): ");
    for (int i = 0; i < size; i++) {
        scanf("%d", &data[i]);
    }

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    printf("--- Bar chart ---\n");

    int max_value = 0;
    #pragma omp parallel for reduction(max:max_value) num_threads(num_threads)
    for (int i = 0; i < size; i++) {
        if (data[i] > max_value) {
            max_value = data[i];
        }
    }

    
    #pragma omp parallel for num_threads(num_threads)
    for (int value = 1; value <= max_value; value++) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] == value) {
            count++;
        }
    }
    // Always print the Data Point
    printf("Data Point %d: ", value);
    for (int j = 0; j < count; j++) {
        printf("*");
    }
    printf("\n");  // New line after each data point
}


    free(data);
    return 0;
}
