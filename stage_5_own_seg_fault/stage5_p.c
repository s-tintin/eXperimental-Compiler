struct Lsymbol *Llookup(char *name)
{
	printf("\nIN LOOKUP");
	struct Lsymbol *search;
	search = head;

	while(search != NULL)
	{
		if(strcmp(search->name,name) != 0)
			search = search->next;
	}
	
	return search;
}

void Linstall(char *name,int type,int size)
{
	struct Lsymbol *temp,*check;
	printf("\nIN Linstall");
	check = Llookup(name);
	if(check != NULL)
	{
		printf("\nVariable redeclared");
		exit(0);
	}

	temp = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));
	temp->name = (char*)malloc(sizeof(name));
	
	strcpy(temp->name,name);
	temp->type = type;
	temp->size = size;
	temp->binding = malloc(size * sizeof(int));

	if(head == NULL)
	{
		head = temp;
		temp->next = NULL;
	}
	
	else
	{
		check = head;
		while(check->next != NULL){
			check = check->next;}

		temp->next = NULL;
		check->next = temp;
	}

	
}

struct Tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char *NAME, struct Tnode *PTR1, struct Tnode *PTR2)
{
	struct Tnode *temp;
	temp = (struct Tnode*)malloc(sizeof(struct Tnode));
	temp->name = (char*)malloc(sizeof(char)*strlen(NAME));
   
	temp->type = TYPE;
	temp->nodetype = NODETYPE;
	temp->value = VALUE;
	
	if(NAME != NULL){printf("in here");
	strcpy(temp->name,NAME);}

	else{printf("its a number");}

	temp->ptr1 = PTR1;
	temp->ptr2 = PTR2;

	printf("%d %d treecreate\n",temp->nodetype,temp->value);
	
	return temp;
}

int evaluate(struct Tnode *t)
{
	struct Lsymbol *res;
	int value;
	printf("%d %d %s evaluate \n",t->nodetype,t->value,t->name);
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
	
		case 7 : res = Llookup(t->ptr1->name);
			if(res == NULL)
			{
				printf("Unallocated Variable");
				exit(0);
			}
			else
			{	
				scanf("%d",res->binding);
			}		

			return 0;
			break;

		case 8 : printf("%d \n",evaluate(t->ptr1));
			return 0;
			break;

		case 9 : res = Llookup(t->name);
			if(res == NULL)
			{
				printf("Unallocated Variable");
				exit(0);
			}
			else
			{	
				return (*(res->binding));
			}
			break;

		case 10 : res = Llookup(t->ptr1->name);
			if(res == NULL)
			{
				printf("Unallocated Variable");
				exit(0);
			}
			else
			{
				printf("%d,%s ASSIGNMENT EVALUATE",res->type,res->name);
				*(res->binding) = evaluate(t->ptr2);
			}
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

		case 13 : res = Llookup(t->ptr1->name);
			value = evaluate(t->ptr2);
			return *((res->binding) + value);
			break;

		case 14 : res = Llookup(t->name);
			if(res == NULL)
			{
				printf("Unallocated Variable");
				exit(0);
			}
			else
			{
				value = evaluate(t->ptr1);
				*((res->binding) + value) = evaluate(t->ptr2);
			}
			return 0;
			break;

		case 15 : res = Llookup(t->ptr1->name);
			if(res == NULL)
			{
				printf("Unallocated Variable");
				exit(0);
			}
			else
			{
				value = evaluate(t->ptr2);
				scanf("%d",(res->binding + value));
			}

			return 0;
			break;


		default : printf("\nUnknown NodeType\n");
			exit(0);
	}
				

}
