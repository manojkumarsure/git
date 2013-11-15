/*
 * Author: Vamsi Krishna.B
 * AVL trees (modified rec_bst template)
 * 
 */

#include "bst.h"
#include <malloc.h>
#include<string.h>
#include<mplib.h>
#include<gtk/gtk.h>
//freeing tag list
char* freeing_tags(id3_tag_list* tag_list,char* data){
char* temp=NULL;
	temp=malloc(strlen(data)+1);
	strcpy(temp,data);
	mp_free_list __P((tag_list));
return temp;
}

//getting version 2 tags using flags

char* version2(id3_tag_list* tag_list,int flag){
if(flag==0){
	id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
		while(temp!=NULL){
			if(temp->data!=NULL){
				if(strcmp(temp->data->frame_id,"TIT2")==0)return (temp->data->data!=NULL)?temp->data->data:"No Title";}
			temp=temp->next;
			}
	return "No Title id";
}else
	if(flag==1){
	id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
		while(temp!=NULL){
			if(temp->data!=NULL){
				if(strcmp(temp->data->frame_id,"TALB")==0)return (temp->data->data!=NULL)?temp->data->data:"Unknow Album";}
			temp=temp->next;
		}
		return "No Album id";
}
else
if(flag==2){
	id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
		while(temp!=NULL){
			if(temp->data!=NULL){
				if(strcmp(temp->data->frame_id,"TPE1")==0)return (temp->data->data!=NULL)?temp->data->data:"Unkown Artist";}
			temp=temp->next;
		}
	return "No Artist id";
}

}

//getting tags from files using flags

char* get_tags(char* data,int flag){
id3_tag_list* tag_list_data=mp_get_tag_list_from_file __P((data));
		if(tag_list_data==NULL)return freeing_tags(tag_list_data,"No Tags");
	
if(flag==0){
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
/*check here*/			if(tag_list_data->tag->version==2){
				if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->title)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->title):"Title(null)"));
				else return freeing_tags(tag_list_data,version2(tag_list_data,flag));

			}
			else return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->title)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->title):"Title(null)"));
}else
if(flag==1){
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
				if(tag_list_data->tag->version==2){
				if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->album)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->album):"Album(null)"));
				else return freeing_tags(tag_list_data,version2(tag_list_data,flag));

			}
			else return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->album)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->album):"Album(null)"));
}else
if(flag==2){
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
				if(tag_list_data->tag->version==2){
				if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->artist)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->artist):"Artist(null)"));
				else return freeing_tags(tag_list_data,version2(tag_list_data,flag));
			}
			else return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->artist)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->artist):"Artist(null)"));
}

}

//print_in order with tags
void print_inorder_for_tags(node* root,int flag){
if(root==NULL)return;
	print_inorder_for_tags(root->left,flag);
printf("%s\n",get_tags(root->data,flag));
	print_inorder_for_tags(root->right,flag);

}

//compare function with flags

int compare_by(char* data,char* root_data,int flag){
	
if(flag==0)
	return strcmp(get_tags(data,flag),get_tags(root_data,flag));
else
if(flag==1)
	return strcmp(get_tags(data,flag),get_tags(root_data,flag));
else
if(flag==2)
	return strcmp(get_tags(data,flag),get_tags(root_data,flag));
}

/*
return the maximum of two numbers
@param: Two data values to return maximum
@return: return the maximum value
*/

int max(int a,int b){
	if(a<b)return b;
	else return a;
}
//return depth of node
int depth(node* root){
if(root==NULL)return 0;
	return (1+max(depth(root->left),depth(root->right)));

}
int factor(node* root){
if(root==NULL)return 0;
	return depth(root->left)-depth(root->right);
}

/*
Make a new node for the bst and intitalize with key and its next pointer to NULL
@param: takes data value and pointers to next nodes(left and right)
@return: return the current pointer of the node item
*/

node* new_node(node* left,node* right,char* data){
node * temp;
	temp=(node*)malloc(sizeof(node));
	temp->data=malloc(sizeof(char)*(strlen(data)+1));
	strcpy(temp->data,data);
	temp->left=left;
	temp->right=right;
return temp;
}
/*
Makes a left rotation at the paticular root node specified
@param: takes root node
@return: return pivot( i.e., current root ) after rotation
*/

node* left_rotation(node* root){
node* pivot;
	pivot=root->right;
	root->right=pivot->left;
	pivot->left=root;
return pivot;
}

/*
Makes a right rotation at particular root node specified
@param: takes root node
@return: return pivot( i.e., current root ) after rotation
*/
node* right_rotation(node* root){
node* pivot;
	pivot=root->left;
	root->left=pivot->right;	
	pivot->right=root;
	
return pivot;
}

/*
Add a key item to the current bst
@param: takes data value and pointer to root node
@return: return the root pointer of the node item
*/
node* bst_add_num(node* root,char* data,int flag){

if(root==NULL)
	return new_node(NULL,NULL,data);
if(compare_by(data,root->data,flag)<=0){
	root->left=bst_add_num(root->left,data,flag);
	if(factor(root)>1 && factor(root->left)>0){return right_rotation(root);}
	else
	if(factor(root)>1 && factor(root->left)<0){root->left=left_rotation(root->left);return right_rotation(root);}
	else
		return root;}
else{
	root->right=bst_add_num(root->right,data,flag);
	if(factor(root)<-1 && factor(root->right)>0){root->right=right_rotation(root->right);return left_rotation(root);}
	else
	if(factor(root)<-1 && factor(root->right)<0){return left_rotation(root);}
	else
		return root;}

}
//helping function,destruct a particular node
node* destruct_node(node* root){
node* temp;
	if(root->left==NULL)temp=root->right;
	else
	if(root->right==NULL)
		temp=root->left;
	else return root;
	free(root->data);
	free(root);
return temp;
}

//helps to find the successor and then delete
node* del_rec(node* root,node* main_root){
	if(root->left==NULL){
		free(main_root->data);
		main_root->data=malloc(sizeof(char)*(strlen(root->data)+1));
		strcpy(main_root->data,root->data);
	return destruct_node(root);
	}

	root->left=del_rec(root->left,main_root);
return root;
}

//helping function to delete the node

node* operate_on(node* root){
	if(root->left==NULL)
		return destruct_node(root);
	else
	if(root->right==NULL)
		return destruct_node(root);
	else
		root->right=del_rec(root->right,root);
return root;
}

///function to delete node in the tree

node* delete_node(node* root,char* data,int flag){
	if(root==NULL)return NULL;
		if(compare_by(root->data,data,flag)==0)return operate_on(root);
		else
		if(compare_by(data,root->data,flag)<0){
			root->left=delete_node(root->left,data,flag);
			if(factor(root)<-1 && factor(root->right)>=0){root->right=right_rotation(root->right);return left_rotation(root);}
			else
			if(factor(root)<-1 && factor(root->right)<=0){return left_rotation(root);}
			else
				return root;}
		else {
			root->right=delete_node(root->right,data,flag);
			if(factor(root)>1 && factor(root->left)>=0){return right_rotation(root);}
			else
			if(factor(root)>1 && factor(root->left)<=0){root->left=left_rotation(root->left);return right_rotation(root);}
			else
				return root;}
}


/*
Print a sequence of integers in one row which are at exact distance of 'distance' from node
@param: takes root node,distance to be seen, and a step counter
@return: return -1 if there are no nodes any more at distance+1 else return distance+1
*/
int print_at_distance(node* root,int distance,int steps){
	if(root==NULL)return -1;
	if(distance==steps){
			printf("%s ",get_tags(root->data,1));
		if(!root->left && !root->right)return -1;
		else return distance+1;
	}
return max(print_at_distance(root->right,distance,steps+1),print_at_distance(root->left,distance,steps+1)) ;
}

/*
Prints the tree in similar fashion each row by row
@param: takes root node
@return: return nothing
*/

void print_treestyle(node* root){
	int i=0;
	while(i!=-1){
		i=print_at_distance(root,i,0);
		//printf("\b");
		  printf("\n");
	}
}




/*
Shifts the last entered node towards up
@param: takes root node, and the key of last node
@return: return the current root node after all the rotations
*/
node* any_node_to_up(node* root,char *data,int flag){
if(root==NULL)
	return root;

root->left=any_node_to_up(root->left,data,flag);
root->right=any_node_to_up(root->right,data,flag);

	if(root->left!=NULL && compare_by(root->left->data,data,flag)==0)
		return right_rotation(root);
	else
	if(root->right!=NULL && compare_by(root->right->data,data,flag)==0)
		return left_rotation(root);
	else 
		return root;
}

//Inorder printing
void print_inorder(node* root){
if(root==NULL)return;
	print_inorder(root->left);
	printf(" %s --->\n",root->data);
	print_inorder(root->right);

}


//print found tags..
void print_after_searching(node* root,char* string,int flag,int flag_to_print){
if(root==NULL){return;}
else{
int compare=strcmp(string,get_tags(root->data,flag));
if(compare==0)
printf(":-%s\n",get_tags(root->data,flag_to_print));
print_after_searching(root->left,string,flag,flag_to_print);
print_after_searching(root->right,string,flag,flag_to_print);
}
}
//searching..
node* searching_node(node* root,char* string,int flag){
if(root==NULL)return NULL;
int compare=strcmp(string,get_tags(root->data,flag));
if(compare==0)return root;
else
if(compare<0)return searching_node(root->left,string,flag);
else
return searching_node(root->right,string,flag);

}
/*
void file_into_clist(char* file,GtkCList* data){
char* d[3]={get_tags(file,0),get_tags(file,2),get_tags(file,1)};
gtk_clist_append((GtkCList *) data,d);
}
*/
void folder_into_clist(node* root,GtkCList* data){
if(root==NULL)return;
folder_into_clist(root->left,data);
char* d[3]={get_tags(root->data,0),get_tags(root->data,2),get_tags(root->data,1)};
gtk_clist_append((GtkCList *) data,d);
folder_into_clist(root->right,data);
}


