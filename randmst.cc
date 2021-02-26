#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 
#include <sys/time.h>
#include <chrono>

#include "./graph.cc"

// this function is to get data for graphs with n from 128 to 262144
void get_data(){

    const int numtests = 12;
    const int numtrials = 5;

    double weight_data[5][numtests];
    double time_data[5][numtests];

    // run numtests on all dimensions 0, 2-4
    for(int d = 0; d <= 4; d++){
        if(d!=1){
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