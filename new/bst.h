/*
 * Author: Vamsi Krishna.B
 * AVL header file
 * 
 */
#include<gtk/gtk.h>
typedef struct node_struct{
    char* data;
    struct node_struct* left;
    struct node_struct* right;
} node;

char* get_tags(char* data,int flag);

int compare_by(char* data,char* root_data,int flag);

node* new_node(node* left,node* right,char* key);

node* bst_add_num(node* root, char* data,int flag);

void print_treestyle(node* root);

node* right_rotation(node* root);

node* left_rotation(node* root);

int print_at_distance(node* root,int distance,int steps);

int max(int a,int b);

node* any_node_to_up(node* root,char* data,int flag);

node* delete_node(node* root,char* data,int flag);

void print_inorder(node* root);

void print_inorder_for_tags(node* root,int flag);

node* destruct_node(node* root);

node* del_rec(node* root,node* main_root);

node* operate_on(node* root);

node* searching_node(node* root,char* string,int flag);

void print_after_searching(node* root,char* string,int flag,int flag_to_print);

//void file_into_clist(char* file,GtkCList* data);

void folder_into_clist(node* root_avl,GtkCList* data);


