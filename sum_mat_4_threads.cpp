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

const int NUMTHREAD=4;
int m, n;
vvi A, B, C;

void *sum_mat(void* in)
{
	int idx;
	idx=(intptr_t)in;
	for(int i=idx; i<m*n; i+=NUMTHREAD)
	{
		int p, q;
		p=i/n;
		q=i%n;
		C[p][q]=A[p][q]+B[p][q];
	}
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
		rc=pthread_create(&thread_vc[i], NULL, sum_mat, (void*)(intptr_t)thread_id);
		if(rc)
		{
			cout<<"Error: Unable to create thread, "<<rc<<endl;
			exit(-1);
		}
	}
	
	rep(i, 0, NUMTHREAD)
		pthread_join(thread_vc[i], NULL);
	auto end=chrono::high_resolution_clock::now();
	
	cout<<"Data processed row wise!\n";
	//print_mat();
	
	cout<< "Time in processing row wise: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	return 0;
}
