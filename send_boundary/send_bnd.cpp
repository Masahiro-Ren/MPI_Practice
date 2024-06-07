#include <mpi.h>
#include <iostream>
#include <cstdlib>

const static int M = 12;
const static int N = 12;

enum ROLES{SRC, DEST};
enum DIRECTIONS{BOTTOM, UP, LEFT, RIGHT};

int grid[M][N];

void print_grid(int grid[M][N]);

int main(int argc, char* argv[])
{
    int myrank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2)
    {
        std::cout << "This program only support 2 processes. \n";
        MPI_Finalize();
        std::exit(EXIT_FAILURE);
    }

    if(myrank == SRC)
    {
        for(int i = 0; i < M; i++)
        {
            for(int j = 0; j < N; j++)
            {
                grid[i][j] = i * N + j;
            }
        }
        // std::cout << "grid of SRC: \n";
        // print_grid(grid);
    }
    if(myrank == DEST)
    {
        for(int i = 0; i < M; i++)
        {
            for(int j = 0; j < N; j++)
            {
                grid[i][j] = 0;
            }
        }
        std::cout << "Before recv: \n";
        print_grid(grid);
    }

    // Waiting for initialization
    MPI_Barrier(MPI_COMM_WORLD);

    /**
     * Commit new mpi data type
    */
    MPI_Datatype row_type;
    MPI_Datatype col_type;
    int blocklen = 1;
    int vector_cnt = M - 2;

    MPI_Type_contiguous(N, MPI_INT, &row_type);
    MPI_Type_vector(vector_cnt, blocklen, N, MPI_INT, &col_type);
    MPI_Type_commit(&row_type);
    MPI_Type_commit(&col_type);

    if(myrank == SRC)
    {
        // top row
        MPI_Send(&grid[0][0], 1, row_type, DEST, UP, MPI_COMM_WORLD);
        // bottom row
        MPI_Send(&grid[M-1][0], 1, row_type, DEST, BOTTOM, MPI_COMM_WORLD);
        // left column
        MPI_Send(&grid[1][0], 1, col_type, DEST, LEFT, MPI_COMM_WORLD);
        // right column
        MPI_Send(&grid[1][N-1], 1, col_type, DEST, RIGHT, MPI_COMM_WORLD);
    }

    if(myrank == DEST)
    {
        MPI_Status status;
        // top row
        MPI_Recv(&grid[0][0], 1, row_type, SRC, UP, MPI_COMM_WORLD, &status);
        // bottom row
        MPI_Recv(&grid[M-1][0], 1, row_type, SRC, BOTTOM, MPI_COMM_WORLD, &status);
        // left column
        MPI_Recv(&grid[1][0], 1, col_type, SRC, LEFT, MPI_COMM_WORLD, &status);
        // right column
        MPI_Recv(&grid[1][N-1], 1, col_type, SRC, RIGHT, MPI_COMM_WORLD, &status);

        std::cout << "After recv: \n";
        print_grid(grid);
    }
    

    // clean up !!
    MPI_Type_free(&row_type);
    MPI_Type_free(&col_type);
    MPI_Finalize();

    return 0;
}

void print_grid(int grid[M][N])
{
    for(int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            std::cout << grid[i][j]  << "\t";
        }
        std::cout << std::endl;
    }
}