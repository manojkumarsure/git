#ifndef QUEUE_H
#define QUEUE_H


//Node for Queue
typedef struct Node_ Node;
struct Node_ {
    char* data;
    Node* next;
};

// Create a new node with data and next element (can be NULL)
Node* node_new( char* data, Node* next );

//Queue data structure with head and tail pointer
typedef struct Queue_ {
Node *head,*tail;
} Queue;

// Create a new empty queue
Queue* queue_new();
// Deletes the queue, frees memory.
void queue_delete( Queue* q );

// Inserts @val to the back of the queue
Queue* queue_enqueue( Queue* st, char* string );

// Remove the element at the front of the queue - also frees memory
Queue* queue_dequeue( Queue* st );

// Returns the element currently at the top of the queue. If the queue is empty,
// @error should be set to 1, else 0.
char* queue_top( Queue* st, int* error );

// Returns the number of elements in the queue
int queue_size( Queue* st );

// Prints the elements currently in the queue
void queue_print( Queue* st );

#endif // queue_H