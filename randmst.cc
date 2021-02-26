#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 
#include <sys/time.h>
#include <chrono>

#include "./graph.cc"

void get_data(){

    const int numtests = 11;
    const int numtrials = 5;

    double weight_data[5][numtests];
    double time_data[5][numtests];

    for(int d = 0; d <= 4; d++){
        if(d!=1){
            for(unsigned long k = 7; k < 7+numtests; k++){
                double avg_mst_weight = 0;
                auto start_m = std::chrono::high_resolution_clock::now();
                for(int i = 0; i<numtrials; i++){
                    Graph g = Graph(pow(2,k), d);
                    avg_mst_weight += g.kruskal()/numtrials;
                    if(d<1) delete g.E;
                    else delete g.V;
                }
                auto stop_m = std::chrono::high_resolution_clock::now();
                auto t_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);

                weight_data[d][k-7] = avg_mst_weight;
                time_data[d][k-7] = (double) t_m.count() / 1e6;
            }
            printf("\n------DIMENSION %d------\n", d);
            for(int i=0; i<numtests; i++){
                printf("%d:\t%f\t%f\n", (int) pow(2,i+7), weight_data[d][i], time_data[d][i]);
            }
        }
    }
    printf("\n");
}


int main(int argc, char** argv) {
    srand(time(NULL));
    int testmode = std::stoi(argv[1]);
    unsigned long n = std::stoi(argv[2]);
    int numtrials = std::stoi(argv[3]);
    int dim = std::stoi(argv[4]);

    if(testmode==1){
        get_data();
    }
    else{
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

        printf("\nNumber of vertices: %ld\n", n);
        printf("Dimensions: %d\n", dim);
        printf("Average MST Weight: %f\n", avg_mst_weight);
        printf("total time to run %d iterations (sec): %f\n\n", numtrials, (double) t_m.count() / 1e6);
    }
}