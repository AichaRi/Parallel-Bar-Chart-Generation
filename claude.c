// // MPI (Message Passing Interface) Parallel Bar Chart Generation
// // This program demonstrates parallel processing using MPI to generate bar charts
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <mpi.h>
// #include <string.h>

// /**
//  * Function to generate a segment of the bar chart based on the process's assigned portion
//  * @param values - Array containing the data values
//  * @param start - Starting index for this process's portion
//  * @param end - Ending index (exclusive) for this process's portion
//  * @param rank - The process ID (rank) generating this segment
//  */
// void generateBarChartSegment(int* values, int start, int end, int rank) {
//     // Each process handles only its assigned portion of the array (from start to end-1)
//     for (int i = start; i < end; i++) {
//         printf("Data Point %d (Process %d): ", i+1, rank);
        
//         // Print asterisks to represent the value graphically
//         for (int j = 0; j < values[i]; j++) {
//             printf("*");
//         }
//         printf("\n");
//     }
// }

// /**
//  * Function to generate random values for a large array
//  * @param array - Pointer to the array to fill with random values
//  * @param size - Size of the array
//  * @param max_value - Maximum value for the random numbers
//  */
// void generateLargeArray(int* array, int size, int max_value) {
//     for (int i = 0; i < size; i++) {
//         array[i] = rand() % max_value + 1; // Generate random values between 1 and max_value
//     }
// }

// int main(int argc, char** argv) {
//     // Variables for MPI management
//     int rank;          // Process ID (rank) - unique identifier for each process
//     int size;          // Total number of processes
//     int* values = NULL; // Array to hold the data values
//     double start_time, end_time, local_time, global_time; // For timing measurements
    
//     // ======= INITIALIZE MPI ENVIRONMENT =======
//     // MPI can only be initialized ONCE per program execution
//     MPI_Init(&argc, &argv);
    
//     // Get this process's rank (ID) - each process gets a unique number from 0 to (size-1)
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
//     // Get the total number of processes
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
    
//     // Process 0 will handle random seed initialization
//     if (rank == 0) {
//         srand(time(NULL)); // Initialize random number generator
//     }
    
//     // ============= PART 1: USER INPUT ARRAY PROCESSING =============
    
//     int user_array_size = 0;     // Size of the user input array
//     int desired_procs_part1 = 0; // User's desired number of processes for part 1
    
//     // Only process 0 handles user input
//     if (rank == 0) {

    
//         setbuf(stdout, NULL);//because sometimes the output gets swallowed for some c reason 
//         printf("\n===== PART 1: USER INPUT ARRAY =====\n\n");
//         setbuf(stdout, NULL);//because sometimes the output gets swallowed for some c reason 

//         // Ask user for desired number of processes
//         printf("How many processes would you like to use? (max available: %d): ", size);
//         scanf("%d", &desired_procs_part1);
        
//         // Validate user input for process count
//         if (desired_procs_part1 <= 0 || desired_procs_part1 > size) {
//             printf("Invalid number of processes. Using all available (%d) processes.\n", size);
//             desired_procs_part1 = size;
//         }
        
//         // Get array size from user
//         printf("Enter the dataset size: ");
//         scanf("%d", &user_array_size);
        
//         // Allocate memory for the array
//         values = (int*)malloc(user_array_size * sizeof(int));
        
//         // Get values from user
//         printf("Enter the dataset values (separated by spaces): ");
//         for (int i = 0; i < user_array_size; i++) {
//             scanf("%d", &values[i]);
//         }
//     }
    
//     // Broadcast configuration to all processes, tell them the nessary info
//     MPI_Bcast(&desired_procs_part1, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&user_array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
//     // Allocate memory for non-root processes 
//     // let each process have an array of the same size
//     if (rank != 0) {
//         values = (int*)malloc(user_array_size * sizeof(int));
//     }
    
//     // Now that we have values array allocated in each process, broadcast the actual array values
//     MPI_Bcast(values, user_array_size, MPI_INT, 0, MPI_COMM_WORLD);
    
//     // Calculate work distribution for active processes
//     int active_procs = desired_procs_part1;
//     int items_per_proc = user_array_size / active_procs;
//     int remainder = user_array_size % active_procs;
    
//     // Determine if this process should participate
//     if (rank < active_procs) {
//         // Calculate this process's portion
//         int start_idx = rank * items_per_proc + (rank < remainder ? rank : remainder);
//         int end_idx = start_idx + items_per_proc + (rank < remainder ? 1 : 0);
        
//         // Synchronize before timer
//         MPI_Barrier(MPI_COMM_WORLD);
        
//         // Start timer
//         start_time = MPI_Wtime();
        
//         // Process 0 prints the header
//         if (rank == 0) {
//             printf("\n--- Bar chart for user input (MPI with %d processes) ---\n", active_procs);
//         }
        
//         // Generate bar chart segment
//         generateBarChartSegment(values, start_idx, end_idx, rank);
        
//         // End timing
//         end_time = MPI_Wtime();
//         local_time = end_time - start_time;
//     } else {
//         // This process is not participating in Part 1
//         local_time = 0.0;
//     }
    
//     // Find the maximum time across all active processes
//     MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
//     // Process 0 reports the execution time
//     if (rank == 0) {
//         printf("Execution time for user input with %d processes: %f seconds\n\n", 
//                active_procs, global_time);
//     }
    
//     // Free memory used for user input array
//     free(values);
//     values = NULL;
    
//     // Synchronize all processes before moving to Part 2
//     MPI_Barrier(MPI_COMM_WORLD);
    
//     // ============= PART 2: RANDOM ARRAY PROCESSING =============
    
//     int random_array_size = 0;   // Size of the random array
//     int max_value = 0;           // Maximum value for random numbers
//     int desired_procs_part2 = 0; // User's desired number of processes for part 2
    
//     // Only process 0 handles user input
//     if (rank == 0) {
//         printf("\n===== PART 2: RANDOM ARRAY =====\n\n");
        
//         // Ask user for desired number of processes
//         printf("How many processes would you like to use for random array? (max available: %d): ", size);
//         scanf("%d", &desired_procs_part2);
        
//         // Validate user input for process count
//         if (desired_procs_part2 <= 0 || desired_procs_part2 > size) {
//             printf("Invalid number of processes. Using all available (%d) processes.\n", size);
//             desired_procs_part2 = size;
//         }
        
//         // Get size for the random array (different from Part 1)
//         printf("Enter the size for the random array: ");
//         scanf("%d", &random_array_size);
        
//         // Get maximum value for random data
//         printf("Enter maximum value for the random dataset: ");
//         scanf("%d", &max_value);
        
//         // Allocate memory for the array
//         values = (int*)malloc(random_array_size * sizeof(int));
        
//         // Generate random data
//         printf("Generating random array with %d elements...\n", random_array_size);
//         generateLargeArray(values, random_array_size, max_value);
//     }
    
//     // Broadcast configuration to all processes
//     MPI_Bcast(&desired_procs_part2, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&random_array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
//     // Allocate memory for non-root processes
//     if (rank != 0) {
//         values = (int*)malloc(random_array_size * sizeof(int));
//     }
    
//     // Now that we have values array allocated in each process, broadcast the actual array values
//     MPI_Bcast(values, random_array_size, MPI_INT, 0, MPI_COMM_WORLD);
    
//     // Calculate work distribution for active processes
//     active_procs = desired_procs_part2;
//     items_per_proc = random_array_size / active_procs;
//     remainder = random_array_size % active_procs;
    
//     // Determine if this process should participate
//     if (rank < active_procs) {
//         // Calculate this process's portion
//         int start_idx = rank * items_per_proc + (rank < remainder ? rank : remainder);
//         int end_idx = start_idx + items_per_proc + (rank < remainder ? 1 : 0);
        
//         // Synchronize before timing
//         MPI_Barrier(MPI_COMM_WORLD);
        
//         // Start timing
//         start_time = MPI_Wtime();
        
//         // Process 0 prints the header
//         if (rank == 0) {
//             printf("\n--- Bar chart for random data (MPI with %d processes) ---\n", active_procs);
//         }
        
//         // Generate bar chart segment
//         generateBarChartSegment(values, start_idx, end_idx, rank);
        
//         // End timing
//         end_time = MPI_Wtime();
//         local_time = end_time - start_time;
//     } else {
//         // This process is not participating in Part 2
//         local_time = 0.0;
//     }
    
//     // Find the maximum time across all active processes
//     MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
//     // Process 0 reports the execution time
//     if (rank == 0) {
//         printf("Execution time for random data with %d processes: %f seconds\n", 
//                active_procs, global_time);
//     }
    
//     // Free memory used for random array
//     free(values);
    
//     // ======= FINALIZE MPI ENVIRONMENT =======
//     // MPI can only be finalized ONCE per program execution
//     MPI_Finalize();
    
//     return 0;
// }

// /*
//  * ============= MPI EXPLANATION =============
//  * 
//  * MPI (Message Passing Interface) is a standard for parallel programming that allows
//  * multiple processes to work together on a distributed memory system.
//  * 
//  * Key MPI Concepts:
//  * 
//  * 1. Processes - Each MPI program runs multiple copies (processes) of the same code
//  *    Each process has its own memory space and cannot directly access variables in other processes
//  * 
//  * 2. Rank - Each process gets a unique ID (rank) from 0 to (size-1)
//  *    Process with rank 0 is often used as the "coordinator" or "root" process
//  * 
//  * 3. Communication - Processes share data by explicitly sending and receiving messages
//  *    Common patterns include:
//  *    - Point-to-point: One process sends to another specific process
//  *    - Collective: All processes participate (e.g., broadcast, gather, reduce)
//  * 
//  * 4. Work Distribution - The problem is divided among processes
//  *    Each process works on a portion of the data in parallel
//  * 
//  * 5. MPI Lifecycle - Important rule: MPI_Init can only be called ONCE per program execution
//  *    and MPI_Finalize can only be called ONCE at the end of the program.
//  *    You cannot reinitialize MPI after finalizing it.
//  * 
//  * Key MPI Functions Used:
//  * 
//  * - MPI_Init: Initializes the MPI environment (called once at beginning)
//  * - MPI_Finalize: Cleans up the MPI environment (called once at end)
//  * - MPI_Comm_rank: Gets the rank (ID) of the current process
//  * - MPI_Comm_size: Gets the total number of processes
//  * - MPI_Bcast: Broadcasts data from one process to all others
//  * - MPI_Barrier: Synchronizes all processes (all wait until everyone reaches this point)
//  * - MPI_Reduce: Combines values from all processes into a single result
//  * - MPI_Wtime: High-precision timer for performance measurement
//  * 
//  * ============= PROGRAM STRUCTURE =============
//  * 
//  * This program is divided into two parts but within a single MPI session:
//  * 
//  * 1. User Input Array Processing:
//  *    - Gets array values from the user
//  *    - Times how long it takes to generate the bar chart in parallel
//  * 
//  * 2. Random Array Processing:
//  *    - Generates a random array of a size specified by the user
//  *    - Times how long it takes to generate the bar chart in parallel
//  * 
//  * Both parts use the same MPI initialization and share the same MPI environment.
//  * 
//  * ============= HOW TO COMPILE AND RUN =============
//  * 
//  * 1. Compile with: mpicc -o mpi_barchart mpi_barchart.c
//  mpicc -o claude claude.c
//  * 2. Run with: mpirun -np <number_of_processes> ./mpi_barchart
//  *    Example: mpirun -np 4 ./mpi_barchart
//  mpirun -np 4 ./claude   
//  *    The "-np 4" tells MPI to create 4 processes.
//  *    You can specify any number, but it's usually best to match your CPU core count.
//  * 
//  * Note: Even though you start with a specific number of processes,
//  * the program will ask which subset of these you want to actually use for each part.
//  */