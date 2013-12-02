#include <iostream>
#include <vector>
#include <cilk.h>
#include <stdlib.h>
#include <math.h>
#include "Par_CC2.h"
#include <set>
#include <algorithm>
#include <cilkview.h>
using namespace std;

int iter = 0, iter2 = 0;
int  *N;
vector<int> L;
int VCOUNT = 0;
bool *H, *C;
int dmax;
double alpha;

void Random_Hook(vector<int> &V){
	int n = V.size();
	cilk_for(int i=0;i<n;i++){
		double r = (double)rand()/RAND_MAX;
		C[V[i]] = (r>=0.5)?HEAD:TAIL;
		H[V[i]] = false;		
	}

	cilk_for(int i=0;i<n;i++){
		int u = V[i];
		int v = N[u];
		if(v >0 && C[u] == TAIL && C[v] == HEAD){
			L[u] = v;
			H[u] = true;
			H[v] = true;
		}	
	}
	
	cilk_for(int i=0;i<n;i++){
		int u = V[i];
		if(H[u] == true)
			C[u] = HEAD;
		else if (C[u] == TAIL)
			C[u] = HEAD;
		else
			C[u] = TAIL;
	}

	cilk_for(int i=0;i<n;i++){
		int u = V[i];
		int v = N[u];
		if(v>0 && C[u] == TAIL && C[v] == HEAD){
			L[u] = L[v];
		}
	}
}

void Par_Randomized_CC2(vector<int> &V, vector<Edge> &E){
	
	int n = V.size();
	int m = E.size();

	if(m == 0){
		return;
	}
	
	iter2++;

	//cout<<"CC2 iter: "<<iter2<<endl;
	cilk_for(int i=1;i<=VCOUNT;i++)
		N[i] = -1;

	cilk_for(int i=0;i<m;i++){
		N[E[i].u] = E[i].v;
		N[E[i].v] = E[i].u;
	}

	Random_Hook(V);
	
	vector<Edge> F;
	get_new_edge_set(E,L,F);
	vector<Edge>().swap(E);
	
	vector<int> V1;
	get_new_vertex_set(V, L, V1);	
	
	Par_Randomized_CC2(V1, F);
	
	cilk_for(int i=0;i<n;i++){
		L[V[i]] = L[L[V[i]]];
	}	
} 

void getSampleEdges(vector<Edge> &E, vector<Edge> &E1){
	int m = E.size();
	int md = E1.size();
	int chunkSize = m/md;
	cilk_for(int i=0;i<md;i++){
		int startIndex = chunkSize*i;
		int offset = rand()%chunkSize;
		E1[i] = E[startIndex+offset];
	}
}

void Par_Randomized_CC3(vector<int> &V, vector<Edge> &E, bool *Phd, bool *U, int d){

	if(d > dmax)
		return;

	int n = V.size();
	int m = E.size();
	int md = m*pow(alpha,d);
	if(md == 0)
		return;

	iter++;
	
	//cout<<"d = "<<d<<" md = "<<md<<" n = "<<n<<endl;

	vector<Edge> E1;

	if(d > 0) {
		E1.resize(md);
		getSampleEdges(E, E1);
	}

	cilk_for(int i=1;i<=VCOUNT;i++)
		N[i] = -2;	

	cilk_for(int i=0;i<n;i++)
		N[V[i]] = -1;
	
	cilk_for(int i=0;i<n;i++)
		U[V[i]] = false;

	cilk_for(int i=0;i<md;i++){
		int u1 = (d > 0)?E1[i].u:E[i].u;
		int v1 = (d > 0)?E1[i].v:E[i].v;
		if(N[u1] != -2 && N[v1] != -2 &&
		   Phd[u1] && Phd[v1])
		{
			N[u1] = v1;
			N[v1] = u1;

			U[u1] = true;
			U[v1] = true;
		}
	}
	
	vector<Edge>().swap(E1);

	cilk_for(int i=0;i<n;i++){
		if(!U[V[i]])
			Phd[V[i]] = false;
	}

	vector<int> V1;
	for(int i=0;i<n;i++){
		if(U[V[i]])
			V1.push_back(V[i]);
	}
	
	Random_Hook(V1);

	vector<int>().swap(V1);

	vector<int> V2;
	get_new_vertex_set(V, L, V2);

	if(V2.size() < V.size()) //vertex set is reduced
		Par_Randomized_CC3(V2, E, Phd, U, d+1);	
	
	vector<int>().swap(V2);

	cilk_for(int i=0;i<n;i++){
		L[V[i]] = L[L[V[i]]];	
	}			
	
	if(d == 0){
		
		//cout<<"Done with CC3. Now gonna do CC2\n";
		
		vector<int> V1;
		get_new_vertex_set(V, L, V1);

		vector<Edge> E1;
		get_new_edge_set(E, L, E1);

		vector<Edge>().swap(E);

		Par_Randomized_CC2(V1, E1);

		int n = V.size();
		cilk_for(int i=0;i<n;i++){
			L[V[i]] = L[L[V[i]]];
		}
	}
}


int cilk_main(int argc, char* argv[]){
	int n, m;
	
	scanf("%d %d", &n, &m);
	
	vector<int> V(n);
	vector<Edge> E(m);
	N = new int[n+1];
	L.resize(n+1);
	VCOUNT = n;
	
	C = new bool[n+1];
	H = new bool[n+1];
	
	bool *Phd = new bool[n+1];
	bool *U = new bool[n+1];

	int d = 0;
	alpha = sqrt(15.0/16.0);
	dmax = 0.25*log(1/alpha)*n;
	
	//cout<<"dmax: "<<dmax<<endl;
	
	cilk_for(int i=0; i<n; i++){
		V[i] = i+1;
		L[i+1] = i+1;
		Phd[i+1] = true;
	}
	
	for(int j=0;j<m;j++){
		scanf("%d %d", &(E[j].u), &(E[j].v));
	}
	
	srand(time(NULL));
	
	cilk::cilkview cv;
	cv.start();
	Par_Randomized_CC3(V, E, Phd, U,d);
	cv.stop();
	
	delete[] C;
	delete[] H;
	delete[] Phd;
	delete[] U;
	delete[] N;
	
	print_results(L);	

	cerr<<"No. of iterations in cc3: "<<iter<<endl;
	cerr<<"NO. of iterations in cc2: "<<iter2<<endl;
	cerr<<"Time taken: "<<cv.accumulated_milliseconds()<<endl;

	return 0;
}


