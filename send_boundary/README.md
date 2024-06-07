# Description
1. send_bnd.cpp uses MPI_Type_contiguous and MPI_Type_vector to send rows and columns data respectively of a 2d array.
2. send_bnd2.cpp uses MPI_Type_construct to send all boundary data in 1 send and receive.