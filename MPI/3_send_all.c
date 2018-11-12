#include<stdio.h>
#include<mpi.h>

//mpicc -std=c99 -g -Wall -O2 -o 3_send_all 3_send_all.c 
//mpiexec -n 6 ./3_send_all

int main()
{
	int rank, num;
	int tag, dest, count;
	int buffer;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	tag=123;
	dest=0;
	count=1;
	
	if(rank!=dest)
	{
		buffer=rank;
		MPI_Send(&buffer, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
		printf("process %d sent %d\n", rank, buffer);
	}
	else if(rank==dest)
	{
		for(int j=0; j<num-1; ++j)
		{	
			if(j==dest) continue;
			MPI_Recv(&buffer, count, MPI_INT, j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("process %d received %d\n", rank, buffer);
		}
	}
	
	MPI_Finalize();
	
	return 0;
}
