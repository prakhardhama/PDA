#include<iostream>
#include<vector>
#include<ctime>
#include<chrono>
#include<math.h>
#include<iomanip>

using namespace std;

#define vi vector<int>
#define vvi vector<vi>
#define rep($, a, b) for(int $=(a); $<(b); ++$)

int NUMTHREAD;
int m, n;
vvi A, B, C;

void *sum_mat_parallel(void* in)
{
	int idx;
	idx=(intptr_t)in;
	int i, j;
	i=idx/n;
	j=idx%n;
	C[i][j]=A[i][j]+B[i][j];
	pthread_exit(NULL);	
}

void print_mat()
{
	cout<<"Mat A:\n";
	rep(i, 0, m)
	{
		rep(j, 0, n)
		cout<<A[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"Mat B:\n";
	rep(i, 0, m)
	{
		rep(j, 0, n)
		cout<<B[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"Mat C:\n";
	rep(i, 0, m)
	{
		rep(j, 0, n)
		cout<<C[i][j]<<"\t";
		cout<<endl;
	}
}

int main()
{
	vector<pthread_t> thread_vc;
	int rc;
	
	cout<<"Enter m, n: ";
	cin>>m>>n;
	NUMTHREAD=m*n;
	thread_vc.resize(NUMTHREAD);
	
	A.resize(m);
	B.resize(m);
	C.resize(m);
	rep(i, 0, m)
	{
		A[i].resize(n);
		B[i].resize(n);
		C[i].resize(n);
	}
	
	srand(time(NULL));
	rep(i, 0, m)
	rep(j, 0, n)
	{
		A[i][j]=rand()%100;
		B[i][j]=rand()%100;
	}
	cout<<"Data generated!\n";
	
	auto begin=chrono::high_resolution_clock::now();
	rep(i, 0, NUMTHREAD)
	{
		int thread_id=i;
		rc=pthread_create(&thread_vc[i], NULL, sum_mat_parallel, (void*)(intptr_t)thread_id);
		if(rc)
		{
			cout<<"Error: Unable to create thread, "<<rc<<endl;
			exit(-1);
		}
	}
	
	rep(i, 0, NUMTHREAD)
		pthread_join(thread_vc[i], NULL);
	auto end=chrono::high_resolution_clock::now();
	
	cout<<"Data processed!\n";
	//print_mat();
	
	cout<< "Time in processing: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	return 0;
}
