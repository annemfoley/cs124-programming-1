#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <time.h> 

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