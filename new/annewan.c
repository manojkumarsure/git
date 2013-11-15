#include<gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<malloc.h>
#include<mplib.h>
#include"Queue.h"
#include "bst.h"
enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};
GtkWidget *artists;
GtkWidget *albums;
GtkWidget *songs;
GtkWidget *window;
node *title_avl=NULL,*album_avl,*artist_avl;
static void
add_to_list(GtkWidget *list, const gchar *str)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

char* ret_dir(char *parent,char* child)
{
	char* temp=malloc(sizeof(char)*(strlen(parent)+strlen(child)+3));
	strcpy(temp,parent); 
	strcat(temp,"/");
	strcat(temp,child);
	return temp;
}


static void
remove_item(GtkWidget *widget, gpointer selection)
{
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;


  store = GTK_LIST_STORE(gtk_tree_view_get_model(
      GTK_TREE_VIEW (songs)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (songs));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
      return;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), 
      &model, &iter)) {
    gtk_list_store_remove(store, &iter);
  }
}
static void
init_list(GtkWidget *list,char* name)
{

  GtkCellRenderer    *renderer;
  GtkTreeViewColumn  *column;
  GtkListStore       *store;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(name,
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);

  store = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW (list), 
      GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void clearclist( gpointer data )
{
        gtk_clist_clear( (GtkCList *) data);
          return;
}

/*
void file_into_list(char* file,GtkCList* data,int flag){
char* temp;
if(flag==1){
temp=get_tags(file,1);
add_to_list((gpointer)albums, temp);

}else
if(flag==2){
temp=get_tags(file,2);
add_to_list((gpointer)albums, temp);

}

}*/
void folder_into_list(node* root,GtkCList* data,int flag){
if(root==NULL)return;
folder_into_list(root->left,data,flag);
if(flag==1){
char* temp;
temp=get_tags(root->data,1);
add_to_list(albums,temp);

}else
if(flag==2){
char* temp;
temp=get_tags(root->data,2);
add_to_list(artists,temp);
}
folder_into_list(root->right,data,flag);

}
void file_add(gpointer data)
{
	GtkWidget* dialog;
	dialog=gtk_file_chooser_dialog_new("choose an mp3 file",GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
		char* file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		title_avl=bst_add_num(title_avl,file,0);clearclist((gpointer)songs);folder_into_clist(title_avl,data);
	}
	gtk_widget_destroy(dialog);
}

node* traverse(char* directory,node* root_ptr,int flag){
 
Queue* q;int i;
q=queue_new();char* dir=NULL,*a=NULL;
DIR* dirp=NULL;
struct dirent * entry=NULL;
queue_enqueue(q,directory);
		while(queue_size(q)>0){
			dir=malloc(sizeof(char)*(strlen(queue_top(q,&i))+1));
			strcpy(dir,queue_top(q,&i));
//				printf("\n\n{-%s----------------\n\n",dir);
				dirp=opendir(dir);queue_dequeue(q);
					if(dirp==NULL){printf("Problem in opening the Directory");return;}
						while((entry=readdir(dirp))!=NULL){
								 if(strcmp(entry->d_name,".")==0)continue;
								 else
								  if(strcmp(entry->d_name,"..")==0)continue;
								  else
								  if(entry->d_type==DT_DIR){a=ret_dir(dir,entry->d_name);queue_enqueue(q,a);free(a);}
							          else{

						
						a=(char*)malloc((strlen(dir)+strlen(entry->d_name)+3)*sizeof(char));
						strcpy(a,dir);
						strcat(a,"/");
						strcat(a,entry->d_name);//printf("---%s--",a);
						//add_to_list(songs,a);
						if(strstr(a,".mp3"))
						{root_ptr=bst_add_num(root_ptr,a,flag);
						//printf("--%s \n",a);
//count++;
						}free(a);

} 
					}
			//printf("\n\n-------------------------------}count:%d\n\n",count);
		closedir(dirp);	
free(dir);  
		}
return root_ptr;
}

//for clearing clist

/*
//for clearing list
//list is global
static void
remove_all(GtkWidget *widget, gpointer selection,int flag)
{
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;
if(flag==1){
  store = GTK_LIST_STORE(gtk_tree_view_get_model(
      GTK_TREE_VIEW (albums)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (albums));
}
else
if(flag==2){
store = GTK_LIST_STORE(gtk_tree_view_get_model(
      GTK_TREE_VIEW (artists)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (artists));
}
  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
      return;
  gtk_list_store_clear(store);
}*/
void folder_add(gpointer data)
{
	GtkWidget* dialog;
	dialog=gtk_file_chooser_dialog_new("choose a folder",GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
		char* folder=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		title_avl=traverse(folder,title_avl,0);//traverse(folder,data);
		album_avl=traverse(folder,album_avl,1);
		artist_avl=traverse(folder,artist_avl,2);
		clearclist(data);		
		folder_into_clist(title_avl,data);//folder_into_list(album_avl,(gpointer)albums);folder_into_list(artist_avl,(gpointer)artists);

	}
	gtk_widget_destroy(dialog);
}
void selection_made( GtkWidget      *clist,
                     gint            row,
                     gint            column,
                     GdkEventButton *event,
                     gpointer        data )
{
    gchar *text;
    gtk_clist_get_text(GTK_CLIST(clist), row, column, &text);
    return;
}
int main(int argc,char *argv[])
{
	GtkWidget *sw1,*sw2,*sw3;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *hbox2;
	GtkWidget *addfile,*addfolder,*remove;
	GtkTreeSelection *selection;
	gchar* titles[3]={"Songs","Artists","Albums"};
	gtk_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	sw1=gtk_scrolled_window_new(NULL, NULL);
	sw2=gtk_scrolled_window_new(NULL, NULL);
	sw3=gtk_scrolled_window_new(NULL, NULL);
	artists=gtk_tree_view_new();
	albums=gtk_tree_view_new();
	songs=gtk_clist_new_with_titles(3,titles);
	gtk_clist_set_column_width (GTK_CLIST(songs), 0, 250);
	gtk_clist_set_column_width (GTK_CLIST(songs), 1, 250);
	gtk_window_set_title (GTK_WINDOW (window), "Music Library");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (window, 800, 600);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw1),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw1),
            GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw2),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw2),
            GTK_SHADOW_ETCHED_IN); 
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw3),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw3),
            GTK_SHADOW_ETCHED_IN);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(artists),TRUE);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(albums),TRUE);
	vbox=gtk_vbox_new(FALSE,3);
	hbox=gtk_hbox_new(TRUE,3);
	hbox2=gtk_hbox_new(TRUE,5);
	gtk_widget_set_size_request(hbox2,800,40);
	addfile=gtk_button_new_with_label("Add File");
	addfolder=gtk_button_new_with_label("Add Folder");
	remove=gtk_button_new_with_label("Remove");
	gtk_box_pack_start(GTK_BOX(hbox2),addfile,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),addfolder,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),remove,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox),sw2,TRUE,TRUE,5);
	gtk_box_pack_start(GTK_BOX(hbox),sw3,TRUE,TRUE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(vbox),sw1,TRUE,TRUE,5);
	gtk_container_add(GTK_CONTAINER(sw1),songs);
	gtk_container_add(GTK_CONTAINER(sw2),artists);
	gtk_container_add(GTK_CONTAINER(sw3),albums);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	init_list(albums,"Albums");
	init_list(artists,"Artists");
	gtk_signal_connect(GTK_OBJECT(songs), "select_row",
                       GTK_SIGNAL_FUNC(selection_made),
                       NULL);
	selection  = gtk_tree_view_get_selection(GTK_TREE_VIEW(albums));
    gtk_signal_connect_object(GTK_OBJECT(addfile), "clicked",
                   G_CALLBACK(file_add),(gpointer)songs);
    gtk_signal_connect_object(GTK_OBJECT(addfolder), "clicked",
                   G_CALLBACK(folder_add),(gpointer)songs);
	g_signal_connect(G_OBJECT(remove), "clicked",
                   G_CALLBACK(remove_item),selection);
    g_signal_connect (G_OBJECT (window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(songs);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}



