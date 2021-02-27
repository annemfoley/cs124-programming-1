#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 
#include <sys/time.h>
#include <chrono>

#include <math.h>
#include <vector>
#include <algorithm>

// group of sets capable of doing union and find
struct DisjointSets{
    unsigned long * parents; // list of parent vertices
    int * ranks; // list of ranks
    unsigned long n; // number of vertices

    DisjointSets(unsigned long num_v){
        this->n = num_v;
        this->parents = new unsigned long[n];
        this->ranks = new int[n];
        for(unsigned long i = 0; i<n; i++){
            parents[i] = i; // each vertex starts as its own set
            ranks[i] = 0; // each starts of with rank 0
        }
    }

    unsigned long find(unsigned long vertex);
    unsigned long link(unsigned long v1, unsigned long v2);
    void set_union(unsigned long v1, unsigned long v2);

};

// finds the root of the set and flattens the tree
unsigned long DisjointSets::find(unsigned long vertex){
    if(parents[vertex]!=vertex){
        parents[vertex] = find(parents[vertex]);
    }
    return parents[vertex];
}

// joins two sets together by comparing rank
unsigned long DisjointSets::link(unsigned long v1, unsigned long v2){
    if(ranks[v1] > ranks[v2]){
        unsigned long temp = v1;
        v1=v2;
        v2=temp;
    }
    else if(ranks[v1]==ranks[v2]){
        ranks[v2]+=1;
    }
    parents[v1]=v2;
    return v2;
}

// union two sets together
void DisjointSets::set_union(unsigned long v1, unsigned long v2){
    link(find(v1),find(v2));
}


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


// this function is to get data for graphs with n from 128 to 262144
void get_data(){

    const int numtests = 12;
    const int numtrials = 5;

    double weight_data[4][numtests];
    double time_data[4][numtests];

    // run numtests on all dimensions 0, 2-4
    for(int d = 0; d <= 4; d++){
        if(d!=1){
            printf("dim %d running...\n", d);
            for(unsigned long k = 7; k < 7+numtests; k++){
                double avg_mst_weight = 0;
                auto start_m = std::chrono::high_resolution_clock::now();
                for(int i = 0; i<numtrials; i++){
                    Graph g = Graph(pow(2,k), d);
                    avg_mst_weight += g.kruskal()/numtrials;
                    if(d>=1) delete g.V;
                }
                auto stop_m = std::chrono::high_resolution_clock::now();
                auto t_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);

                weight_data[d][k-7] = avg_mst_weight;
                time_data[d][k-7] = (double) t_m.count() / numtrials / 1e6;
            }
            // display the MST weights and algorithm running time
            printf("\n------DIMENSION %d------\n", d);
            for(int i=0; i<numtests; i++){
                printf("%d:\t%f\t%f\n", (int) pow(2,i+7), weight_data[d][i], time_data[d][i]);
            }
        }
    }
    printf("\n");
}


int main(int argc, char** argv) {
    // set the seed
    srand(time(NULL));

    // get user-inputted arguments
    int testmode = std::stoi(argv[1]);
    unsigned long n = std::stoi(argv[2]);
    int numtrials = std::stoi(argv[3]);
    int dim = std::stoi(argv[4]);

    // run get_data if first argument is 1
    if(testmode==1){
        get_data();
    }

    // otherwise run user-specified number of vertices, iterations
    //  and dimensions
    else{
        double max = 0;  // to calculate k(n)
    
        // for numtrial times, generate a graph and calculate MST weight
        //  and record total time for all numtrials 
        double avg_mst_weight = 0;
        auto start_m = std::chrono::high_resolution_clock::now();
        for(int i = 0; i<numtrials; i++){
            Graph g = Graph(n, dim);
            avg_mst_weight += g.kruskal()/numtrials;
            if(dim>=1) delete g.V;
            if(g.max_weight>max){
                max = g.max_weight;
            }
        }
        auto stop_m = std::chrono::high_resolution_clock::now();
        auto t_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);

        printf("\nNumber of vertices: %ld\n", n);
        printf("Dimensions: %d\n", dim);
        printf("Average MST Weight: %f\n", avg_mst_weight);
        printf("total time to run per iteration (sec): %f\n\n", (double) t_m.count() / numtrials / 1e6);
    }
}