
// to compile and run this code 👇🏻
// gcc -o SequentialBarChart SequentialBarChart.c        (Compile)
// SequentialBarChart.exe (for windows)                  (Run)
// ./SequentialBarChart   (for mac)                      (Run)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // included to measure time using clock() function

// Sequential Function
void generate_bar_chart(int* data, int size) {
    printf("\n--- Bar chart ---\n");
    for (int i = 0; i < size; i++) {
        printf("Data Point %d: ", i + 1);
        for (int j = 0; j < data[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    int size;
    printf("Enter the dataset size: ");
    scanf("%d", &size);

    int* data = (int*)malloc(size * sizeof(int));
    if (!data) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter the dataset values (separated by spaces): ");
    for (int i = 0; i < size; i++) {
        scanf("%d", &data[i]);
    }

    clock_t start = clock();        // start measuring function time
    generate_bar_chart(data, size);
    clock_t end = clock();          // end measuring function time

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime taken to generate bar chart in Sequential: %.6f seconds\n", time_taken);

    free(data);

    return 0;
}
