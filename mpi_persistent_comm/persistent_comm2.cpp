#include <iostream>
#include <mpi.h>
#include <cstring>

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

    /**
     * Preparing for persistent communcation
     */
    int send_buf[2];
    int recv_buf[2];
    int TARGET;
    MPI_Request pc_req[2];

    if(my_rank == 0)
    {
        TARGET = 1;
    }
    if(my_rank == 1)
    {
        TARGET = 0;
    }
    MPI_Send_init(send_buf, 2, MPI_INT, TARGET, 0, MPI_COMM_WORLD, &pc_req[0]);
    MPI_Recv_init(recv_buf, 2, MPI_INT, TARGET, 0, MPI_COMM_WORLD, &pc_req[1]);

    for(int i = 0; i < 3; i++)
    {
        send_buf[0] = my_rank;
        send_buf[1] = i;

        std::cout << "MY_RANK : " << my_rank << " Send (" << send_buf[0] << "," << send_buf[1] << ") \n";

        MPI_Startall(2, pc_req);
        MPI_Waitall(2, pc_req, MPI_STATUS_IGNORE);

        std::cout << "MY_RANK : " << my_rank << " Recv (" << recv_buf[0] << "," << recv_buf[1] << ") \n";
    }


    MPI_Request_free(&pc_req[0]);
    MPI_Request_free(&pc_req[1]);
    MPI_Finalize();
    return EXIT_SUCCESS;
}