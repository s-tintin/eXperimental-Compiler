#include "stage5.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Lsymbol* Lsearch(char* name)
{
	struct Lsymbol *temp;
    	temp = Lhead;
    	while(temp != NULL && strcmp(temp->name,name) != 0)
	{
   	 	temp = temp->next;    
    	}
    	return temp;    
}

void Linstall(char* name, int type,int size)
{
	struct Lsymbol *check;
	int i = 0;
    	check = Lsearch(name);
    	if(check != NULL)			
	{
        	printf("variable redefined ");
        	printf(" %s",name);
        	exit(0);
    	}

    	struct Lsymbol *temp;
    	temp = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));

    	temp->name = (char *)malloc(sizeof(name));
    	strcpy(temp->name,name);
    	temp->type = type;
    	temp->size = size;
    	temp->next = NULL;
    
	switch(temp->type)
	{
		case STYPE_INT:
   		 	temp->binding = malloc(sizeof(int));
   			break;

   	 	case STYPE_ARR_INT:
   		 	temp->binding = malloc(sizeof(int)*(temp->size));
   		 	break;

   	 	default:
   		 	printf("unknown type to allocate space in Lsymbol table\n");
   		 	break;
    	}
    	    
    
    	if(Lhead == NULL)
	{
   	 	Lhead = temp;   	    
    	}
    	
	else
	{
		check = Lhead;
		while(check->next != NULL)
		{check = check->next;}

		check->next = temp;
	}

	//i=i+1;
	//printf("%d\n",i);
}

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char * NAME,struct tnode *ptr1,struct tnode * ptr2)
{
	struct tnode * temp = (struct tnode *) malloc (sizeof(struct tnode));
	temp->	TYPE = TYPE;
	temp->NODETYPE=NODETYPE;
	temp->VALUE=VALUE;
	temp->NAME=NAME;
	temp->ptr1=ptr1;
	temp->ptr2=ptr2;
	return temp;
}

int evaluate(struct tnode*root)
{ 
	struct tnode* temp;
	struct Lsymbol* templ;
	char name;
	int value;
	switch(root->NODETYPE)
	{
		case NUM: return root->VALUE;
				break;
		case PLUS: return evaluate(root->ptr1)+evaluate(root->ptr2);
			       break;
		case MUL:return evaluate(root->ptr1)*evaluate(root->ptr2);
			       break;
		case LT: return evaluate(root->ptr1)<evaluate(root->ptr2);
			       break;
		case GT: return evaluate(root->ptr1)>evaluate(root->ptr2);
			       break;	     
		case EQ: return evaluate(root->ptr1)==evaluate(root->ptr2);
			       break;  
		case STMT : evaluate(root->ptr1);
				evaluate(root->ptr2);
				return 0;
				break;
		case READ: templ = Lsearch(root->ptr1->NAME);
				if(templ == NULL)
				{
					printf("Unallocated variable");
					exit(0);	
				}
				else
				{ 
					scanf("%d", (templ->binding));
			    	}
				return 0;
				break;
		case READARRAY : templ = Lsearch(root->ptr1->NAME);
				if(templ == NULL)
				{
					printf("Unallocated variable");
					exit(0);	
				}
				else
				{ 
					value = evaluate(root->ptr2);
					if(value >= templ->size)
					{
						printf("Error: invalild array size accessed\n");
						exit(0);
					}
					scanf("%d", (templ->binding + value));
					//printf("\n%d",*(templ->binding + value));
			    	}
				return 0;
				break;
		case WRITE : printf("%d ",evaluate(root->ptr1));
					 return 0;
					 break;
		case ID : templ = Lsearch(root->NAME);
			if(templ==NULL)
			{printf("unallocated variable\n");
			exit(0);}
			return *(templ->binding);
			break;
		case ASGN: templ = Lsearch(root->ptr1->NAME);
			if(templ==NULL)
			{
				printf("variable unallocated ");
        			exit(0);
				
			}
			else if(templ != NULL)			
			{
        			value=evaluate(root->ptr2);
				*(templ->binding) = value;
    			}

			return 0;
			 break;
		case IF : if(evaluate(root->ptr1))
					return evaluate(root->ptr2);
				else return 0;
				break;

		case WHILE : while(evaluate(root->ptr1))
						evaluate(root->ptr2);
					return 0;
					break;
		case ARRAY : value = evaluate(root->ptr2);
				templ = Lsearch(root->ptr1->NAME);
				if(value >= templ->size)
					{
						printf("Error: invalild array size accessed\n");
						exit(0);
					}
				return *((templ->binding) + value);
				break;
		case ARRAYASGN : templ = Lsearch(root->NAME);
				if(templ  == NULL)
				{
					printf("Unallocated variable '%s'", root->NAME);
					exit(0);
				}
				value = evaluate(root->ptr1);
				if(value >= templ->size)
					{
						printf("Error: invalild array size accessed\n");
						exit(0);
					}
				*(templ->binding + value) = evaluate(root->ptr2);
			return 0;
			break;	
		default : printf("Wrong Choice!\n");
					exit(0);

	}
}

struct tnode* makeLeafNode(int n){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    bzero(temp, sizeof(struct tnode));
    temp->TYPE=NUM;
    temp->NODETYPE=NUM;
    temp->VALUE=n;
    return temp;
}

struct tnode* makeOperatorNode(int op,struct tnode *l,struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    bzero(temp, sizeof(struct tnode));
    temp->TYPE=NUM;
    temp->NODETYPE=op;
    temp->ptr1=l;
    temp->ptr2=r;
    return temp;
}
