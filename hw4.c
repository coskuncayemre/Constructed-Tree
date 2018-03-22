#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Emre COSKUNCAY 21526806

int unique_counter=0;
int how_many_counter=0;
int file_size;
int file_size2;
int root_data;
int leaf_counter;
int leaf_data[50];

struct Node{
	int data;
	int no_child;
	int deleted;
	struct Node *parent;
	struct Node *children;
	struct Node *prev;
	struct Node *sibling;
};
struct Node *leaf_nodes[50];

void output_writer(struct Node *p,FILE *output){
    if(p==NULL)return;
    if(p->data==leaf_nodes[leaf_counter-1]->data){
    	fprintf(output,"%d",p->data);
	}
	else{
		fprintf(output,"%d,",p->data);
	}
    output_writer(p->children,output);
	output_writer(p->sibling,output);
}

int calculate_line(char *filename,int lines){
	char ch;
	FILE *fp = fopen(filename,"r");
	while(!feof(fp)){
  		ch = fgetc(fp);
  		if(ch == '\n'){
  			lines++;
		}
	}
	return lines;
}


struct Node* create_node(int data){
	struct Node *newnode = (struct Node*)malloc(sizeof(struct Node));
	newnode->children=NULL;
	newnode->sibling=NULL;
	newnode->parent=NULL;
	newnode->prev=NULL;
	newnode->data=data;
	newnode->no_child=0;
	unique_counter++;
	return newnode;
}

struct Node* add_child(struct Node* parent,int data){
	struct Node *baby = (struct Node*)malloc(sizeof(struct Node));
	baby->children=NULL;
	baby->sibling=NULL;
	baby->data=data;
	baby->parent=parent;
	baby->no_child=0;
	baby->prev=NULL;
	parent->children=baby;
	parent->no_child++;
	unique_counter++;
	return parent;
}

struct Node* add_sibling(struct Node* big_bro,int data){
	struct Node *bro= (struct Node*)malloc(sizeof(struct Node));
	bro->children=NULL;
	bro->sibling=NULL;
	bro->parent=big_bro->parent;
	bro->data=data;
	bro->prev=big_bro;
	bro->no_child=0;
	big_bro->sibling=bro;
	big_bro->parent->no_child++;
	unique_counter++;
	return big_bro;
}


void fromRootLeafNodes(struct Node* root){
	struct Node* temp=(struct Node*)malloc(sizeof(struct Node));
	struct Node* kol=(struct Node*)malloc(sizeof(struct Node));
	temp=root;
	int num=root->no_child;
	while(temp!=NULL){
		if(num>1){
			fromRootLeafNodes(root->children);
			kol=root->children->sibling;
			while(kol!=NULL){
				fromRootLeafNodes(kol);
				kol=kol->sibling;
			}
			temp=NULL;
		}
		else if(num==1){
			fromRootLeafNodes(root->children);
			temp=NULL;
		}
		else{
			leaf_nodes[leaf_counter]=root;
			leaf_data[leaf_counter++]=root->data;
			temp=NULL;
		}
	}
}

void showLeafNodes(){
	int i;
	for(i=0;i<=leaf_counter-1;i++){
		
	}
}

int isLeaf(int data){
	int i;
	for(i=0;i<=leaf_counter-1;i++){
		if(leaf_nodes[i]->data == data){
			return 1;
		}
	}
	return 0;
}

int isRoot(struct Node *p){
	if(p->parent==NULL){
		return 1;
	}
	return 0;
}

int checkSize(int x){
	if(x>leaf_counter-1){
		return 0;
	}
	return 1;
}

void adder(struct Node* root,int *unique_array,int *how_many_array){
	struct Node *temp= (struct Node*)malloc(sizeof(struct Node));
	int limit=how_many_array[how_many_counter],x;
	leaf_counter=0;
	fromRootLeafNodes(root);
	for(x=0;x<limit;x++){
        if(unique_counter>file_size){
            
        }
        else{
            int data=unique_array[unique_counter];
            if(limit>leaf_counter){
                if(checkSize(x)){
                    leaf_nodes[x]=add_child(leaf_nodes[x],data);
                }
                else{
                    if(leaf_nodes[(x-leaf_counter)%leaf_counter]->no_child>1){
                    	temp=leaf_nodes[(x-leaf_counter)%leaf_counter]->children->sibling;
                    	while(temp->sibling!=NULL){
                    		temp=temp->sibling;
						}
						temp=add_sibling(temp,data);
					}
                    else{
                    	leaf_nodes[(x-leaf_counter)%leaf_counter]->children=add_sibling(leaf_nodes[(x-leaf_counter)%leaf_counter]->children,data);
					}
                }
            }
            else{
                if(checkSize(x)){
                    leaf_nodes[x]=add_child(leaf_nodes[x],data);
                }
            }
        }
	}
	how_many_counter++;
	if(unique_counter<file_size){
		adder(root,unique_array,how_many_array);
	}
}

struct Node* tree_creator(struct Node* root,int *unique_array,int *how_many_array){
	struct Node *temp= (struct Node*)malloc(sizeof(struct Node));
	struct Node *target= (struct Node*)malloc(sizeof(struct Node));
	int sequencer=how_many_array[0],x;
	if(root==NULL){
		root=create_node(unique_array[0]);
		root_data=unique_array[0];
	}
	if(sequencer>0){
		root=add_child(root,unique_array[unique_counter]);
		temp=root->children;
		for(x=1;x<sequencer;x++){
			temp=add_sibling(temp,unique_array[unique_counter]);
			temp=temp->sibling;
		}
	}
	
	how_many_counter++;
	adder(root,unique_array,how_many_array);
	return root;
}

struct Node *getNode(struct Node *root,int data){
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	if(root==NULL){
		return NULL;
	}
	if(root->data==data){
		return root;
	}
	temp=getNode(root->children,data);
	if(temp!=NULL){
		if(temp->data==data){
			return temp;
		}
	}
	temp=getNode(root->sibling,data);
}


void display(struct Node *root){
	leaf_counter=0;
	fromRootLeafNodes(root);
	showLeafNodes();
}

int check_unique(int data,int *unique_array,struct Node *root){
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	int i=0;
	for(i=0;i<=file_size;i++){
		if(unique_array[i] == data){
			return 1;
		}
	}
	return 0;
}

void command_builder(char *command_array,int *index_array,int *unique_array,struct Node* root,FILE *output){
	int counter=0;
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	struct Node *temp2 = (struct Node*)malloc(sizeof(struct Node));
	struct Node *temp3 = (struct Node*)malloc(sizeof(struct Node));
	for(counter=0;counter<file_size2;counter++){
		if(command_array[counter]=='d'){
			if(check_unique(index_array[counter],unique_array,root)){
				temp=getNode(root,index_array[counter]);
				if(isLeaf(temp->data)){
					if(temp->parent->no_child>1){
						temp2=temp->parent->children;
						if(temp==temp2){
							temp->deleted=1;
							temp->parent->children=temp2->sibling;
						}
						if(temp->sibling==NULL){
							while(temp!=temp2){
								temp2=temp2->sibling;
							}
							temp->deleted=1;
							temp->prev->sibling=NULL;
						}
						else{
							temp->deleted=1;
							temp->sibling->prev=temp2;
							temp->prev->parent->children->sibling=temp->sibling;
						}
					}
					else{
						temp->deleted=1;
						temp->parent->children=NULL;
						temp->parent->no_child--;
					}
				}
				if(isRoot(temp)){
					temp2=temp->children->sibling;
					if(temp->children->no_child==1){
						temp->deleted=1;
						temp3=temp->children->children;
						temp3->sibling=temp2;
						temp2->prev=temp3;
					}
					else if(temp->children->no_child>1){
						temp3=temp->children->children->sibling;
						while(temp3->sibling!=NULL){
							temp3=temp3->sibling;
						}
						temp3->sibling=temp2;
						temp2->prev=temp3;
					}
					else{
						
					}
					temp->deleted=1;
					temp=temp->children;
					
				}
				
				else if(temp->parent!=NULL && temp->children!=NULL){
					temp2=temp->children;
					if(temp->parent->children==temp){
						temp->parent->children=temp->children;
						temp->children->parent=temp->parent;
						while(temp2->sibling!=NULL){
							temp2->parent=temp->parent;
							temp2=temp2->sibling;
						}
						temp->deleted=1;
						temp2->sibling=temp->sibling;
						temp->sibling->prev=temp2;
					}
					
					else{
						if(temp->sibling==NULL){
							temp->prev->sibling=temp->children;
							temp->children->parent=temp->parent;
							while(temp2->sibling!=NULL){
								temp2->parent=temp->parent;
								temp2=temp2->sibling;
							}
							temp->children->prev=temp->prev;
							temp->deleted=1;
						}
						else if (temp->prev!=NULL && temp->sibling!=NULL){
							temp->children->parent=temp->parent;
							while(temp2->sibling!=NULL){
								temp2->parent=temp->parent;
								temp2=temp2->sibling;
							}
							temp->prev->sibling=temp->children;
							temp2->sibling=temp->sibling;
							temp->sibling->prev=temp2;
						}
					}
				}
			}
		}
		else if(command_array[counter]=='l'){
			if(check_unique(index_array[counter],unique_array,root)){
				temp=getNode(root,index_array[counter]);
				if(temp!=NULL && temp->children!=NULL){
					fprintf(output,"%d,",temp->data);
					output_writer(temp->children,output);
					fprintf(output,"\n");
				}
				else{
					output_writer(temp,output);
				}
			}
			
		}
	}
}


int main(){
	
	char fName1[50];
	scanf("%s",fName1);
	char fName2[50];
	scanf("%s",fName2);
	int lines=0;
	lines=calculate_line(fName1,lines);
	file_size=lines;
	int lines2=0;
	lines2=(calculate_line(fName2,lines2))+1;
	file_size2=lines2;
	int unique,how_many,e;
	char ch;
	int ind;
	int x=0;
	int *unique_array,*how_many_array,*index_array;
	char command_array[lines2];
	unique_array=(int*)malloc(sizeof(int)*lines);
	how_many_array=(int*)malloc(sizeof(int)*lines);
	index_array=(int*)malloc(sizeof(int)*lines2);
	FILE *output=fopen("output.txt","w");
	FILE *f=fopen(fName1, "r");
	
	while(fscanf(f, "%d %d", &unique,&how_many) > 0 ){
	    	unique_array[e]=unique;
	    	how_many_array[e++]=how_many;
	}
	
	int ccount=0;
	FILE *cmd=fopen(fName2, "r");
	
	while(fscanf(cmd, "%s %d", &ch,&ind) > 0 ){
		command_array[ccount]=ch;
		index_array[ccount++]=ind;
	}
	
	struct Node* root=NULL;
	root=tree_creator(root,unique_array,how_many_array);
	
	leaf_counter=0;
	fromRootLeafNodes(root);
	
	command_builder(command_array,index_array,unique_array,root,output);
	
}
