#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>



int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int size;
    int dims[3] = {0, 0, 0};
    int periods[3] = {true, true, true};
    int reorder = true;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Dims_create(size, 3, dims);

    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, reorder, &cart_comm);

    int my_rank;
    int my_coords[3];

    MPI_Comm_rank(cart_comm, &my_rank);
    MPI_Cart_coords(cart_comm, my_rank, 3, my_coords);
    
    printf("[MPI Process %d] located at (%d, %d, %d) \n", my_rank, my_coords[0], my_coords[1], my_coords[2]); 

    MPI_Finalize();

    return EXIT_SUCCESS;
}
