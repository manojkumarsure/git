#include "Queue.h"
#include<malloc.h>
#include<string.h>
/*
Make a new node for the Queue and intitalize with data and its next pointer
@param: takes data value and pointer to next node
@return: return the current pointer of the node item
*/
Node* node_new( char* data, Node* next ){
        Node* temp;
                temp=(Node*)malloc(sizeof(Node));
                temp->data=(char*)malloc(sizeof(char)*strlen(data)+1);
                strcpy(temp->data,data);
                temp->next=next;
        return temp;

}

/*
Make a new Queue and allocates memory for the pointer and returns it
@param: no arguments are passed
@return: return the current pointer value of the Queue
*/

Queue* queue_new(){
        Queue *temp;
                temp=(Queue*)malloc(sizeof(Queue));
                temp->head=temp->tail=NULL;
        return temp;
}

/*
returns the size of the Queue by traversing by a while loop
@param: takes a pointer to the Queue
@return: return the size of the Queue
*/

int queue_size( Queue* st ){
int i=0;
                Node *temp=st->head;
                        while(temp!=NULL){
                                i++;
                                temp=temp->next;
                        }
        return i;

}
/*
Push a node at the end of Queue
@param: takes Queue pointer, index value as well as data to be set
@return: return the current pointer of the Queue
*/
Queue* queue_enqueue( Queue* st, char *string ){
        if(queue_size(st)==0){
                st->head=node_new(string,NULL);
                st->tail=st->head;
        return st;
        }

        st->tail->next=node_new(string,NULL);
        st->tail=st->tail->next;
return st;

}

/*
Pop an item at an end of the Queue
@param: takes Queue pointer
@return: return the current pointer of the list
*/
Queue* queue_dequeue( Queue* st ){
        if(queue_size(st)==0)
                return st;
        Node* temp;
                temp=st->head->next;
                free(st->head->data);free(st->head);
                st->head=temp;
        return st;

}
/*
Prints the data value of the list
@param: takes a pointer to the list
@return: returns void
*/
void queue_print( Queue* st ){
Node *temp=st->head;
                while(temp!=NULL){
                        printf("%s ",temp->data);
                        temp=temp->next;
                }

}
/*
To get value the value of the list at end of the Queue
@param: takes  a pointer to Queue and pointer to the integer to set the existence of the top value
@return: returns value of data item at the end of the Queue
*/
char* queue_top( Queue* st, int* error ){
        if(queue_size(st)==0)
                *error=1;
        else{
                *error=0;
        return st->head->data;
        }
}
/*
Delete entire Queue by freeing the allocated memory
@param: takes Queue pointer
@return: return nothing
*/
void queue_delete( Queue* q ){

        while(q->head!=NULL)
        queue_dequeue(q);

//free(q); //here queue is not released
}
