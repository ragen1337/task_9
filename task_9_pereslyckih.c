#include <stdio.h>
#include <mpi.h>

double integrable_func(double x);
double rectangle_method_func(long num, double a, double b, int size, int rank);

int main (int argc, char *argv[])
{
    long num = 1000;//number of splits
    double a = 0.0;
    double b = 3.0;
    
    int size, rank;// mpi size and rank
    double start, end;
    double integral;
    double reducing_sum;
    
    MPI_Init(&argc, &argv);//mpi start
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0)
        start = MPI_Wtime();

    integral = rectangle_method_func(num, a, b, size, rank);
    MPI_Reduce(&integral, &reducing_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("%f\n", integral);
    if (rank == 0)
    {
        printf(" \nIntegral by rectangle method: %e\n" , reducing_sum);
        end = MPI_Wtime();
        printf("Time: %f\n", end - start);
            printf("\n\n");
    }

    MPI_Finalize();
}
/*
parallel rectangle algorithm
*/
double rectangle_method_func(long num, double a, double b, int size, int rank)
{
    double sum = 0;
    double h = (b - a) / (double)num;
    double x;
    long i;

    for ( i = rank; i < num; i += size ){
        x = a + (double)i * h; 
        sum += integrable_func(x);
    }

    return h * sum;
}

/*
*integrable function (x^3)
*if you want you can change it
*/
double integrable_func(double x)
{
    return x*x*x; 
}