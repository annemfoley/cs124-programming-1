#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 

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
    unsigned long n = std::stoi(argv[2]);
    int numtrials = std::stoi(argv[3]);
    int dim = std::stoi(argv[4]);

    
    double avg_mst_weight = 0;
    for(int i = 0; i<numtrials; i++){
        srand(time(NULL));
        Graph g = Graph(n, dim);
        avg_mst_weight += g.kruskal()/numtrials;
        if(dim<1) delete g.E;
        else delete g.V;
    }
    printf("\nAverage MST Weight: %f\n\n", avg_mst_weight);
}