#include<stdio.h>
#include<stdlib.h>
#include<time.h>

const int N=3;
int **A, **B, **C;

void *prod_mat_row(void* in)
{
	int idx, j, k, sum;
	idx=(int)in;
	for(j=0; j<N; ++j)
	{
		sum=0;
		for(k=0; k<N; ++k)
			sum+=A[idx][k]*B[k][j];
		C[idx][j]=sum;
	}
	pthread_exit(NULL);	
}

void random_mat(int **arr)
{
	int i, j;
	for(i=0; i<N; ++i)
	for(j=0; j<N; ++j)
	arr[i][j]=rand()%10;
}

void print_mat(int **arr)
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
	int i;
	int rc, thread_id;
	pthread_t thread_arr[N];
	
	A=(int**)malloc(N*sizeof(int*));
	B=(int**)malloc(N*sizeof(int*));
	C=(int**)malloc(N*sizeof(int*));
	for(i=0; i<N; ++i)
	{
		A[i]=(int*)malloc(N*sizeof(int));
		B[i]=(int*)malloc(N*sizeof(int));
		C[i]=(int*)malloc(N*sizeof(int));
	}
	
	srand(time(NULL));
	random_mat(A); random_mat(B);
	print_mat(A); print_mat(B);
	
	for(i=0; i<N; ++i)
	{
		thread_id=i;
		rc=pthread_create(&thread_arr[i], NULL, prod_mat_row, (void*)(int)thread_id);
		if(rc)
		{
			printf("Error: Unable to create thread, %d\n",rc);
			exit(-1);
		}
	}
	for(i=0; i<N; ++i)
		pthread_join(thread_arr[i], NULL);
	
	print_mat(C);
	
	free(A); free(B); free(C);		
	return 0;
}
