#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 


struct DisjointSets{
    unsigned long * parents; // list of parent vertices
    int * ranks; // list of ranks
    int n; // number of vertices

    DisjointSets(unsigned long num_v){
        this->n = num_v;
        parents = (unsigned long *) malloc(sizeof(*parents) * n);
        ranks = (int *) malloc(sizeof(*ranks) * n);
        for(int i = 0; i<n; i++){
            parents[i] = i;
            ranks[i] = 0;
        }
    }

    unsigned long find(unsigned long vertex);
    unsigned long link(unsigned long v1, unsigned long v2);
    void set_union(unsigned long v1, unsigned long v2);

};

unsigned long DisjointSets::find(unsigned long vertex){
    if(parents[vertex]!=vertex){
        parents[vertex] = find(parents[vertex]);
    }
    return parents[vertex];
}

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

void DisjointSets::set_union(unsigned long v1, unsigned long v2){
    link(find(v1),find(v2));
}