#include <iostream>
#include <vector>
#include <cilk.h>
#include <set>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include "Par_CC.h"

using namespace std;

int iter = 0;
vector<int> N;
bool *H, *C;
int VCOUNT;

void Random_Hook(vector<int> &V, vector<int> &L){
	int n = V.size();
	cilk_for(int i=0;i<n;i++){
		double r = (double)rand()/RAND_MAX;
		C[V[i]] = (r>=0.5)?HEAD:TAIL;
		H[V[i]] = false;		
	}

	cilk_for(int i=0;i<n;i++){
		int u = V[i];
		if(N[u] != -1){
			int v = N[u];
			if(C[u] == TAIL && C[v] == HEAD){
				L[u] = v;
				H[u] = true;
				H[v] = true;
			}	
		}
	}
	
	cilk_for(int i=0;i<n;i++){
		if(N[V[i]] != -1){
		int u = V[i];
		if(H[u] == true)
			C[u] = HEAD;
		else if (C[u] == TAIL)
			C[u] = HEAD;
		else
			C[u] = TAIL;
		}
	}

	cilk_for(int i=0;i<n;i++){
		if(N[V[i]]!=-1){
		int u = V[i];
		int v = N[u];
		if(C[u] == TAIL && C[v] == HEAD)
			L[u] = L[v];
		}
	}
}

void Par_Randomized_CC2(vector<int> &V, vector<Edge> &E, vector<int> &L){
	
	iter++;
	int n = V.size();
	int m = E.size();

	if(m == 0){
		return;
	}
	
	cilk_for(int i=1;i<=VCOUNT;i++)
		N[i] = -1;

	cilk_for(int i=0;i<m;i++){
		N[E[i].u] = E[i].v;
		N[E[i].v] = E[i].u;
	}

	Random_Hook(V, L);
	
	vector<Edge> F;
	get_new_edge_set(E,L,F);
	vector<Edge>().swap(E);

	vector<int> V1;
	get_new_vertex_set(V, L, V1);	
	
	Par_Randomized_CC2(V1, F, L);
	
	cilk_for(int i=0;i<n;i++){
		L[V[i]] = L[L[V[i]]];
	}	
} 

int cilk_main(int argc, char* argv[]){
	int n, m;
	
	scanf("%d %d", &n, &m);
	VCOUNT = n;	

	vector<int> V(n);
	vector<Edge> E(m);
	vector<int> L(n+1);

	C = new bool[n+1];
	H = new bool[n+1];
	N.resize(n+1);

	cilk_for(int i=0; i<n; i++){
		V[i] = i+1;
		L[i+1] = i+1;
	}
	
	for(int j=0;j<m;j++){
		scanf("%d %d", &(E[j].u), &(E[j].v));
	}

	srand(time(NULL));	
	
	Par_Randomized_CC2(V, E, L);

	delete[] H;
	delete[] C;

	print_results(L);
	//cout<<"No. of iterations: "<<iter<<endl;

	return 0;
}


