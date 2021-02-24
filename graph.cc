#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 

#include "./union-find.cc"


// for Kruskal's algorithm: vertex-weight pair representing edge & mergesort
typedef std::pair<std::pair<unsigned long,unsigned long>,double> edge;
edge * sort(edge * e, unsigned long sz){
    
}





// representation of our generated graph
struct Graph{
    unsigned long n; // # of vertices
    int d; // dimension, -1 = custom
    double* V; // array of vertices, use for >=1 dimension
    double* E; // matrix of edge weights, use for 0 dimension

    void init_graph();
    Graph(unsigned long num_v, int dim){
        this->n = num_v;
        this->d = dim;
        if(this->d<1){
            this->E = (double*) malloc(sizeof(*(this->E)) * this->n * this->n);
        }
        else{
            this->V = (double*) malloc(sizeof(*(this->V)) * this->n * this->d);
        }
        this->init_graph();
    };

    double calculate_edge(unsigned long v1, unsigned long v2);

    double kruskal();
    double prim();
};




// create our random graph
void Graph::init_graph(){

    // handle 0 dimension
    if(d < 1){
        for(int i = 0; i<n; i++){
            for(int j = 0; j<i; j++){
                double edge_weight =  (double) rand() / (double) (RAND_MAX-1);
                E[i*n+j] = edge_weight;
                E[j*n+i] = edge_weight;
            }
            E[i*n+i] = 0;
        }
    }

    // handle >= 1 dimension
    else{
        for(int i=0; i<this->n; i++){
            for(int k=0; k<this->d; k++){
                V[i*d+k] = (double) rand() / (double) (RAND_MAX-1);
            }
        }
    }
}

// return the edge weight between v1 and v2
double Graph::calculate_edge(unsigned long v1, unsigned long v2){
    if(d < 1){
        return E[v1*n + v2];
    }
    else{
        double edge_weight_sq = 0;
        for(int k = 0; k<d; k++){
            edge_weight_sq += pow(V[v1*d+k] - V[v2*d+k],2);
        }
        return sqrt(edge_weight_sq);
    }
}



double Graph::kruskal(){
    double mst_weight = 0;
    edge * edges_list = (edge *) malloc(sizeof(*edges_list) * (n*n/2 - n));
    for(unsigned long i = 0; i<n; i++){
        for(unsigned long j = 0; j<i; j++){
            edges_list[i*n+j] = {{i,j},calculate_edge(i,j)};
        }
    }
    sort(edges_list, (n*n/2 - n));

    DisjointSets sets(n);
    
    for(int i = 0; i < (n*n/2 - n); i++){
        unsigned long v1 = edges_list[i].first.first;
        unsigned long v2 = edges_list[i].first.second;
        double weight = edges_list[i].second;
        if(sets.find(v1) != sets.find(v2)){
            mst_weight += weight;
            sets.set_union(v1, v2);
        }

    }

    return mst_weight;
}


double Graph::prim(){
    return 0;
}

