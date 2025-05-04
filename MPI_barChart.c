/*
mpicc MPI_barChart.c -o MPI_barChart
mpirun -np 4 ./MPI_barChart
*/


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Function to generate bar chart using MPI
void generate_bar_chart_MPI(int *data, int size, int max_val, int rank, int active_procs) {
    // Divide the value range across the available processes
    int chunk = max_val / active_procs;
    int start = rank * chunk + 1;
    int end = (rank == active_procs - 1) ? max_val : start + chunk - 1;

    // Count and print occurrences of each number in the assigned range
    for (int i = start; i <= end; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (data[j] == i) {
                count++;
            }
        }

        // Print the histogram bar for each number
        printf("Data Point %d: ", i);
        for (int k = 0; k < count; k++) {
            printf("*");
        }
        printf("\n");
    }
}

// Generate a random dataset of integers from 1 to max_val
void generate_random_data(int *data, int size, int max_val) {
    for (int i = 0; i < size; i++) {
        data[i] = rand() % max_val + 1;
    }
}

int main(int argc, char *argv[]) {
    int rank, num_procs, active_procs;
    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // Get total number of processes

    int size = 0, max_val = 0;
    int *data = NULL;

    if (rank == 0) {
        setbuf(stdout, NULL); // Disable output buffering (helps in ordered printing)
        srand(time(NULL)); // Seed random number generator

        // Prompt user for number of MPI processes to use
        printf("Available MPI processes: %d\n", num_procs);
        printf("Enter how many processes to use (1 to %d): ", num_procs);
        scanf("%d", &active_procs);

        // Validate number of processes
        if (active_procs > num_procs || active_procs < 1) {
            printf("Invalid input. Using maximum available (%d).\n", num_procs);
            active_procs = num_procs;
        }

        // Get dataset size and values from user
        printf("Enter the dataset size: ");
        scanf("%d", &size);
        data = malloc(size * sizeof(int));

        printf("Enter the dataset values (space-separated): ");
        for (int i = 0; i < size; i++) {
            scanf("%d", &data[i]);
            if (data[i] > max_val) {
                max_val = data[i]; // Determine maximum value in dataset
            }
        }
    }

    // Broadcast user-input test parameters
    MPI_Bcast(&active_procs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory on other processes
    if (rank != 0) {
        data = malloc(size * sizeof(int));
    }

    // Broadcast dataset to all processes
    MPI_Bcast(data, size, MPI_INT, 0, MPI_COMM_WORLD);

    // --- First Test: User Input ---
    double start_time = MPI_Wtime(); // Start timing
    if (rank < active_procs) {
        generate_bar_chart_MPI(data, size, max_val, rank, active_procs);
    }
    double end_time = MPI_Wtime(); // End timing
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes

    if (rank == 0) {
        printf("Time taken for user-input bar chart: %f seconds\n\n", end_time - start_time);
    }

    // --- Second Test: Large Random Dataset ---
    const int large_size = 100;
    const int large_max_val = 20;

    if (rank == 0) {
        free(data); // Free previous dataset
        data = malloc(large_size * sizeof(int));
        generate_random_data(data, large_size, large_max_val);

        // Print generated dataset
        printf("\nGenerated random dataset of size %d:\n", large_size);
        for (int i = 0; i < large_size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    // Reuse 'size' and 'max_val' variables for new dataset
    size = large_size;
    max_val = large_max_val;

    // Broadcast new dataset size and max_val
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate and broadcast new dataset
    if (rank != 0) {
        free(data);
        data = malloc(size * sizeof(int));
    }
    MPI_Bcast(data, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Run bar chart generation for random dataset
    start_time = MPI_Wtime();
    if (rank < active_procs) {
        generate_bar_chart_MPI(data, size, max_val, rank, active_procs);
    }
    end_time = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Time taken for random dataset bar chart: %f seconds\n", end_time - start_time);
    }

    // Cleanup
    free(data);
    MPI_Finalize();
    return 0;
}
