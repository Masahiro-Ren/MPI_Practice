#include <mpi.h>
#include <iostream>
#include <cstdlib>

const static int M = 12;
const static int N = 12;

enum ROLES{SRC, DEST};
enum DIRECTIONS{BOTTOM, UP, LEFT, RIGHT};

int grid[M][N];

void print_grid(int grid[M][N]);

void create_bnd_type(int n_rows, int c_cols, MPI_Datatype* bnd_type);

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

    // Wating for initialization
    MPI_Barrier(MPI_COMM_WORLD);

    /**
     * Commit new mpi data type
    */
    MPI_Datatype bnd_type;
    create_bnd_type(M, N, &bnd_type);

    if(myrank == SRC)
    {
        MPI_Send(&grid[0][0], 1, bnd_type, DEST, SRC, MPI_COMM_WORLD);
    }

    if(myrank == DEST)
    {
        MPI_Status status;
        MPI_Recv(&grid[0][0], 1, bnd_type, SRC, SRC, MPI_COMM_WORLD, &status);

        std::cout << "After recv: \n";
        print_grid(grid);
    }
    

    // clean up !!
    MPI_Type_free(&bnd_type);
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

void create_bnd_type(int n_rows, int n_cols, MPI_Datatype* bnd_type)
{
    int type_size = 2 * n_cols + 2 * (n_rows - 2);

    int* blocklens = new int [type_size];
    MPI_Aint* displacements = new MPI_Aint[type_size];
    MPI_Datatype* datatypes = new MPI_Datatype[type_size];

    int index = 0;

    // top row
    for(int i = 0; i < n_cols; i++)
    {
        int offset = i * sizeof(int);
        blocklens[index] = 1;
        displacements[index] = offset;
        datatypes[index] = MPI_INT;
        index++;
    }
    // bottom row
    for(int i = 0; i < n_cols; i++)
    {
        int offset = ( n_cols * (n_rows - 1) + i ) * sizeof(int);
        blocklens[index] = 1;
        displacements[index] = offset;
        datatypes[index] = MPI_INT;
        index++;
    }
    // left column
    for(int i = 1; i < n_rows - 1; i++)
    {
        int offset = ( i * n_rows ) * sizeof(int);
        blocklens[index] = 1;
        displacements[index] = offset;
        datatypes[index] = MPI_INT;
        index++;
    }
    // right column
    for(int i = 1; i < n_rows - 1; i++)
    {
        int offset = ( i * n_rows + (n_cols - 1) ) * sizeof(int);
        blocklens[index] = 1;
        displacements[index] = offset;
        datatypes[index] = MPI_INT;
        index++;
    }


    MPI_Type_create_struct(type_size, blocklens, displacements, datatypes, bnd_type);
    MPI_Type_commit(bnd_type);

    // clean up
    delete[] blocklens;
    delete[] displacements;
    delete[] datatypes;
}