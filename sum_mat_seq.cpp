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

int m, n;
vvi A, B, C;

void solve()
{
	rep(i, 0, m)
	rep(j, 0, n)
	C[i][j]=A[i][j]+B[i][j];
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
	cout<<"Enter m, n: ";
	cin>>m>>n;
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
	solve();
	auto end=chrono::high_resolution_clock::now();
	
	cout<<"Data processed!\n";
	//print_mat();
	
	cout<< "Time in processing: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	return 0;
}
