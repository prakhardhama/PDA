#include<iostream>
#include<math.h>
#include<iomanip>
#include<ctime>
#include<chrono>

using namespace std;

#define PI 3.141592653589793
#define ARRSIZE 500000
#define NUMTERM 1000

double cos_x(double x, double t)
{
	double pres, res, xval, fval;
	int sign;
	sign=-1;
	xval=x*x; fval=2;
	res=1+sign*xval/fval;
	for(int k=2; k<=t; ++k)
	{
		xval*=x*x;
		fval*=(2*k-1)*(2*k);
		sign*=-1;
		pres=res;
		res+=sign*xval/fval;
		if(isnan(res)) return pres;
	}
	return res;	
}

int main()
{
	cout<<"Start the work\n";
	cout<<setprecision(15);
	
	double array[ARRSIZE];
	double cos_array[ARRSIZE];
	
	srand(time(NULL));
	for(int i=0; i<ARRSIZE; ++i)
		array[i]=(rand()%360)*PI/180.0;
	
	cout<<"Data generated\n";
	
	auto begin=chrono::high_resolution_clock::now();
	for(int i=0; i<ARRSIZE; ++i)
		cos_array[i]=cos_x(array[i], NUMTERM);
	auto end=chrono::high_resolution_clock::now();
	
	cout<<"Data processed\n";
	
	/*
	for(int i=0; i<ARRSIZE; ++i)
	{
		cout<<array[i]<<"\t: "<<cos_array[i]<<endl;
	}*/
	cout<<array[0]<<"\t: "<<cos_array[0]<<endl;
	
	cout<<"End of work\n";
	cout<< "Time in processing: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	return 0;
}





