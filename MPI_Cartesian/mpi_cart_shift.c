#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mpi.h>

enum DIRECTIONS{DOWN, UP, LEFT, RIGHT};
char* direction_str[4] = {"down", "up", "left", "right"};

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	
	int size;
	int dims[2] = {0, 0};
	int periods[2] = {false, false};
	bool reorder = true;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Dims_create(size, 2, dims);

	MPI_Comm cart_comm;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);

	int my_rank;
	int my_coords[2];

	MPI_Comm_rank(cart_comm, &my_rank);
	MPI_Cart_coords(cart_comm, my_rank, 2, my_coords);
	
	int my_neighbours_ranks[4];
	
	// from left to right
	MPI_Cart_shift(cart_comm, 0, 1, &my_neighbours_ranks[LEFT], &my_neighbours_ranks[RIGHT]);	
	// from down to up
	MPI_Cart_shift(cart_comm, 1, 1, &my_neighbours_ranks[DOWN], &my_neighbours_ranks[UP]);	


	for(int i = 0; i < 4; i++)
	{
		if(my_neighbours_ranks[i] == MPI_PROC_NULL)
			printf("Process %d : I have no %s neighbour \n", my_rank, direction_str[i]);
		else
			printf("Process %d : My %s neighbour is rank %d \n", my_rank, direction_str[i], my_neighbours_ranks[i]);
	}
	
		
	MPI_Finalize();
	return EXIT_SUCCESS;
}
