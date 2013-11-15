/* **************************************************************
 * Music Library
 * Authors:
 * CS12B028------SURE MANOJ KUMAR
 * CS12B057------VAMSI KRISHNA BOKAM
 * CS12B032------ABHISHEK YADAV
 * Purpose-
 * arranges mp3 files in various sorted orders based on it's tags
 * searching,removing an mp3 file  can be done
 * you can add a file or a folder to this
 * playing is done using default music player
 * Last Date of Modification-13/11/2013
 * *************************************************************/
#include<gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<malloc.h>
#include<mplib.h>
#include"Queue.h"
enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};
GtkWidget *artists;
GtkWidget *albums;
GtkWidget *songs;
GtkWidget *window,*entry;
char a[100];

//copies data to a temp,frees the taglist and return the temp
char* freeing_tags(id3_tag_list* tag_list,char* data)
{
	char* temp=NULL;
	temp=malloc(strlen(data)+1);
	strcpy(temp,data);
	mp_free_list __P((tag_list));
	return temp;
}

//removes the end spaces,tabs,newline charectars of a string
void str_pro(char* s)
{
	if(s!=NULL)
	{
		while(1)
		{
			if(s[strlen(s)-1]=='\t' || s[strlen(s)-1]==' ' || s[strlen(s)-1]=='\n')
			{
				s[strlen(s)-1]=0;
			}
			else 
				break;
		}
	}
}

//processing the string if it contains [ or ] charectars
char* str_prosq(char* s)
{
	char *s2;
	s2=(char*)malloc(200*sizeof(char));
	int i=0,j=0;
	while(i<strlen(s))
	{
		if(s[i]!='[' && s[i]!=']')
		{
			s2[j]=s[i];
			i++;
			j++;
			continue;
		}
		else
		{
			s2[j]='\\';
			s2[++j]=s[i];
			i++;
			j++;
		}
	}
	s2[j]='\0';
	return s2;
}

//getting version 2 tags using flags
char* version2(id3_tag_list* tag_list,int flag)
{
	if(flag==0)
	{
		id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
		while(temp!=NULL)
		{
			if(temp->data!=NULL)
			{
				if(strcmp(temp->data->frame_id,"TIT2")==0)
					return (temp->data->data!=NULL)?temp->data->data:"No Title";
			}
			temp=temp->next;
		}
		return "No Title";
	}
	else
		if(flag==1)
		{
			id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
			while(temp!=NULL)
			{
				if(temp->data!=NULL)
				{
					if(strcmp(temp->data->frame_id,"TALB")==0)
						return (temp->data->data!=NULL)?temp->data->data:"Unknow Album";
				}
				temp=temp->next;
			}
			return "No Album id";
		}
	else
	if(flag==2)
	{
		id3v2_frame_list *temp=(((id3v2_tag*)(tag_list->tag->tag))->frame_list);
		while(temp!=NULL)
		{
			if(temp->data!=NULL)
			{
				if(strcmp(temp->data->frame_id,"TPE1")==0)
					return (temp->data->data!=NULL)?temp->data->data:"Unkown Artist";
			}
			temp=temp->next;
		}
		return "No Artist id";
	}
}

//getting tags from files using flags
char* get_tags(char* data,int flag)
{
	id3_tag_list* tag_list_data=mp_get_tag_list_from_file __P((data));
	if(tag_list_data==NULL)
		return freeing_tags(tag_list_data,"No Tags");
	if(flag==0)
	{
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )
				return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
				if(tag_list_data->tag->version==2)
				{
					if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)
						return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->title)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->title):"No Title"));
					else 
						return freeing_tags(tag_list_data,version2(tag_list_data,flag));
				}
			else 
				return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->title)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->title):"No Title"));
	}
	else
	if(flag==1)
	{
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )
				return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
				if(tag_list_data->tag->version==2)
				{
					if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)
						return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->album)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->album):"Album(null)"));
					else 
						return freeing_tags(tag_list_data,version2(tag_list_data,flag));
			}
			else 
				return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->album)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->album):"Album(null)"));
	}
	else
	if(flag==2)
	{
			if(tag_list_data->tag->version!=1 && tag_list_data->tag->version!=2 )
				return freeing_tags(tag_list_data,"Tags Unsupported ");
			else
				if(tag_list_data->tag->version==2)
				{
					if(tag_list_data->next!=NULL && tag_list_data->next->tag!=NULL)
						return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->next->tag->tag))->artist)!=NULL)?(((id3v1_tag*)(tag_list_data->next->tag->tag))->artist):"Artist(null)"));
					else 
						return freeing_tags(tag_list_data,version2(tag_list_data,flag));
				}
			else 
				return freeing_tags(tag_list_data,(((((id3v1_tag*)(tag_list_data->tag->tag))->artist)!=NULL)?(((id3v1_tag*)(tag_list_data->tag->tag))->artist):"Artist(null)"));
	}
}

//Adding a string to the list
static void add_to_list(GtkWidget *list, const gchar *str)
{
  GtkListStore *store;
  GtkTreeIter iter;
  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

//returns the directory name by combining parent and child directories
char* ret_dir(char *parent,char* child)
{
	char* temp=malloc(sizeof(char)*(strlen(parent)+strlen(child)+3));
	strcpy(temp,parent); 
	strcat(temp,"/");
	strcat(temp,child);
	return temp;
}

//to clear the clist
void clearclist( gpointer data )
{
        gtk_clist_clear( (GtkCList *) data);
          return;
}

//to remove all the elements from the list
static void remove_all(GtkWidget *list)
{
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;


  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (list)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (list));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
      return;
  gtk_list_store_clear(store);
}

//updating all the panes when an event is done
void paneup(char* file)
{
	FILE* fp=fopen(file,"r");int i;char* s;
	s=(char*)malloc(300*sizeof(char));
	char* s1,*s2,*s3,*s4;
	if((strcmp(file,"global_songs.txt")==0) || (strcmp(file,"after_search.txt")==0))
	{
		while(fgets(s,298,fp)!=NULL)
		{
			char* al;
			al=strtok(s,"|");
				s1=al;
				al=strtok(NULL,"|");
				s2=al;
				al=strtok(NULL,"|");
				s3=al;
				al=strtok(NULL,"|");
				s4=al;
				al=strtok(NULL,"|");
				str_pro(s1);
				str_pro(s2);
				str_pro(s3);
				str_pro(s4);
				FILE* fpp;
				fpp=fopen(s4,"rb");
				if(fpp!=NULL)
				{
					char*d[4]={s1,s2,s3,s4};
					gtk_clist_append(GTK_CLIST(songs),d);
					fclose(fpp);
				}
				
		}
	}
	else
	{
		if(strcmp(file,"Albums.txt")==0)
		{
			while(fgets(s,298,fp)!=NULL)
			{
				str_pro(s);
				add_to_list(albums,s);
			}
		}
		else
		{
			if(strcmp(file,"Artists.txt")==0)
			{	
				while(fgets(s,298,fp)!=NULL)
				{
					str_pro(s);
					add_to_list(artists,s);
				}
			}
		}
	}
fclose(fp);
}
char art[100]="",alb[100]="";//to store the selected artists or albums
gchar *text="";
void alball();
void artall();

//when an artist or album is selected it searches for them and updates the clist
void artist_album_selection()
{
	char s[400];
	strcpy(art,str_prosq(art));
	strcpy(alb,str_prosq(alb));
	char* anartist=(char*)malloc(200*sizeof(char));
	char* analbum=(char*)malloc(200*sizeof(char));
	if(strcmp(alb,"")==0)
	{
		sprintf(s,"grep -i \"%s\" global_songs.txt > after_search.txt",art);
		system(s);
		gtk_clist_clear(GTK_CLIST(songs));
		paneup("after_search.txt");
		return;
	}
	if(strcmp(art,"")==0)
	{
		sprintf(s,"grep \"%s\" global_songs.txt > after_search.txt",alb);
		gtk_clist_clear(GTK_CLIST(songs));
		system(s);
		paneup("after_search.txt");
		return;
	}
	sprintf(s,"grep \"%s\" global_songs.txt | grep \"%s\" > after_search.txt",art,alb);
	system(s);
	gtk_clist_clear(GTK_CLIST(songs));
    paneup("after_search.txt");
}

//select function for artists
void sel(GtkWidget *widget, gpointer label) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  strcpy(alb,"");
  char* value;
  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
  {
    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
	strcpy(art,value);
  }
  artist_album_selection();
}

//select function for albums
void sel2(GtkWidget *widget, gpointer label) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char* value;
  strcpy(art,"");
  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
   strcpy(alb,value);
  }
	artist_album_selection();
}

//function for the remove button it removes the selected file and update the panes
void remove_button()
{
	if(strcmp(text,"")!=0)
	{
		char s[500];
		str_pro((char*)text);
		sprintf(s,"grep \"%s\" global_songs.txt > remove_global.txt",(char*)text);
		system(s);
		system("sort remove_global.txt global_songs.txt | uniq -u > temp.txt");
		system("mv temp.txt global_songs.txt");
		unlink("remove_global.txt");
		FILE* fp1=fopen("global_songs.txt","r");
		FILE* fp2=fopen("remove_album.txt","w+");
		FILE* fp3=fopen("remove_artist.txt","w+");
		while(fgets(s,298,fp1)!=NULL)
		{
			char* al,*artist,*album;
			strtok(s,"|");
			album=strtok(NULL,"|");fprintf(fp2,"%s\n",album);
			artist=strtok(NULL,"|");fprintf(fp3,"%s\n",artist);
		}
		fclose(fp1);fclose(fp2);fclose(fp3);
		system("sort -u remove_album.txt > album.temp");
		system("mv album.temp Albums.txt");
		system("rm -f remove_album.txt");
		system("sort -u remove_artist.txt > artist.temp");
		system("mv artist.temp Artists.txt");
		system("rm -f remove_artist.txt");
		gtk_clist_clear(GTK_CLIST(songs));
		remove_all(albums);
		remove_all(artists);
	    paneup("global_songs.txt");
		paneup("Artists.txt");
		paneup("Albums.txt");

	}
}

//updates the files by sorting them
void update(char* dest,char* source)
{
	FILE* fp=fopen(dest,"a+");fclose(fp);
	char s[300];
	sprintf(s,"sort -t \'|\' -i -f  -u %s %s -k1,3> buffer.txt",dest,source);
	system(s);
	rename("buffer.txt",dest);
	unlink(source);
}

int revso=1,reval=1,revar=1;//to store if the file is sorted in ascending or descending

//TO SORT THE SORT global_songs FILE USING flag(=1,2,3) AND FLAG TO REVERSE OR NOT
void sort_global(int flag,int reverse)
{
	char s[300];
	char *d=(reverse)?"-r" :"";
	sprintf(s,"sort -t \'|\' %s -k%d,%d -k1,1 after_search.txt > temporary.txt",d,flag,flag);
	system(s);
	rename("temporary.txt","after_search.txt");
	gtk_clist_clear(GTK_CLIST(songs));
	paneup("after_search.txt");
}

//to sort the clist based on songs
void sort_global_songs()
{	
	revso=1-revso;
	sort_global(1,revso);
}

//to sort the clist based on artists
void sort_global_artists()
{	
	revar=1-revar;
	sort_global(3,revar);
}

//to sort the clist based on albums
void sort_global_albums()
{	
	reval=1-reval;
	sort_global(2,reval);
}

//returns the file name with complete path
char* ret_file_name(char* data)
{
	char* toks;
	toks=strtok(data,"|");
	while(strstr(toks,".mp3")!=0)toks=strtok(NULL,"|");
	return toks;
}

//addition of a file to the library
void file_add(gpointer data)
{
	FILE* fp=fopen("temp.txt","w+");
	FILE* fp1=fopen("tempI.txt","w+");
	FILE* fp2=fopen("tempII.txt","w+");
	GtkWidget* dialog;
	dialog=gtk_file_chooser_dialog_new("choose an mp3 file",GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
		char* file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)),*temp1,*temp2,*title_check;
		if(strstr(file,".mp3"))
		{
			temp1=(char*)malloc(200*sizeof(char));
			temp2=(char*)malloc(200*sizeof(char));
			temp1=get_tags(file,1);temp2=get_tags(file,2),title_check=get_tags(file,0);
			str_pro(temp1);
			str_pro(temp2);
			str_pro(title_check);
			if(strcmp(temp1,"")==0)
				temp1="Unknown";
			if(strcmp(temp2,"")==0)
				temp2="Unknown";
			fprintf(fp,"%s|%s|%s|%s\n",(strcmp(title_check,"No Title")==0 || strcmp(title_check,"")==0)?ret_file_name(file):title_check,temp1,temp2,file);

			fprintf(fp1,"%s\n",temp1);fprintf(fp2,"%s\n",temp2);
		}	
	}
	gtk_widget_destroy(dialog);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);

	update("global_songs.txt","temp.txt");
	update("Albums.txt","tempI.txt");
	update("Artists.txt","tempII.txt");
	gtk_clist_clear(GTK_CLIST(songs));
	remove_all(albums);
	remove_all(artists);
	paneup("global_songs.txt");
	paneup("Artists.txt");
	paneup("Albums.txt");
	system("cp global_songs.txt after_search.txt");
}

//function to traverse the whole directory added
void traverse(char* directory)
{
	FILE* fp=fopen("temp.txt","w+");
	FILE* fp1=fopen("tempI.txt","w+");
	FILE* fp2=fopen("tempII.txt","w+");

	Queue* q;int i;
	q=queue_new();char* dir=NULL,*a=NULL,*temp1,*temp2,*title_check;
	temp1=(char*)malloc(200*sizeof(char));
				temp2=(char*)malloc(200*sizeof(char));
	DIR* dirp=NULL;
	struct dirent * entry=NULL;
	queue_enqueue(q,directory);
	while(queue_size(q)>0)
	{
			dir=malloc(sizeof(char)*(strlen(queue_top(q,&i))+1));
			strcpy(dir,queue_top(q,&i));
			dirp=opendir(dir);queue_dequeue(q);
			if(dirp==NULL)
			{
				printf("Problem in opening the Directory");
				return;
			}
			while((entry=readdir(dirp))!=NULL)
			{
				if(strcmp(entry->d_name,".")==0)continue;
				else
				if(strcmp(entry->d_name,"..")==0)continue;
				else
				if(entry->d_type==DT_DIR)
				{	
					a=ret_dir(dir,entry->d_name);
					queue_enqueue(q,a);
					free(a);
				}
				else
				{
					a=(char*)malloc((strlen(dir)+strlen(entry->d_name)+3)*sizeof(char));
					strcpy(a,dir);
					strcat(a,"/");
					strcat(a,entry->d_name);
					if(strstr(a,".mp3"))
					{
						temp1=get_tags(a,1);
						temp2=get_tags(a,2);
						title_check=get_tags(a,0);
						str_pro(temp1);
						str_pro(temp2);
						str_pro(title_check);
						if(strcmp(temp1,"")==0)
							strcpy(temp1,"Unknown");
						if(strcmp(temp2,"")==0)
							strcpy(temp2,"Unknown");
						
						fprintf(fp,"%s|%s|%s|%s\n",(strcmp(title_check,"No Title")==0 || strcmp(title_check,"")==0 )?entry->d_name:title_check,temp1,temp2,a);
						fprintf(fp1,"%s\n",temp1);
						fprintf(fp2,"%s\n",temp2);
					}
					free(a);

				} 
			}
			closedir(dirp);	
			free(dir);  
		}
		fclose(fp);
		fclose(fp1);
		fclose(fp2);

		update("global_songs.txt","temp.txt");
		update("Albums.txt","tempI.txt");
		update("Artists.txt","tempII.txt");
		return;
}

//to remove an item from the list
static void remove_item(GtkWidget *widget, gpointer selection)
{
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;
  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (songs)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (songs));
  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
      return;
  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection),&model, &iter)) 
  {
    gtk_list_store_remove(store, &iter);
  }
}

//to initialise the list
static void init_list(GtkWidget *list,char* name)
{

  GtkCellRenderer    *renderer;
  GtkTreeViewColumn  *column;
  GtkListStore       *store;
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(name,renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);
  store = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW (list),GTK_TREE_MODEL(store));
  g_object_unref(store);
}

//adding a folder to the library
void folder_add(gpointer data)
{
	GtkWidget* dialog;
	dialog=gtk_file_chooser_dialog_new("choose a folder",GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
		char* folder=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		traverse(folder);
		gtk_clist_clear(GTK_CLIST(songs));
		remove_all(albums);
		remove_all(artists);
	    paneup("global_songs.txt");
		paneup("Albums.txt");
		paneup("Artists.txt");
		system("cp global_songs.txt after_search.txt");
	}
	gtk_widget_destroy(dialog);
}

//to remove all the files related to the library
void allremove()
{
		system("rm Albums.txt Artists.txt global_songs.txt after_search.txt");
		gtk_clist_clear(GTK_CLIST(songs));
		remove_all(albums);
		remove_all(artists);
}	

//to play the song using default music player
void songplay()
{
	char s[300];
	str_pro((char*)text);
	sprintf(s,"xdg-open \"%s\"",(char*)text);
	system(s);
}

//if a selection is made on the clist;it returns the location of the selected file
void selection_made( GtkWidget      *clist,
                     gint            row,
                     gint            column,
                     GdkEventButton *event,
                     gpointer        data )
{
    gtk_clist_get_text(GTK_CLIST(clist), row,3, &text);
	if(event->type==GDK_2BUTTON_PRESS)
	{
		songplay();
	}
    return;
}

//when all albums button is clicked;clist will be updated to all songs present in library
void alball()
{
	remove_all(albums);
	strcpy(alb,"");
	gtk_clist_clear(GTK_CLIST(songs));
	paneup("Albums.txt");
	paneup("global_songs.txt");
	system("cp global_songs.txt after_search.txt");
}

//when all albums button is clicked;clist will be updated to all artists present in the library
void artall()
{
	remove_all(artists);
	paneup("Artists.txt");
	strcpy(art,"");
	gtk_clist_clear(GTK_CLIST(songs));
	paneup("global_songs.txt");
	system("cp global_songs.txt after_search.txt");
}

//when a search is made it prints the results in clist
void searchbox(gpointer entry)
{
	const char* text;
	text=(char*)malloc(200*sizeof(char));
	text=gtk_entry_get_text(entry);
	char* s;
	s=(char*)malloc(200*sizeof(char));
	sprintf(s,"grep -i \"%s\" global_songs.txt > after_search.txt",text);
	system(s);
	gtk_clist_clear(GTK_CLIST(songs));
	paneup("after_search.txt");
}

//to quit the program
void quit()
{
	system("rm -f after_search.txt");
	gtk_main_quit();
}

//main function placing all widgets in correct positions and to add their respective callbacks
int main(int argc,char *argv[])
{
	GtkWidget *sw1,*sw2,*sw3;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *hbox2;
	GtkWidget *vpaned;
	GtkWidget *removeall;
	GtkWidget *new;
	GtkWidget *play,*search;
	GtkWidget *hbox3,*hbox4,*hbox5;
	GtkWidget *songsort,*albumsort,*artistsort;
	GtkWidget *addfile,*addfolder,*remove;
	GtkWidget* allartists,*allalbums;
	GtkTreeSelection *selection,*selection2;
	gchar* titles[4]={"Songs","Albums","Artists","location"};
	gtk_init(&argc,&argv);
	play=gtk_button_new_with_label("Play");
	allartists=gtk_button_new_with_label("Artists");
	allalbums=gtk_button_new_with_label("Albums");
	new=gtk_button_new_with_label("Songs");
	entry=gtk_entry_new();
	search=gtk_button_new_with_label("search");
	vpaned=gtk_vpaned_new();
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	sw1=gtk_scrolled_window_new(NULL, NULL);
	sw2=gtk_scrolled_window_new(NULL, NULL);
	sw3=gtk_scrolled_window_new(NULL, NULL);
	artists=gtk_tree_view_new();
	albums=gtk_tree_view_new();
	hbox3=gtk_hbox_new(TRUE,3);
	hbox4=gtk_hbox_new(TRUE,3);
	hbox5=gtk_hbox_new(FALSE,3);
	songsort=gtk_button_new_with_label("Sort by Title");
	artistsort=gtk_button_new_with_label("Sort by artist");
	albumsort=gtk_button_new_with_label("Sort by album");
	removeall=gtk_button_new_with_label("Remove All");
	songs=gtk_clist_new_with_titles(4,titles);
	gtk_clist_set_column_width (GTK_CLIST(songs), 0, 250);
	gtk_clist_set_column_width (GTK_CLIST(songs), 1, 250);
	gtk_window_set_title (GTK_WINDOW (window), "Music Library");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (window,800,600);
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
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(artists),FALSE);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(albums),FALSE);
	vbox=gtk_vbox_new(FALSE,1);
	hbox=gtk_hbox_new(TRUE,3);
	hbox2=gtk_hbox_new(TRUE,5);
	gtk_widget_set_size_request(hbox2,800,40);
	addfile=gtk_button_new_with_label("Add File");
	addfolder=gtk_button_new_with_label("Add Folder");
	remove=gtk_button_new_with_label("Remove");
	gtk_widget_set_size_request(entry,600,30);
	gtk_box_pack_start(GTK_BOX(hbox2),addfile,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),addfolder,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),play,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),remove,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox2),removeall,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox3),songsort,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox3),albumsort,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox3),artistsort,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox4),allartists,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox4),allalbums,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox5),entry,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox5),search,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(hbox),sw2,TRUE,TRUE,5);
	gtk_box_pack_start(GTK_BOX(hbox),sw3,TRUE,TRUE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,TRUE,3);
	gtk_box_pack_start(GTK_BOX(vbox),hbox5,FALSE,FALSE,3);
	gtk_box_pack_start(GTK_BOX(vbox),hbox4,FALSE,TRUE,3);
	gtk_paned_add1(GTK_PANED(vpaned),hbox);
	gtk_paned_add2(GTK_PANED(vpaned),sw1);
	gtk_paned_set_position(GTK_PANED(vpaned),180);
	gtk_box_pack_start(GTK_BOX(vbox),vpaned,TRUE,TRUE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,TRUE,3);
	gtk_container_add(GTK_CONTAINER(sw1),songs);
	gtk_container_add(GTK_CONTAINER(sw2),artists);
	gtk_container_add(GTK_CONTAINER(sw3),albums);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_clist_set_column_visibility(GTK_CLIST(songs),3,0);
	init_list(albums,"Albums");
	init_list(artists,"Artists");
	FILE *fp;
	fp=fopen("global_songs.txt","a+");
	paneup("global_songs.txt");
	fclose(fp);
	fp=fopen("Albums.txt","a+");
	paneup("Albums.txt");
	fclose(fp);
	fp=fopen("Artists.txt","a+");
	paneup("Artists.txt");
	fclose(fp);
	fp=fopen("after_search.txt","w+");
	system("cp global_songs.txt after_search.txt");
	fclose(fp);
	gtk_clist_column_titles_passive(GTK_CLIST(songs));
	gtk_signal_connect(GTK_OBJECT(songs), "select_row",
                       GTK_SIGNAL_FUNC(selection_made),
                       NULL);
	selection  = gtk_tree_view_get_selection(GTK_TREE_VIEW(artists));
	g_signal_connect(selection,"changed",G_CALLBACK(sel),NULL);
	selection2  = gtk_tree_view_get_selection(GTK_TREE_VIEW(albums));
	g_signal_connect(selection2,"changed",G_CALLBACK(sel2),NULL);
    gtk_signal_connect_object(GTK_OBJECT(addfile), "clicked",
                   G_CALLBACK(file_add),(gpointer)songs);
    gtk_signal_connect_object(GTK_OBJECT(addfolder), "clicked",
                   G_CALLBACK(folder_add),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(removeall), "clicked",
						G_CALLBACK(allremove),(gpointer)albums);
	gtk_signal_connect_object(GTK_OBJECT(songsort), "clicked",
						G_CALLBACK(sort_global_songs),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(artistsort), "clicked",
						G_CALLBACK(sort_global_artists),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(albumsort), "clicked",
						G_CALLBACK(sort_global_albums),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(allalbums), "clicked",
						G_CALLBACK(alball),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(play), "clicked",
						G_CALLBACK(songplay),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(allartists), "clicked",
						G_CALLBACK(artall),(gpointer)songs);
	gtk_signal_connect_object(GTK_OBJECT(search), "clicked",
						G_CALLBACK(searchbox),entry);
	gtk_signal_connect_object(GTK_OBJECT(remove),"clicked",G_CALLBACK(remove_button),selection);
	g_signal_connect(G_OBJECT(remove), "clicked",
                   G_CALLBACK(alball),selection);
    g_signal_connect (G_OBJECT (window), "destroy",
                   G_CALLBACK(quit), NULL);
	gtk_widget_show(songs);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}