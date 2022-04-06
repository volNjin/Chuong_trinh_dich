#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int type; 

typedef struct node{
    type data;
    struct node *next;
    struct node *prev;
} node;

typedef struct elm{
	char str[50];
	int count;
	node *root,*tail;
}elm;

typedef struct node_T{
    elm data;
    struct node_T *left,*right;
}node_T;

typedef node_T *tree;

void makeNullList(node* *root,node* *tail){
    *root=NULL;
    *tail=NULL;
}

node* makeNewNode(type data){
    node *new = (node*)malloc(sizeof(node));
    new->data=data;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void appendList(type data,node **root,node **tail){
	node* new=makeNewNode(data);
    if((*root)==NULL){
   	    (*root)=new;
 		(*tail)=new;
 		new->next = new->prev = NULL;
    } else if(!((*tail)->data==new->data)){
  	        new->prev=(*tail);
  	        (*tail)->next=new;
  	        (*tail)=new;
  	        new->next = NULL;
	}
}

void makeNullTree(tree* Tree){
    (*Tree)=NULL;
}

int isEmptyTree(tree Tree){
	return Tree==NULL;
}

node_T* createNode(elm data){
    node_T* new = (node_T*)malloc(sizeof(node_T));
    if(new!=NULL){
	    new->data=data;
	    new->left=NULL;
	    new->right=NULL;
    }
    return new;
}

int isLeaf(tree node){
	if(isEmptyTree(node)){
		return 0;
	}
	else return(node->left==NULL && node->right==NULL);
}

void insertNode(tree* Tree,elm data){
	if(*Tree==NULL) (*Tree)=createNode(data);
	else {
		if(strcmp(data.str, (*Tree)->data.str)>0) insertNode(&(*Tree)->right,data);
		else insertNode(&(*Tree)->left,data);
	}
}

tree searchTree(tree root, char* s){
	if (root==NULL) return root;
	if (strcmp(s,root->data.str)==0) 
        return root;
	else if (strcmp(s,root->data.str)>0) 
        return searchTree(root->right,s);
	else return searchTree(root->left,s);
}

void addWord(tree* wordTree ,char *s, int line){
	tree cur = searchTree(*wordTree, s);
	if (cur==NULL){
		elm data;
		strcpy(data.str, s);
		data.count=1;
		makeNullList(&(data.root), &(data.tail));
		appendList(line, &(data.root), &(data.tail));
		insertNode(wordTree, data);
	} else {
		cur->data.count++;
		appendList(line, &(cur->data.root), &(cur->data.tail));
	}
}

void loadFile(char* fileName ,tree* wordTree, tree* stopWTree){
	char c, preC;
	int count=0, line=1;
	char s[50];
	int idvN=0;
	FILE *f=fopen(fileName,"r");
	if(f==NULL){
		printf("Cannot open %s\n", fileName);
		return;
	}
	for(preC=' ';(c=fgetc(f))!=EOF; preC = c){
		if('A'<=c&&c<='Z'){
			c+=32;
			if(preC<'a'||preC>'z'){
				idvN=1;
			}
		}
		if(('a'<=c&&c<='z')||c=='\'')
			s[count++]=c;
		else {
			s[count]='\0';
			if(!idvN)
				if(searchTree(*stopWTree,s)==NULL)
					addWord(wordTree,s,line);
			count=0;
			idvN=0;
			if(c=='\n') line++;
		}
	}
	fclose(f);
}

void displayNode(node* node){
    if(node==NULL){
        printf("This node is NULL\n");
        return;
    }
    printf(", %d",node->data);
}

void traverseList(node *root){
	node *cur;
    for(cur=root;cur!=NULL;cur=cur->next)
        displayNode(cur);
}

void inOrderPrint(tree root){
	if(root!=NULL){
		inOrderPrint(root->left);
		printf("%s %d", root->data.str, root->data.count);
		traverseList(root->data.root);
		printf("\n");
		inOrderPrint(root->right);
	}
}

void freeNode(node *root){
	node *tmp;
	for(tmp=root;tmp!=NULL;tmp=root){
        root=tmp->next;
        free(tmp);
    }
}

void freeTree(tree root){
	if(root==NULL) return;
	freeTree(root->left);
	freeTree(root->right);
	freeNode(root->data.root);
	free((void*)root);
}

int main(int argc, char *argv[]){
	tree wordTree, stopWTree;
	makeNullTree(&wordTree);
	makeNullTree(&stopWTree);
	if(argc<3){
        printf("Wrong command! Please try again with ./[exe_file] [stopw_file] [source_file]\n");
        exit(0);
    }
	loadFile(argv[1], &stopWTree, &wordTree);
	loadFile(argv[2], &wordTree, &stopWTree);
	inOrderPrint(wordTree);
	freeTree(wordTree);
	freeTree(stopWTree);
	return 0;
}