#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>

void random_mat(int **arr, int N)
{
	int i, j;
	for(i=0; i<N; ++i)
	for(j=0; j<N; ++j)
	arr[i][j]=rand()%5;
}

void print_mat(int **arr, int N)
{
	int i, j;
	printf("\n");
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; ++j)
		printf("%d ", arr[i][j]);
		printf("\n");
	}	
}

int main()
{
	int i, j, num, rank, N, sum;
	int **A, **B;
	int *CA, *LA, *LC;
		
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	N=num;
	B=(int**)malloc(N*sizeof(int*));
	for(i=0; i<N; ++i)
		B[i]=(int*)malloc(N*sizeof(int));
	LA=(int*)malloc(N*sizeof(int));
	LC=(int*)malloc(N*sizeof(int));
	
	if(rank==0)
	{
		printf("Initializing matrices by p%d\n", rank);
		srand(time(NULL));
		A=(int**)malloc(N*sizeof(int*));
		for(i=0; i<N; ++i)
			A[i]=(int*)malloc(N*sizeof(int));
		random_mat(A, N); random_mat(B, N);	
		printf("Matrix A:\n");
		print_mat(A, N); 
		printf("Matrix B:\n");
		print_mat(B, N);
		CA=(int*)malloc(N*N*sizeof(int));
		for(i=0; i<N; ++i)
		for(j=0; j<N; ++j)
			CA[i*N+j]=A[i][j];
		free(A);
	}
	
	MPI_Scatter(CA, N, MPI_INT, LA, N, MPI_INT, 0, MPI_COMM_WORLD);
	for(i=0; i<N; ++i)
		MPI_Bcast(B[i], N, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(i=0; i<N; ++i)
	{
		sum=0;
		for(j=0; j<N; ++j)
			sum+=LA[j]*B[j][i];
		LC[i]=sum;
	}
	
	if(rank==0)
	{
		printf("\nLocal A Row process %d: ", rank);
		for(i=0; i<N; ++i)
			printf("%d ", LA[i]);
		printf("\nLocal C cal process %d: ", rank);
		for(i=0; i<N; ++i)
			printf("%d ", LC[i]);
		printf("\n");
	}
	
	MPI_Gather(LC, N, MPI_INT, CA, N, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank==0)
	{
		printf("\nMatrix C:\n");
		for(i=0; i<N; ++i)
		{	
			for(j=0; j<N; ++j)
				printf("%d ", CA[i*N+j]);
			printf("\n");
		}
	}
	
	free(B); free(LA); free(LC);
	MPI_Finalize();	
	return 0;
}
