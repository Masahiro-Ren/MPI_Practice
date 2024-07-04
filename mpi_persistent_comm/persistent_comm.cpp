#include <iostream>
#include <mpi.h>
#include <cstring>

const static size_t BUFFER_SIZE = 256;

const static int SRC = 0;
const static int DEST = 1;

int main(int argc, char* argv[])
{
    int my_rank;
    int size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2)
    {
        std::cerr << "Process number is neq to 2 \n";
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int* buffer = new int[BUFFER_SIZE];

    MPI_Request pc_req;

    if(my_rank == SRC)
    {
        for(int i = 0; i < BUFFER_SIZE ; i++) buffer[i] = 66;
        MPI_Send_init(buffer, BUFFER_SIZE, MPI_INT, DEST, 0, MPI_COMM_WORLD, &pc_req);
    }
    if(my_rank == DEST)
    {
        MPI_Recv_init(buffer, BUFFER_SIZE, MPI_INT, SRC, 0, MPI_COMM_WORLD, &pc_req);
    }

    for(int i = 0; i < 3; i++)
    {
        MPI_Start(&pc_req);
        MPI_Wait(&pc_req, MPI_STATUS_IGNORE);

        if(my_rank == DEST)
        {
            if(buffer[0] == 66)
            {
                std::cout << "Receive " << BUFFER_SIZE * sizeof(int) << " bytes from source \n";
            }
        }
    }


    // clean up
    delete[] buffer;
    MPI_Request_free(&pc_req);
    MPI_Finalize();
    return EXIT_SUCCESS;
}