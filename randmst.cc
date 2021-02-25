#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 

#include "./graph.cc"

void display(Graph g){
    
    /*printf("PRINTING VERTICES:\n");
    if(g.d >=1){
        for(int i = 0; i<g.n; i++){
            printf("vertex %d:\t", i);
            for(int k = 0; k<g.d; k++){
                printf("%f\t", g.V[i*g.d+k]);
            }
            printf("\n");
        }
    }*/
    

    printf("--------------------\nPRINTING EDGE WEIGHTS:\n");
    for(unsigned long i=0; i<g.n; i++){
        printf("[\t");
        for(int j=0; j<g.n; j++){
            printf("%f\t", g.calculate_edge(i,j));
        }
        printf("]\n");
    }
    
}


int main(int argc, char** argv) {
    int seed = std::stoi(argv[1]);
    srand(time(NULL)); // generate seed
    unsigned long n = std::stoi(argv[2]);
    int dim = std::stoi(argv[3]);

    Graph g = Graph(n, dim);
    //display(g);
    printf("\nKruskal's MST Weight: %f\n\n", g.kruskal());

    if(dim < 1){
        delete g.E;
    }
    else{
        delete g.V;
    }
}