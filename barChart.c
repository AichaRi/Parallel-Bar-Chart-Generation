#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void generate_bar_chart(int *data, int size, int max_val, int rank, int num_procs) {
    int chunk = max_val / num_procs;
    int start = rank * chunk + 1;
    int end = (rank == num_procs - 1) ? max_val : start + chunk - 1;

    for (int i = start; i <= end; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (data[j] == i) {
                count++;
            }
        }

        // Critical section: synchronized output
        printf("Data Point %d: ", i);
        for (int k = 0; k < count; k++) {
            printf("*");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size, max_val = 0;
    int *data = NULL;

    if (rank == 0) {
        printf("Enter the dataset size: ");
        scanf("%d", &size);

        data = malloc(size * sizeof(int));

        printf("Enter the dataset values (separated by spaces): ");
        for (int i = 0; i < size; i++) {
            scanf("%d", &data[i]);
            if (data[i] > max_val) {
                max_val = data[i];
            }
        }
    }

    // Broadcast size and max_val to all processes
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        data = malloc(size * sizeof(int));
    }

    // Broadcast dataset to all processes
    MPI_Bcast(data, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process generates part of the bar chart
    generate_bar_chart(data, size, max_val, rank, num_procs);

    free(data);
    MPI_Finalize();
    return 0;
}
