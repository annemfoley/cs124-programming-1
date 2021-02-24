#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 

#include "./union-find.cc"


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
    return 0;
}


double Graph::prim(){
    return 0;
}

