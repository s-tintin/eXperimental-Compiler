#include <string.h>
#include <stdlib.h>
#include "y.tab.h"

#define TYPE_int 1
#define TYPE_bool 2
#define TYPE_void 3
#define TYPE_intarray 4
#define TYPE_boolarray 5

struct Lsymbol 
{
    int type;   
    int size; 
    int* binding;
    char* name; 
    struct Lsymbol *next; 
};

struct Lsymbol *Lhead;

struct Lsymbol *Lsearch(char* name);

void Linstall(char* name, int type,int size);


struct tnode 
{
	int TYPE;
	int NODETYPE;
	char* NAME; 
	int VALUE; 
	struct tnode *ptr1, *ptr2;
};

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char* NAME,struct tnode *ptr1, struct tnode *ptr2);

struct tnode* makeLeafNode(int n);

struct tnode* makeOperatorNode(int op,struct tnode *l,struct tnode *r);

int evaluate(struct tnode *t);
