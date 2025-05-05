// // MPI Parallel Bar Chart Generation
// #include <stdio.h>
// #include <stdlib.h>//for generaring random numbers
// #include <mpi.h>
// #include <string.h>

// // Function to generate bar chart segment for a process's portion of the array
// void generateBarChartSegment(int* values, int start, int end, int rank) {
//     for (int i = start; i < end; i++) {
//         printf("Data Point %d (Process %d): ", i+1, rank);
        
//         // Print asterisks based on value
//         for (int j = 0; j < values[i]; j++) {
//             printf("*");
//         }
//         printf("\n");
//     }
// }

// // Function to generate random values for large array
// void generateLargeArray(int* array, int size, int max_value) {
//     for (int i = 0; i < size; i++) {
//         array[i] = rand() % max_value + 1;
//     }
// }

// int main(int argc, char** argv) {
//     int rank, size, *values = NULL, max_value;
//     char choice;
//     int desired_num_processes = 0; // User's desired number of processes
//     int actual_num_processes = 0;  // Actual number of processes available
    
//     // Initialize MPI
//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &actual_num_processes);//we would like to know the number of CPU's available in this PC
    
//     // Only rank 0 handles user input
//     if (rank == 0) {
//         setbuf(stdout, NULL);//because sometimes the output gets swallowed for some c reason 

//         // Seed random number generator
//         srand(time(NULL));

//         // Ask user for desired number of processes used
//         printf("How many processes would you like to use? (max available: %d): ", actual_num_processes);
//         scanf("%d", &desired_num_procs);

// // -------------------------------------- SMALL ARRAY SECTION ----------------------------------------------

//         printf("\n--- SMALL ARRAY TEST ---\n");             

        
//         // Validate user input
//         if (desired_num_procs <= 0 || desired_num_procs > actual_num_processes) {
//             printf("Invalid number of processes. Using all available (%d) processes.\n", actual_num_processes);
//             desired_num_procs = actual_num_processes;
//         }
        

//         // First Small array input from the user
//         printf("Enter the dataset size: ");
//         scanf("%d", &size);
            
//         values = (int*)malloc(size * sizeof(int));
            
//         printf("Enter the dataset values (separated by spaces): ");
//         for (int i = 0; i < size; i++) {
//             scanf("%d", &values[i]);
//             }

//         // Start timing the parallel execution
//         double start_time = MPI_Wtime();

//         // Each process generates part of the bar chart
//         generate_bar_chart_MPI(data, size, max_val, rank, num_procs);

//         // End timing the parallel execution
//         double end_time = MPI_Wtime();


//             // Large array test for performance measurement
//             printf("Enter the size of large array: ");
//             scanf("%d", &size);
            
//             printf("Enter maximum value: ");
//             scanf("%d", &max_value);
            
//             values = (int*)malloc(size * sizeof(int));
//             generateLargeArray(values, size, max_value);
//         }
//     }
    
//     // Broadcast user selections to all processes
//     MPI_Bcast(&choice, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&desired_num_procs, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

//      // ---------------- Why??? ----------------
   
//     // Allocate memory for values on non-root processes
//     if (rank != 0) {
//         values = (int*)malloc(size * sizeof(int));
//     }
    
//     // Broadcast values array to all processes
//     MPI_Bcast(values, size, MPI_INT, 0, MPI_COMM_WORLD);
    
//     // Check if this process should participate based on user-specified count
//     if (rank >= desired_num_procs) {
//         free(values);
//         MPI_Finalize();
//         return 0;
//     }
    
//     // Calculate workload distribution among active processes
//     int active_procs = desired_num_procs; // Number of processes actually working
//     int items_per_proc = size / active_procs;
//     int remainder = size % active_procs;
    
//     int start_idx = rank * items_per_proc + (rank < remainder ? rank : remainder);
//     int end_idx = start_idx + items_per_proc + (rank < remainder ? 1 : 0);
    
//     // Measure time
//     double start_time, end_time, local_time, global_time;
    
//     // Synchronize all active processes
//     MPI_Barrier(MPI_COMM_WORLD);
    
//     start_time = MPI_Wtime();
    
//     if (rank == 0) {
//         printf("--- Bar chart (MPI with %d processes) ---\n", active_procs);
//     }
    
//     // Each process generates its part of the bar chart
//     generateBarChartSegment(values, start_idx, end_idx, rank);
    
//     end_time = MPI_Wtime();
//     local_time = end_time - start_time;
    
//     // Get the maximum time across all active processes
//     MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
//     if (rank == 0) {
//         printf("Execution time with %d processes: %f seconds\n", active_procs, global_time);
        
//         // If it's a large array test, run multiple times for timing
//         if (choice == 'L' || choice == 'l') {
//             double total_time = global_time;
//             int runs = 9;  // Already did one run
            
//             printf("Running additional %d times for timing...\n", runs);
            
//             for (int run = 0; run < runs; run++) {
//                 MPI_Barrier(MPI_COMM_WORLD);
                
//                 start_time = MPI_Wtime();
//                 generateBarChartSegment(values, start_idx, end_idx, rank);
//                 end_time = MPI_Wtime();
                
//                 local_time = end_time - start_time;
//                 MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
                
//                 total_time += global_time;
//             }
            
//             printf("Average execution time with %d processes: %f seconds\n", active_procs, total_time / (runs + 1));
            
//             // If testing large arrays, perform testing with various process counts
//             if (active_procs == actual_num_processes) {
//                 int test_procs[] = {2, 4, 8, 16};
                
//                 for (int p = 0; p < 4; p++) {
//                     // Skip if we don't have enough processes or already tested this count
//                     if (test_procs[p] > actual_num_processes || test_procs[p] == active_procs) {
//                         continue;
//                     }
                    
//                     if (rank == 0) {
//                         printf("\nTesting with %d processes:\n", test_procs[p]);
//                     }
                    
//                     // Skip if this process shouldn't participate
//                     if (rank >= test_procs[p]) {
//                         continue;
//                     }
                    
//                     // Recalculate workload distribution for this test
//                     items_per_proc = size / test_procs[p];
//                     remainder = size % test_procs[p];
                    
//                     start_idx = rank * items_per_proc + (rank < remainder ? rank : remainder);
//                     end_idx = start_idx + items_per_proc + (rank < remainder ? 1 : 0);
                    
//                     // Run 10 times and calculate average
//                     double test_total_time = 0.0;
                    
//                     for (int run = 0; run < 10; run++) {
//                         MPI_Barrier(MPI_COMM_WORLD);
                        
//                         start_time = MPI_Wtime();
//                         generateBarChartSegment(values, start_idx, end_idx, rank);
//                         end_time = MPI_Wtime();
                        
//                         local_time = end_time - start_time;
//                         MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
                        
//                         if (rank == 0) {
//                             test_total_time += global_time;
//                         }
//                     }
                    
//                     if (rank == 0) {
//                         printf("Average execution time with %d processes: %f seconds\n", 
//                                test_procs[p], test_total_time / 10);
//                     }
//                 }
//             }
//         }
//     } else {
//         // Non-root processes participate in the additional runs
//         if (choice == 'L' || choice == 'l') {
//             for (int run = 0; run < 9; run++) {
//                 MPI_Barrier(MPI_COMM_WORLD);
                
//                 start_time = MPI_Wtime();
//                 generateBarChartSegment(values, start_idx, end_idx, rank);
//                 end_time = MPI_Wtime();
                
//                 local_time = end_time - start_time;
//                 MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//             }
            
//             // Participate in tests with various process counts
//             if (active_procs == actual_num_processes) {
//                 int test_procs[] = {2, 4, 8, 16};
                
//                 for (int p = 0; p < 4; p++) {
//                     // Skip if we don't have enough processes or already tested this count
//                     if (test_procs[p] > actual_num_processes || test_procs[p] == active_procs) {
//                         continue;
//                     }
                    
//                     // Skip if this process shouldn't participate
//                     if (rank >= test_procs[p]) {
//                         continue;
//                     }
                    
//                     // Recalculate workload distribution for this test
//                     items_per_proc = size / test_procs[p];
//                     remainder = size % test_procs[p];
                    
//                     start_idx = rank * items_per_proc + (rank < remainder ? rank : remainder);
//                     end_idx = start_idx + items_per_proc + (rank < remainder ? 1 : 0);
                    
//                     for (int run = 0; run < 10; run++) {
//                         MPI_Barrier(MPI_COMM_WORLD);
                        
//                         start_time = MPI_Wtime();
//                         generateBarChartSegment(values, start_idx, end_idx, rank);
//                         end_time = MPI_Wtime();
                        
//                         local_time = end_time - start_time;
//                         MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//                     }
//                 }
//             }
//         }
//     }
    
//     free(values);
//     MPI_Finalize();
//     return 0;


// /*
//  * How to compile and run the MPI program:
//  * 
//  * 1. Compile with: 
//  mpicc -o mpi_barchart mpi_barchart.c
//  * 2. Run with: mpirun -np <max_processes> ./mpi_barchart
//  * mpirun -np 8 ./mpi_barchart
//  mpirun -np 2 ./mpi_barchart
//  * Note: The actual number of processes used will be determined at runtime
//  * based on user input, up to the maximum specified with -np
//  */