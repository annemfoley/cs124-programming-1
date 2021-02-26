#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 
#include <sys/time.h>
#include <chrono>

#include "./graph.cc"

void display(Graph g){

    printf("\nPRINTING EDGE WEIGHTS:\n");
    for(unsigned long i=0; i<g.n; i++){
        printf("[\t");
        for(int j=0; j<g.n; j++){
            printf("%f\t", g.calculate_edge(i,j));
        }
        printf("]\n");
    }
    
}


int main(int argc, char** argv) {
    srand(time(NULL));
    unsigned long n = std::stoi(argv[2]);
    int numtrials = std::stoi(argv[3]);
    int dim = std::stoi(argv[4]);

    double max = 0;  // to calculate k(n)
 
    double avg_mst_weight = 0;
    auto start_m = std::chrono::high_resolution_clock::now();
    for(int i = 0; i<numtrials; i++){
        Graph g = Graph(n, dim);
        avg_mst_weight += g.kruskal()/numtrials;
        if(dim<1) delete g.E;
        else delete g.V;

        if(g.max_weight>max){
            max = g.max_weight;
        }
    }
    auto stop_m = std::chrono::high_resolution_clock::now();
    auto t_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);

    printf("\nMAX WEIGHT: %f\n", max);
    printf("Predicted max weight: %f\n", weight_cap(n, dim));

    printf("\nNumber of vertices: %ld\n", n);
    printf("Dimensions: %d\n", dim);
    printf("Average MST Weight: %f\n", avg_mst_weight);
    printf("total time to run %d iterations (sec): %f\n\n", numtrials, (double) t_m.count() / 1e6);

}