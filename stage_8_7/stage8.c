
int curr_reg = -1, label = -1, line_count;
int stack_addr = 4095;

int get_staticaddr(int size)
{
	int ret_val;
	stack_addr += 1;

	if(stack_addr <= 5119)
	{
		ret_val = stack_addr;

		if(size > 1)
		{	
			stack_addr = stack_addr + size - 1;
		}
	}
	else
	{
		stack_addr -= 1;
		printf("\nError : Out of Stack address");
	}
	return ret_val;
}

struct Gsymbol* Gsearch(char* name)
{
	struct Gsymbol *temp;
    	temp = Ghead;
    	while(temp != NULL && strcmp(temp->name,name) != 0)
	{
   	 	temp = temp->next;    
    	}
    	return temp;    
}

void Ginstall(char* name,int type,int size)
{
	struct Gsymbol *check;
	check = Gsearch(name);
    	if(check != NULL)			
	{
        	printf("\nVariable redeclared");
        	exit(0);
    	}

    	struct Gsymbol *temp;
    	temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol));

    	temp->name = (char *)malloc(sizeof(name));
    	strcpy(temp->name,name);
    	temp->type = type;
    	temp->size = size;
    	temp->next = NULL;
    	temp->binding = get_staticaddr(size);
	
	if(Ghead == NULL)
	{
   	 	Ghead = temp;   	    
    	}
    	
	else
	{
		check = Ghead;
		while(check->next != NULL)
		{check = check->next;}

		check->next = temp;
	}
	//printf("SYMBOL TABLE %s  %d  %d\n",temp->name,temp->size,temp->binding);
}

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char * NAME,struct tnode *ptr1,struct tnode *ptr2,struct tnode *ptr3)
{
	struct tnode * temp = (struct tnode *) malloc (sizeof(struct tnode));
	temp->	TYPE = TYPE;
	temp->NODETYPE = NODETYPE;
	temp->VALUE = VALUE;
	if(NAME != NULL)
	{
		temp->NAME = (char *)malloc(sizeof(NAME));
		strcpy(temp->NAME,NAME);
	}
	temp->ptr1 = ptr1;
	temp->ptr2 = ptr2;
	temp->ptr3 = ptr3;

	//printf("In Tree Create %d %d\n",temp->TYPE,temp->NODETYPE);
	return temp;
}

struct tnode* makeLeafNode(int n)
{
	struct tnode *temp;
	temp = (struct tnode*)malloc(sizeof(struct tnode));
	temp->TYPE = TYPE_int;
	temp->NODETYPE = NODE_NUM;
	temp->VALUE = n;

	//printf("In Leaf Node %d\n",temp->VALUE);
	return temp;
}

struct tnode* makeOperatorNode(int op,struct tnode *l,struct tnode *r)
{
	struct tnode *temp;
	temp = (struct tnode*)malloc(sizeof(struct tnode));
    	temp->NODETYPE = op;
    	temp->ptr1 = l;
    	temp->ptr2 = r;

	//printf("In Operator Node %d \n",temp->NODETYPE);
    	return temp;
}

void labelentry(int num,int addr)
{
	struct label_node *temp,*check;
	temp = (struct label_node*)malloc(sizeof(struct label_node));
	temp->label_num = num;
	temp->label_addr = addr;
	temp->next = NULL;

	if(lroot == NULL)
	{
   	 	lroot = temp;   	    
    	}
    	
	else
	{
		check = lroot;
		while(check->next != NULL)
		{check = check->next;}

		check->next = temp;
	}
}

int labelsearch(int num)
{
	//printf("IN LABEL SEARCH\n");
	struct label_node *check;
    	check = lroot;
    	while(check != NULL && (check->label_num != num))
	{
   	 	check = check->next;
    	}
	//printf("END LABEL SEARCH\n");
    	return check->label_addr;    
}
		

int get_reg()
{
	curr_reg += 1;
	if(curr_reg >=20)
	{
		printf("\nError : Out of Registers");
		exit(0);
	}

	else
		return curr_reg;
}

void free_reg()
{
	if(curr_reg > -1)
		curr_reg -= 1;

	else
		printf("All registers are already free\n");
}

int get_label()
{
	label += 1;
	return label;
}

void Generate(struct tnode *root,FILE *fp)
{
	//printf("In Generate function\n");
	int r1;
	line_count = 2056;
	if(fp == NULL)
	{
		printf("Error in opening the file");
		exit(1);
	}
	
	//printf("HEADER\n");
	fprintf(fp,"0\n"); //XMAGIC
	fprintf(fp,"2056\n"); //ENTRY POINT
	fprintf(fp,"0\n"); //TEXT SIZE
	fprintf(fp,"0\n"); //DATA SIZE
	fprintf(fp,"0\n"); //HEAP SIZE
	fprintf(fp,"0\n"); //STACK SIZE
	fprintf(fp,"1\n"); //LIBRARY FLAG
	fprintf(fp,"0\n"); //UNUSED
	//printf("ENDHEADER\n");
	fprintf(fp, "MOV SP,%d\n",get_staticaddr(1));	
	//printf("STACK_ADDR: %d\n",stack_addr);
	line_count += 2;
	codegen(root);

	r1 = get_reg();
	//printf("END CALL\n");
	fprintf(fp,"MOV R%d,%s\n",r1,"\"Exit\"");
	fprintf(fp,"PUSH R%d\n",r1);
	fprintf(fp,"PUSH R%d\n",r1);
	fprintf(fp,"PUSH R%d\n",r1);
	fprintf(fp,"PUSH R%d\n",r1);
	fprintf(fp,"PUSH R%d\n",r1);
	fprintf(fp,"CALL 0\n");

	fprintf(fp,"POP R%d\n",r1);
	fprintf(fp,"POP R%d\n",r1);
	fprintf(fp,"POP R%d\n",r1);
	fprintf(fp,"POP R%d\n",r1);
	fprintf(fp,"POP R%d\n",r1);
	//printf("END END CALL\n");
	line_count += 24;
	free_reg();
}

int codegen(struct tnode *root)
{
	//printf("In Codegen Func\n");
	int r1,r2,r3,l1,l2,offset;
	
	if(fp == NULL)
	{
		printf("\nError in opening the file");
		exit(1);
	}

	switch(root->NODETYPE)
	{
		case NODE_NUM : r1 = get_reg();
				fprintf(fp,"MOV R%d,%d\n",r1,root->VALUE);
				line_count += 2;
				return r1;
				break;

		case NODE_IDENTIFIER :// printf("IN ID\n");
				r1 = get_reg();
				fprintf(fp,"MOV R%d,[%d]\n",r1,root->Gentry->binding);
				line_count += 2;				
				//printf("END ID\n"); 
				return r1;
				break;

		case NODE_PLUS : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"ADD R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();			
				return r1;
				break;

		case NODE_MINUS : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"SUB R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();
				return r1;
				break;

		case NODE_MUL : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"MUL R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();
				return r1;
				break;

		case NODE_DIV : r1 = codegen(root->ptr1);
				r2 = codegen(root->ptr2);
				l1 = get_label();

				fprintf(fp,"JZ R%d,L%d\n",r2,l1);
				fprintf(fp,"DIV R%d,R%d\n",r1,r2);
				line_count += 4;
				r1 = get_reg();
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				fprintf(fp,"MOV R%d,%s\n",r1,"\"Exit\"");
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"CALL 0\n");

				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				line_count+=24;
				free_reg();
				free_reg();
				return r1;
				break;

		case NODE_MOD : r1 = codegen(root->ptr1);
				r2 = codegen(root->ptr2);
				l1 = get_label();
				fprintf(fp,"JZ R%d,L%d\n",r2,l1);
				fprintf(fp,"MOD R%d,R%d\n",r1,r2);
				line_count += 4;
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				fprintf(fp,"MOV R%d,%s\n",r1,"\"Exit\"");
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"CALL 0\n");

				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				line_count+=24;
				free_reg();
				free_reg();
				return r1;
				break;

		case NODE_ARRAY : offset = codegen(root->ptr2);
				r1 = get_reg();

				fprintf(fp,"MOV R%d,%d\n",r1,(root->ptr1->Gentry->binding));
				fprintf(fp,"ADD R%d,R%d\n",r1,offset);
				fprintf(fp,"MOV R%d,[R%d]\n",offset,r1);
				line_count += 6;
				free_reg();
				return offset;
				break;

		case NODE_True : r1 = get_reg();
				fprintf(fp,"MOV R%d,%d\n",r1,root->VALUE);
				line_count += 2;
				return r1;
				break;

		case NODE_False : r1 = get_reg();
				fprintf(fp,"MOV R%d,%d\n",r1,root->VALUE);
				line_count += 2;
				return r1;
				break;

		case NODE_LT : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"LT R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;

		case NODE_GT : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"GT R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;

		case NODE_EQ : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"EQ R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;

		case NODE_NEQ : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"NE R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;


		case NODE_LTE : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"LE R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;

		case NODE_GTE : r1 = codegen(root->ptr1);
				r2  = codegen(root->ptr2);
				fprintf(fp,"GE R%d,R%d\n",r1,r2);
				line_count += 2;
				free_reg();				
				return r1;
				break;

		case NODE_AND : r1 = codegen(root->ptr1);
				r3 = get_reg();
				fprintf(fp,"MOV R%d,1\n",r3);
				l1 = get_label();
				fprintf(fp,"JZ R%d,L%d\n",r1,l1);
				line_count += 4;
				r2 = codegen(root->ptr2);
				fprintf(fp,"MOV R%d,R%d\n",r3,r2);
				line_count += 2;;
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				fprintf(fp,"MUL R%d,R%d\n",r1,r3);
				line_count += 2;
				free_reg();
				free_reg();
				return r1;
				break;

		case NODE_OR : r1 = codegen(root->ptr1);
				r3 = get_reg();
				fprintf(fp,"MOV R%d,0\n",r3);
				l1 = get_label();
				fprintf(fp,"JNZ R%d,L%d\n",r1,l1);
				line_count += 4;
				r2 = codegen(root->ptr2);
				fprintf(fp,"MOV R%d,R%d\n",r3,r2);
				line_count += 2;
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				fprintf(fp,"ADD R%d,R%d\n",r1,r3);
				line_count+=2;
				free_reg();
				free_reg();
				return r1;
				break;

		case NODE_NOT : r1 = codegen(root->ptr1);
				l1 = get_label();
				fprintf(fp,"JNZ R%d,L%d\n",r1,l1);
				fprintf(fp,"MOV R%d,1\n",r1);
				l2 = get_label();
				fprintf(fp,"JMP L%d\n",l2);
				line_count += 6;
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				fprintf(fp,"MOV R%d,0\n",r1);
				line_count += 2;
				//fprintf(fp,"L%d:\n",l2);
				labelentry(l2,line_count);
				return r1;
				break;

		case NODE_STMT : r1 = codegen(root->ptr1);
				r2 = codegen(root->ptr2);
				break;

		case NODE_ASGN : r1 = codegen(root->ptr2);
				fprintf(fp,"MOV [%d],R%d\n",(root->ptr1->Gentry->binding),r1);
				line_count += 2;
				free_reg();
				break;

		case NODE_ARRAYASGN : offset = codegen(root->ptr2);
				r1 = get_reg();		
				fprintf(fp,"MOV R%d,%d\n",r1,(root->ptr1->Gentry->binding));
				fprintf(fp,"ADD R%d,R%d\n",r1,offset);
				line_count += 4;
				r2 = codegen(root->ptr3);
				fprintf(fp,"MOV [R%d],R%d\n",r1,r2);
				line_count += 2;
				free_reg();
				free_reg();
				free_reg();
				break;

		case NODE_READ : r1 = get_reg();
				r2 = get_reg();
				fprintf(fp,"MOV R%d,-1\n",r1);//buffer
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"MOV R%d,%s\n",r2,"\"Read\"");
				fprintf(fp,"PUSH R%d\n",r2);
				fprintf(fp,"MOV R%d,-1\n",r2);//pushing arg 1
				fprintf(fp,"PUSH R%d\n",r2);
				fprintf(fp,"MOV R%d,SP\n",r1);
				fprintf(fp,"MOV R%d,%d\n",r2,2);
				fprintf(fp,"SUB R%d,R%d\n",r1,r2);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r2);//blank reg
				fprintf(fp,"PUSH R%d\n",r2);//blank_reg for return val
				fprintf(fp,"CALL 0\n");

				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
	
				free_reg();
				fprintf(fp,"MOV [%d],R%d\n",root->ptr1->Gentry->binding,r1);			
				line_count+=40;	
				free_reg();
				break;

		case NODE_READARRAY : //printf("IN READARRAY\n"); 
				r1 = get_reg();
				r2 = get_reg();

				fprintf(fp,"MOV R%d,-1\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"MOV R%d,%s\n",r2,"\"Read\"");
				fprintf(fp,"PUSH R%d\n",r2);
				fprintf(fp,"MOV R%d,-1\n",r2);//pushing arg 1
				fprintf(fp,"PUSH R%d\n",r2);
				fprintf(fp,"MOV R%d,SP\n",r1);
				fprintf(fp,"MOV R%d,%d\n",r2,2);
				fprintf(fp,"SUB R%d,R%d\n",r1,r2);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r2);//blank reg
				fprintf(fp,"PUSH R%d\n",r2);//blank_reg for return val
				fprintf(fp,"CALL 0\n");

				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r2);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
	
				offset = codegen(root->ptr2);
				//printf("IN READ ARRAY : %d\n",(root->ptr1->Gentry->binding));
				fprintf(fp,"MOV R%d,%d\n",r2,(root->ptr1->Gentry->binding));
				fprintf(fp,"ADD R%d,R%d\n",offset,r2);
				fprintf(fp,"MOV [R%d],R%d\n",offset,r1);
				line_count+=44;
				free_reg();
				free_reg();
				free_reg();
				//printf("END READARRAY\n");
				break;

		case NODE_WRITE : r1 = get_reg();
				r2 = get_reg();
				//printf("IN WRITE %d\n",codegen(root->ptr1));
				fprintf(fp,"PUSH R%d\n",codegen(root->ptr1));
				fprintf(fp,"MOV R%d,%s\n",r1,"\"Write\"");
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"MOV R%d,-2\n",r1);//pushing arg 1
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"MOV R%d,SP\n",r1);
				fprintf(fp,"MOV R%d,%d\n",r2,2);
				fprintf(fp,"SUB R%d,R%d\n",r1,r2);
				fprintf(fp,"PUSH R%d\n",r1);
				fprintf(fp,"PUSH R%d\n",r1);//arg 3 : blank reg
				fprintf(fp,"PUSH R%d\n",r1);//blank_reg for return val
				fprintf(fp,"CALL 0\n");
				
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				fprintf(fp,"POP R%d\n",r1);
				line_count+=36;	
				free_reg();
				free_reg();
				free_reg();
				break;

		case NODE_IF : l1 = get_label();
				r1 = codegen(root->ptr1);
				fprintf(fp,"JZ R%d,L%d\n",r1,l1);
				line_count += 2;
				free_reg();
				codegen(root->ptr2);
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				break;

		case NODE_ELSE : l1 = get_label();
				l2 = get_label();
				r1 = codegen(root->ptr1);
				fprintf(fp,"JZ R%d,L%d\n",r1,l1);
				line_count += 2;
				free_reg();
				codegen(root->ptr2);
				fprintf(fp,"JMP L%d\n",l2);
				line_count += 2;
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				codegen(root->ptr3);
				//fprintf(fp,"L%d:\n",l2);
				labelentry(l2,line_count);
				break;

		case NODE_WHILE : l1 = get_label();
				l2 = get_label();
				//fprintf(fp,"L%d:\n",l1);
				labelentry(l1,line_count);
				r1 = codegen(root->ptr1);
				fprintf(fp,"JZ R%d,L%d\n",r1,l2);
				line_count += 2;
				codegen(root->ptr2);
				fprintf(fp,"JMP L%d\n",l1);
				line_count += 2;
				//fprintf(fp,"L%d:\n",l2);
				labelentry(l2,line_count);
				free_reg();
				break;

		default : printf("\nError : Unknown Node Type");
				exit(1);
				
	}
	
	return 0;
}
				
void finalgen(FILE *fp,FILE *f)
{
	//printf("IN FINALGEN\n");
	char string[40],num[10],new_string[40];
	int pos=-1,no,i,addr,j;
	
	while(1)
	{
		//printf("IN WHILE\n");
		if(fgets(string,40,fp)==NULL) 
			break;            
		
		//printf("THE STRING READ : %s\n",string);
		pos = getposition(string);
		//printf("POS : %d\n",pos);

		if(pos != -1)
		{
			//printf("IN ELSE IF\n");
			j = 0;
			i = pos+1;
			while(string[i]!='\0')
			 {
				//printf("IN WHILE 1\n");
				num[j]=string[i];			
				j++;
				i++;
				//printf("END WHILE 1\n");
			} 

			num[j] = '\0';
			no = atoi(num);
			//printf("LABEL NUMBER : %d\n",no);
			addr = labelsearch(no);
			//printf("addr : %d\n",addr);
		
			i = 0;
			j = 0;
			
			//printf("BEFORE WHILE 2\n");
			while(i < pos)
			 {
				//printf("IN WHILE 2\n");
				new_string[j] = string[i];
				//printf("(%c)",new_string[j]);			
				j++;
				i++;
				//printf("END WHILE 2\n");
			}
			//printf("(%d)",j);	
			
			new_string[j] = '\0';
			//printf("%s %d\n",new_string,addr);
			fprintf(f,"%s%d\n",new_string,addr); 
			//printf("END ELSE IF\n");
		}
		else
		{
			//printf("IN ELSE\n");
			fprintf(f,"%s",string);
			//printf("END ELSE\n");
		}
	}
	//printf("END FINALGEN\n");	
}

int getposition(char* string)
{
	//printf("IN GETPOS\n");
	int i = 0;
	while(string[i] != '\0')
	{
		if((string[i] == 'L' && string[i-1] == ',') || (string[i] == 'L' && string[i-2] == 'P'))
			return i;
		
		i++;
	}
	//printf("END GETPOS\n");
	return -1;
}



