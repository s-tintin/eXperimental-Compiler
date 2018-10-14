
extern int variable[26];
extern int i;

struct Tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char NAME, struct Tnode *PTR1, struct Tnode *PTR2)
{
	struct Tnode *temp;
	temp = (struct Tnode*)malloc(sizeof(struct Tnode));
	temp->type = TYPE;
	temp->nodetype = NODETYPE;
	temp->value = VALUE;
	temp->name = NAME;
	temp->ptr1 = PTR1;
	temp->ptr2 = PTR2;

	printf("%d %d %c treecreate\n",temp->nodetype,temp->value,temp->name);
	
	return temp;
}

int evaluate(struct Tnode *t)
{
	printf("%d %d %c \n",t->nodetype,t->value,t->name);
	switch(t->nodetype)
	{
		case 0 :return t->value;
			break;

		case 1 : return evaluate(t->ptr1) + evaluate(t->ptr2);
			break;

		case 2 : return evaluate(t->ptr1) * evaluate(t->ptr2);
			break;

		case 3 : return evaluate(t->ptr1) < evaluate(t->ptr2);
			break;

		case 4 : return evaluate(t->ptr1) < evaluate(t->ptr2);
			break;

		case 5 : return evaluate(t->ptr1) == evaluate(t->ptr2);
			break;

		case 6 : evaluate(t->ptr1);
			evaluate(t->ptr2);
			return 0;
			break;
	
		case 7 : scanf("%d",&variable[(t->ptr1->name) - 'a']);
			//printf("%d read\n",*variable[*(t->ptr1->name) - 'a']);
			return 0;
			break;

		case 8 ://for(i = 0;i < 26;i++) {printf("%d\n",variable[i]);}
			//printf("%d\n ",t->ptr1->nodetype);
			//printf("%c\n ",(t->ptr1->name));
			//printf("%d\n",t->ptr1->value);
			printf("%d \n",evaluate(t->ptr1));
			return 0;
			break;

		case 9 : if(variable[(t->name) - 'a'] == -1)
			{
				printf("\nUnallocated Variable\n");
				exit(0);
			}
			else 
			{
				//printf("%d identifier\n",variable[(t->name) - 'a']);		
				return (variable[(t->name) - 'a']);
			}
			break;

		case 10 : variable[(t->ptr1->name) - 'a'] = evaluate(t->ptr2);
			//printf("%d assignment\n",*variable[*(t->ptr1->name) - 'a']);
			return 0;
			break;

		case 11 : if(evaluate(t->ptr1))
				return (evaluate(t->ptr2));
			else
				return 0;
			break;

		case 12 : while(evaluate(t->ptr1))
				evaluate(t->ptr2);
			return 0;
			break;

		default : printf("\nUnknown NodeType\n");
			exit(0);
	}
				

}
