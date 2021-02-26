#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <time.h> 
#include <vector>
#include <algorithm>

#include "./union-find.cc"


// vertex-weight pair representing edge
typedef std::pair<double,std::pair<unsigned long,unsigned long>> edge;

// constants for calculating the (likely) maximum possible edge weight of an MST
const double eq_params[4][2] = {{ 2.76348 , -0.6682713 },
                                { 1.626765, -0.3542332},
                                { 1.631566, -0.2606066 },
                                { 1.750868, -0.2368739}};

// a function that returns a "cap" on the weight of an edge in an mst
//  based on number of vertices and the dimension, an exponential decay function
double weight_cap(unsigned long n, int d){
    if(d>1){
        d-=1;
    }
    double * eq = (double *) &eq_params[d];
    return 1.2 * eq[0] * pow(n,eq[1]) ; // multiply by 1.2 as extra insurance for correctedness
}


// representation of our generated graph
struct Graph{
    unsigned long n; // # of vertices
    int d; // dimension, 0 = randomly generated weights in range [0,1]
    double* V; // array of vertices, use for >=1 dimension
    double max_weight;  // to calculate k(n)
    void init_graph();
    Graph(unsigned long num_v, int dim){
        this->n = num_v;
        this->d = dim;
        if(d>=1){
            this->V = new double[n * d];
            this->init_graph();
        }
        this->max_weight = 0;
    };

    double calculate_edge(unsigned long v1, unsigned long v2);

    double kruskal();
};




// create our random graph, only necessary when dimension >= 1
//  initialize all vertex coordinates
void Graph::init_graph(){
    if(d>=1){
        for(unsigned long i=0; i<this->n; i++){
            for(int k=0; k<this->d; k++){
                V[i*d+k] = (double) rand() / (double) (RAND_MAX-1); // random number in range [0,1] with 1/(RANDMAX-1) spacing
            }
        }
    }
}

// return the edge weight between v1 and v2
//  NOTE: only call this function once per edge in our algorithm
double Graph::calculate_edge(unsigned long v1, unsigned long v2){
    // if 0 dimension, return random number in range [0,1]
    if(d < 1){
        return (double) rand() / (double) (RAND_MAX-1); // random number in range [0,1] with 1/(RANDMAX-1) spacing
    }
    // otherwise calculate euclidian distance
    else{
        double edge_weight_sq = 0;
        for(int k = 0; k<d; k++){
            edge_weight_sq += pow(V[v1*d+k] - V[v2*d+k],2);
        }
        return sqrt(edge_weight_sq);
    }
}


// Kruskal's algorithm to find the MST
//  returns the total weight of the MST
double Graph::kruskal(){
    
    // create vector of edges, add an edge only if the minimum
    //  spanning tree is very likely to contain said edge
    std::vector<edge> edges;
    double edge_weight;
    for(unsigned long i = 0; i<n; i++){
        for(unsigned long j = 0; j<i; j++){
            edge_weight = calculate_edge(i,j);
            if(edge_weight<=weight_cap(n,d)){
                edges.push_back({edge_weight,{i,j}});
            }
        }
    }
    // sort the edges in ascending order of weight
    sort(edges.begin(), edges.end());
    
    // initialize our disjoint sets capable of union and find
    DisjointSets sets(n);
    
    // perform Kruskal's algorithm and keep track of mst_weight
    double mst_weight = 0;
    for(unsigned long i = 0; i < edges.size(); i++){
        unsigned long v1 = edges[i].second.first;
        unsigned long v2 = edges[i].second.second;
        double weight = edges[i].first;
        if(sets.find(v1) != sets.find(v2)){

            mst_weight += weight;
            max_weight = weight; // to calculate k(n)
            sets.set_union(v1, v2);
        }

    }

    delete sets.parents;
    delete sets.ranks;
    return mst_weight;
}

