#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std; 

// Think about using Fibonacci heaps
// add n = 1 when generating a graph

struct heapNode {
  double key;
  unsigned long vertexId;
  unsigned long connectingVertexId;
};

struct heap {
  unsigned long size;
  unsigned long length;
  unsigned long* locations; // (index, value) = (graph node number, location in heap)
  struct heapNode* arr;
};

struct graphEdge {
  unsigned long id;
  double weight;
  struct graphEdge* next;
};

struct graphList {
  struct graphEdge* head;
};

struct graph {
  unsigned long dim;
  unsigned long vertices;
  struct graphList* arr;
};

graphEdge* initNewNode(unsigned long id, double weight) {
  graphEdge* newNode = new graphEdge;
  newNode -> id = id;
  newNode -> weight = weight;
  newNode -> next = NULL;
  return newNode;
}

void addEdge(graph* G, unsigned long v1, unsigned long v2, double weight)
{
  graphEdge* newNode = initNewNode(v1, weight);
  newNode->next = G->arr[v2].head;
  G->arr[v2].head = newNode;

  newNode = initNewNode(v2, weight);
  newNode->next = G->arr[v1].head;
  G->arr[v1].head = newNode;
}

graph* generateRandomGraph(unsigned long d, unsigned long n)
{
  graph* G = new graph;
  G->vertices = n;
  G->dim = d;
  G->arr = new graphList[n];

  double verticeLocations[d*n];
  double weight;

  if(d == 1)
  {
    for(unsigned long i = 0; i < n; i++)
    {
      for(unsigned long j = 0; j < i; j++)
      {
        addEdge(G, i, j, ((double) rand() / (RAND_MAX)));
      }
    }
  } else {
    for(unsigned long i = 0; i < n; i++)
    {
      G->arr[i].head = NULL;
      // make new vertex
      for(unsigned long j = 0; j < d; j++)
      {
        verticeLocations[d*i + j] = ((double) rand() / (RAND_MAX));
      }
      
      for(unsigned long j = 0; j < i; j++)
      {
        weight = 0;
        for(unsigned long k = 0; k < d; k++)
        {
          weight += pow(verticeLocations[d*i + k] - verticeLocations[d*j + k], 2);
        }
        addEdge(G, i, j, sqrt(weight));
      }
    }
  }
  return G;
}

void printGraph(graph* G) 
{ 
  graphEdge* currentEdge;
  for (unsigned long v1 = 0; v1 < G->vertices; v1++) 
  { 
      printf("Vertice %lu:\n", v1);
      currentEdge = G->arr[v1].head;
      while (currentEdge != NULL) 
      { 
        printf("\tID: %lu, WEIGHT: %f\n", currentEdge->id, currentEdge->weight);
        currentEdge = currentEdge->next;
      } 
      printf("\n");
  } 
} 

/*

MIN PRIORITY QUEUE

*/

// This set of functions takes in an index of a heapNode in the heap and returns the index of the parent, left child, and right child of that heapNode

unsigned long leftChildIndex(unsigned long parentIndex){
  return 2 * (parentIndex + 1) - 1;
}

unsigned long rightChildIndex(unsigned long parentIndex){
  return 2 * (parentIndex + 1);
}

unsigned long parentIndex(unsigned long childIndex){
  return floor((childIndex + 1) / 2) - 1;
}

// Usually, the parentIndex will be 0 in this function. That means the top of the priority queue, if it is a large number, will be bubbled down to maintain the min-heap property

void minHeapify(unsigned long parentIndex, heap* H){

  //Initialize the index variables
  unsigned long leftIndex;
  unsigned long rightIndex;
  unsigned long minIndex;

  while(true){
    // Get the left child and right child of the parent we are working with
    leftIndex = leftChildIndex(parentIndex);
    rightIndex = rightChildIndex(parentIndex);

    // If (1) the left child is inside the priority queue, and (2) the left child's key is MORE MIN than the parent... 
    if (leftIndex < H->size && H->arr[leftIndex].key < H->arr[parentIndex].key){
      // ...then let minIndex be the left child's index
      minIndex = leftIndex;
    } 
    // Consider the right child
    else if (rightIndex < H->size && H->arr[rightIndex].key < H->arr[parentIndex].key){
      minIndex = rightIndex;
    }
    // If the first two do not apply -> The parent is MORE MIN than both its children, so it can rest at its position in the heapNode, and we break out of the while loop and the function terminates
    else {
      break;
    }
    
    // This chunk of code executes when at least one of the children is MORE MIN than its parent. We want to swap them in the priority queue, and also in H->locations.

    // Swapping their nodes in the priority queue
    heapNode tempHeapNode = H->arr[parentIndex];
    H->arr[parentIndex] = H->arr[minIndex];
    H->arr[minIndex] = tempHeapNode;

    // Swapping their associated locations
    H->locations[H->arr[parentIndex].vertexId] = parentIndex;
    H->locations[H->arr[minIndex].vertexId] = minIndex;

    // Now, we consider the child as the parent! (This is how we "bubble down")
    parentIndex = minIndex;
  }
}

// Basic function for printing the heap to analyze

void printHeap(heap* H)
{
  for(unsigned long i = 0; i < H->length; i++)
  {
    printf("v: %lu, cv: %lu, Key: %f\n", H->arr[i].vertexId, H->arr[i].connectingVertexId, H->arr[i].key);
  }
}

// We extract the vertex with the minimum, minimum associated edge weight

// In other words, we take the first heapNode from the priority queue, and we ensure that the min-heap property is maintained

heapNode heapExtractMin(heap* H)
{
  // H->arr[0] is at the front of the priority queue, and therefore is the minimum
  heapNode minHeapNode = H->arr[0];

  // We take the back of the priority queue (usually quite large!) and put it at the very front (where the minimum of the heap SHOULD belong)
  H->arr[0] = H->arr[H->size - 1];

  // We throw the minimum to the back of the array, and thus OUT of the heap node
  H->arr[H->size - 1] = minHeapNode;

  // Change the locations
  H->locations[H->arr[0].vertexId] = 0;
  H->locations[H->arr[H->size - 1].vertexId] = H->size - 1;

  // We decrease the size of the priority queue by 1 due to the extraction
  H->size -= 1;

  // Now, we bubble down the 'quite large!' value at the front back to where it belongs, and thereby maintain the min-heap property
  minHeapify(0, H);

  // Returns the heapNode with the minimum associated edge weight
  return minHeapNode;
}

// We insert a new key into a chosen heapNode that is SMALLER than it's previous key

void heapDecreaseKey(heap* H, unsigned long index, double newKey)
{
  H->arr[index].key = newKey;
  unsigned long parent = parentIndex(index);

  // To maintain the min-heap property, we will bubble up the changed heapNode to where it should be
  while ((index > 0) && (H->arr[index].key < H->arr[parent].key))
  {
    // if the child is more min than the parent, swap the two heapNodes in the heap

    parent = parentIndex(index);
    heapNode temp = H->arr[index];
    H->arr[index] = H->arr[parent];
    H->arr[parent] = temp;

    // make sure you swap their locations in H->locations as well
    H->locations[H->arr[index].vertexId] = index;
    H->locations[H->arr[parent].vertexId] = parent;

    // now onto the parent (part of bubbling up)
    index = parentIndex(index);
  }
}

// Prim returns the weight of the MST of a graph G

double Prim(graph* G)
{
  // First, initialize a heap for the graph
  heap* H = new heap;
  // We want the size of the heap to AT FIRST be the number of vertices in G, aka all vertices in G start off in the priority queue
  H->size = G->vertices;
  // The length will always the number of vertices in G.
  H->length = G->vertices;
  H->arr = new heapNode[H->length];
  // The index of H->locations gives the vertexId, and the value gives the location of that vertexId in H->arr. 
  // For instance, H->locations[2] gives the location of the vertex with an id = 2 in H->arr
  H->locations = new unsigned long[H->length];

  // The priority queue will at first hold all of the vertices in a graph G, and 
  // (1) each vertex will have a key of 2 (because we can only go down from there, since the edge weight is capped at 1), an
  // (2) each vertex will have a connectingVertexId of -1 (aka none)
  // (3) the vertexId will be its index in heap->arr
  // (4) and so the current location of a vertex with id X in heap->arr will be X

  // connectingVertexId is: Consider all the edges connected to a vertex v. Out of all the edges, what is the edge with the minimum weight?
  // key is: What is that minimum weight?

  for(unsigned long i = 0; i < H->length; i++)
  {
    H->arr[i].key = 2;
    H->arr[i].connectingVertexId = H->length;
    H->arr[i].vertexId = i;
    H->locations[i] = i;
  }

  // Initilaize the weight of the tree
  double weightOfTree = 0;

  // While the priority queue is not empty...
  unsigned long u;
  while(H->size != 0)
  {
    // Consider a vertex u where key(u) less than or equal to all v for v in V
    heapNode minHeapNode = heapExtractMin(H);
    u = minHeapNode.vertexId;

    if(minHeapNode.connectingVertexId != H->length)
    {
      weightOfTree += minHeapNode.key;
    }

    // For every vertex v connected to u...
    unsigned long v;
    graphEdge* currentEdge = G->arr[u].head;

    while(currentEdge != NULL)
    {
      v = H->locations[currentEdge->id];
      //printf("We are on %i where v: %i is at %i\n", u, currentEdge->id, v);
      // If (1) v is in the priority queue and (2) the weight of the edge uv is less than key(v)
      if (v < H->size && currentEdge->weight < H->arr[v].key)
      {
        // key(v) is set equal to uv
        H->arr[v].key= currentEdge->weight;
        // connectingvertexId(v) is set equal to u
        H->arr[v].connectingVertexId = u;
        heapDecreaseKey(H, v, currentEdge->weight); // maintain the min-heap property
        //printf("Activated!\n");
      }
      //printHeap(H);
      //printf("\n");
      currentEdge = currentEdge->next;
    }
  }
  //printHeap(H);
  //printf("\n");
  return weightOfTree;
}

int main() {
  srand(42);
  graph* G = generateRandomGraph(3, 65536);
  //printGraph(G);
  printf("%f\n", Prim(G));
}

