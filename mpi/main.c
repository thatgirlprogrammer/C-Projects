#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f( double n ) {
    return (4.0 / (1.0 + n*n));
}

int main( int argc, char *argv[])
{
    int done = 0, num_rectangles = 1000000, comm_rank, numprocs, i;
    double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&comm_rank);

    startwtime = MPI_Wtime();
    MPI_Bcast(&num_rectangles, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h   = 1.0 / (double) num_rectangles;
    sum = 0.0;
    for (i = comm_rank + 1; i <= num_rectangles; i += numprocs) {
        x = h * ((double)i - 0.5);
        sum += f(x);
    }

    mypi = h * sum;
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (comm_rank == 0) {
        printf("pi is approximately %.16f\n", pi);
        endwtime = MPI_Wtime();
        printf("Time taken: %f milliseconds\n", 1000 * (endwtime - startwtime));
    }

    MPI_Finalize();
    return 0;
}

