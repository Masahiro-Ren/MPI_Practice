#include <iostream>
#include <mpi.h>

const static size_t BUFFER_SIZE = 10;

int main(int argc, char* argv[])
{
    int my_rank;
    int size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2)
    {
        std::cout << "Process number is not equal to 2 \n";
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int* buffer = new int[2 * BUFFER_SIZE];

    /**
     * Preparing for window
     */
    MPI_Win win;
    MPI_Aint buf_size = 2 * BUFFER_SIZE * sizeof(int);
    int disp_unit = sizeof(int);
    MPI_Win_create(buffer, buf_size, disp_unit, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    /**
     * Preparing for data
     */
    if(my_rank == 0)
    {
        for(int i = 0; i < 2 * BUFFER_SIZE; i++)
            buffer[i] = 1;
    }
    if(my_rank == 1)
    {
        for(int i = 0; i < 2 * BUFFER_SIZE; i++)
            buffer[i] = 2;
    }

    /**
     * Communication start
     */
    MPI_Win_fence(0, win);
    if(my_rank == 0)
    {
        // Put first 10 integers to last 10 entires of the array in rank 1
        MPI_Aint target_disp = BUFFER_SIZE;
        MPI_Put(buffer, BUFFER_SIZE, MPI_INT, 1, target_disp, BUFFER_SIZE, MPI_INT, win);

        // Get first 10 integers from the array in rank 1 to last 10 entries of the array in this rank
        MPI_Get((buffer + BUFFER_SIZE), BUFFER_SIZE, MPI_INT, 1, 0, BUFFER_SIZE, MPI_INT, win);
    }
    MPI_Win_fence(0, win);

    // print result
    std::cout << "Rank " << my_rank << std::endl;
    for(int i = 0; i < 2 * BUFFER_SIZE; i++)
    {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;

    // clean up
    delete[] buffer;
    MPI_Win_free(&win);
    MPI_Finalize();
    return EXIT_SUCCESS;
}