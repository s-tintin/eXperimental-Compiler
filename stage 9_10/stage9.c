
int stack_addr = 4095,label = -1,line_count = 2056,curr_reg = -1,func_bind = 1,arg_bind = -3,local_bind = 1;

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
		exit(1);
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

void Ginstall(char* name,int type,int size,struct Paramstruct *paramlist)
{
	struct Gsymbol *check;
	check = Gsearch(name);
    	if(check != NULL)			
	{
        	printf("\nVariable redeclared");
        	exit(1);
    	}

    	struct Gsymbol *temp;
    	temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol));

    	temp->name = (char *)malloc(sizeof(name));
    	strcpy(temp->name,name);
    	temp->type = type;
    	temp->size = size;
	temp->paramlist = paramlist;
    	temp->next = NULL;
    	
	if(temp->type == TYPE_func_int || temp->type == TYPE_func_str)
	{
		if(strcmp(temp->name,"MAIN") != 0)
		{
			temp->binding = func_bind;
			func_bind++;
		}
	}

	else if(temp->type == TYPE_int || temp->type == TYPE_str || temp->type == TYPE_strarray || temp->type == TYPE_intarray) 
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
	//printf("GLOBAL SYMBOL TABLE %s  %d  %d\n",temp->name,temp->size,temp->binding);
}

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

void Linstall(char* name,int type)
{
	struct Lsymbol *check;
	check = Lsearch(name);
    	if(check != NULL)			
	{
        	printf("\nVariable redeclared");
        	exit(1);
    	}

    	struct Lsymbol *temp;
    	temp = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));

    	temp->name = (char *)malloc(sizeof(name));
    	strcpy(temp->name,name);
    	temp->type = type;
    	temp->next = NULL;
    	
	if(temp->type == TYPE_arg_int || temp->type == TYPE_arg_str || temp->type == TYPE_arg_ref_int || temp->type == TYPE_arg_ref_str)
	{
		temp->binding = arg_bind;
		arg_bind--;
	}

	else if(temp->type == TYPE_int || temp->binding == TYPE_str)
	{
		temp->binding = local_bind;
		local_bind++;
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
	//printf("LOCAL SYMBOL TABLE %s %d %d\n",temp->name,temp->type,temp->binding);
}

struct Paramstruct *install_list(int p_type,char *p_name,int passtype)
{
	struct Paramstruct *p;
	p = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));

	p->ptype = p_type;
	p->pname = (char *)malloc(sizeof(p_name));
	strcpy(p->pname,p_name);
	p->pass_type = passtype;
	p->next = NULL;

	if(p->ptype == TYPE_int)
	{
		if(p->pass_type == TYPE_param_val)
			Linstall(p_name,TYPE_arg_int);

		else if(p->pass_type == TYPE_param_ref)
			Linstall(p_name,TYPE_arg_ref_int);
	}

	else if(p->ptype == TYPE_str)
	{
		if(p->pass_type == TYPE_param_val)
			Linstall(p_name,TYPE_arg_str);

		else if(p->pass_type == TYPE_param_ref)
			Linstall(p_name,TYPE_arg_ref_str);
	}
	
	return p;
}

void reinitialise_lvar()
{
	struct Lsymbol *temp;
	temp = Lhead;
	
	while(temp != NULL)
	{
		Lhead = temp;
		temp = Lhead->next;
		free(Lhead->name);
		free(Lhead);
	}
	Lhead = NULL;
	local_bind = 1;
	arg_bind = -3;
}

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char * NAME,struct tnode *ptr1,struct tnode *ptr2,struct tnode *ptr3,struct tnode *arglist)
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
	else
		temp->NAME = NULL;

	temp->ptr1 = ptr1;
	temp->ptr2 = ptr2;
	temp->ptr3 = ptr3;
	temp->arglist = arglist;
	temp->Gentry = NULL;
	temp->Lentry = NULL;

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

int get_bind(struct tnode *t)
{
	if(t->NODETYPE == NODE_ARRAY || t->NODETYPE == NODE_ARRAYASGN || t->NODETYPE == NODE_READARRAY)
		return (t->ptr1->Gentry->binding);

	else if(t->NODETYPE == NODE_ASGN || t->NODETYPE == NODE_READ)
	{
		if(t->ptr1->Lentry == NULL)
			return (t->ptr1->Gentry->binding);
		else
			return (t->ptr1->Lentry->binding);
	}

	else if(t->NODETYPE == NODE_IDENTIFIER)
	{
		if(t->Lentry == NULL)
			return (t->Gentry->binding);
		else
			return (t->Lentry->binding);
	}
}

int get_reg()
{
	curr_reg += 1;
	if(curr_reg >=20)
	{
		printf("\nError : Out of Registers");
		exit(1);
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

void func_labelentry(int fnum,int faddr)
{
	struct label_node_func *temp,*check;
	temp = (struct label_node_func*)malloc(sizeof(struct label_node_func));
	temp->flabel_num = fnum;
	temp->flabel_addr = faddr;
	temp->next = NULL;

	if(flroot == NULL)
	{
   	 	flroot = temp;   	    
    	}
    	
	else
	{
		check = flroot;
		while(check->next != NULL)
		{check = check->next;}

		check->next = temp;
	}
}

int func_labelsearch(int fnum)
{
	//printf("IN FUNCTION LABEL SEARCH\n");
	struct label_node_func *check;
    	check = flroot;
    	while(check != NULL && (check->flabel_num != fnum))
	{
   	 	check = check->next;
    	}
	//printf("END FUNCTION LABEL SEARCH\n");
    	return check->flabel_addr;    
}

int codegen(struct tnode *root)
{
	//printf("In Codegen Func\n");
	int r1,r2,r3,l1,l2,offset;
	struct Lsymbol *ltemp;
	struct Gsymbol *gtemp;
	
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
				if(root->Lentry == NULL)
				{
					fprintf(fp,"MOV R%d,[%d]\n",r1,get_bind(root));
					line_count += 2;
				}
				else
				{
					fprintf(fp,"MOV R%d,%d\n",r1,get_bind(root));
					r2 = get_reg();
					fprintf(fp,"MOV R%d,BP\n",r2);
					fprintf(fp,"ADD R%d,R%d\n",r1,r2);
					free_reg();
					fprintf(fp,"MOV R%d,[R%d]\n",r1,r1);
					if(root->Lentry->type == TYPE_arg_ref_int || root->Lentry->type == TYPE_arg_ref_str)
					{		
						fprintf(fp,"MOV R%d,[R%d]\n",r1,r1);
						line_count += 2;
					}
	    				line_count += 8;
	        		}
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

				fprintf(fp,"MOV R%d,%d\n",r1,get_bind(root));
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
				if(root->ptr1->Lentry == NULL)
				{
					fprintf(fp,"MOV [%d],R%d\n",get_bind(root),r1);
					line_count += 2;
				}
				else
				{
					r2 = get_reg();
					r3 = get_reg();
					fprintf(fp,"MOV R%d,%d\n",r2,get_bind(root));
					fprintf(fp,"MOV R%d,BP\n",r3);
					fprintf(fp,"ADD R%d,R%d\n",r2,r3);
					if(root->ptr1->Lentry->type == TYPE_arg_ref_int || root->ptr1->Lentry->type == TYPE_arg_ref_str)
					{
						fprintf(fp,"MOV R%d,[R%d]\n",r2,r2);
						line_count += 2;
					}
					fprintf(fp,"MOV [R%d],R%d\n",r2,r1);
					free_reg();
					free_reg();
					line_count += 8;
				}
				free_reg();
				break;

		case NODE_ARRAYASGN : offset = codegen(root->ptr2);
				r1 = get_reg();		
				fprintf(fp,"MOV R%d,%d\n",r1,get_bind(root));
				fprintf(fp,"ADD R%d,R%d\n",r1,offset);
				line_count += 4;
				r2 = codegen(root->ptr3);
				fprintf(fp,"MOV [R%d],R%d\n",r1,r2);
				line_count += 2;
				free_reg();
				free_reg();
				free_reg();
				break;

		case NODE_READ : //printf("READ CODEGEN\n"); 
				r1 = get_reg();
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
				line_count += 38;

				if(root->ptr1->Lentry == NULL)
				{
					fprintf(fp,"MOV [%d],R%d\n",get_bind(root),r1);			
					line_count += 2;
					free_reg();
					free_reg();
				}
				else
				{
					fprintf(fp,"MOV R%d,%d\n",r2,get_bind(root));
					r3 = get_reg();
					fprintf(fp,"MOV R%d,BP\n",r3);
					fprintf(fp,"ADD R%d,R%d\n",r2,r3);
					if(root->ptr1->Lentry->type == TYPE_arg_ref_int || root->ptr1->Lentry->type == TYPE_arg_ref_str)
					{
						fprintf(fp,"MOV R%d,[R%d]\n",r2,r2);
						line_count += 2;
					}
					fprintf(fp,"MOV [R%d],R%d\n",r2,r1);
					free_reg();
					free_reg();
					free_reg();
					line_count += 8;
				}
				//printf("END CODEGEN READ\n");
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
	
				line_count += 38;
				offset = codegen(root->ptr2);
				//printf("IN READ ARRAY : %d\n",(root->ptr1->Gentry->binding));
				fprintf(fp,"MOV R%d,%d\n",r2,get_bind(root));
				fprintf(fp,"ADD R%d,R%d\n",offset,r2);
				fprintf(fp,"MOV [R%d],R%d\n",offset,r1);
				line_count+=6;
				free_reg();
				free_reg();
				free_reg();
				//printf("END READARRAY\n");
				break;

		case NODE_WRITE : r1 = get_reg();
				r2 = get_reg();
				//printf("IN WRITe codegen\n");
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
				//printf("END CODEGEN WRITE\n");
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

		case NODE_FUNCTION : r1 = gen_func_call(root);
				return r1;
				break;

		case NODE_RET : r1 = codegen(root->ptr1);	
				r2 = get_reg();
				fprintf(fp,"MOV R%d,BP\n",r2);
				r3 = get_reg();
				fprintf(fp,"MOV R%d,2\n",r3);
				fprintf(fp,"SUB R%d,R%d\n",r2,r3);
				fprintf(fp,"MOV [R%d],R%d\n",r2,r1);
				line_count += 8;
				free_reg();
				free_reg();
				free_reg();
				break;

		default : printf("\nError : Unknown Node Type");
				exit(1);
				
	}
	
	return 0;
}

void codegen_func(char *name,struct tnode *t)
{
	int fl,r1;
	
	struct Gsymbol *check;
	struct Lsymbol *ltemp;
	check = Gsearch(name);

	curr_reg = -1;
	func_labelentry(check->binding,line_count);

	fprintf(fp,"PUSH BP\n");
	fprintf(fp,"MOV BP,SP\n");
	line_count += 4;
	r1 = get_reg();
	ltemp = Lhead;
				
	while(ltemp != NULL)
	{
		if(ltemp->type != TYPE_arg_int || ltemp->type != TYPE_arg_str || ltemp->type != TYPE_arg_ref_int || ltemp->type != TYPE_arg_ref_str)
		{
			fprintf(fp,"PUSH R%d\n",r1);
			line_count += 2;
		}
		ltemp = ltemp->next; 
	}
	free_reg();

	codegen(t);

	r1 = get_reg();
	ltemp = Lhead;
	while(ltemp != NULL)
	{
		if(ltemp->type != TYPE_arg_int || ltemp->type != TYPE_arg_str || ltemp->type != TYPE_arg_ref_int || ltemp->type != TYPE_arg_ref_str)
		{
			fprintf(fp,"POP R%d\n",r1);
			line_count += 2;
		}
		ltemp = ltemp->next; 
	}
	
	free_reg();
	fprintf(fp,"POP BP\n");
	fprintf(fp,"RET\n");
	line_count += 4;
}

int generate(struct tnode *t)
{
	//printf("IN GENERTE\n");
	int r1;
	struct Lsymbol *ltemp;
	curr_reg = -1;
	label = -1;
	func_labelentry(0,line_count);

	//printf("eufuhijej\n");
	fprintf(fp,"PUSH BP\n");
	fprintf(fp,"MOV BP,SP\n");
	line_count += 4;
	r1 = get_reg();
	ltemp = Lhead;
	
	//printf("before pushing local variables\n");			
	while(ltemp != NULL)
	{
		if(ltemp->type != TYPE_arg_int || ltemp->type != TYPE_arg_str || ltemp->type != TYPE_arg_ref_int || ltemp->type != TYPE_arg_ref_str)
		{
			fprintf(fp,"PUSH R%d\n",r1);
			line_count += 2;
		}
		ltemp = ltemp->next; 
	}
	free_reg();

	//printf("before codegen of body\n");
	codegen(t);
	//printf("after codegen of body\n");

	r1 = get_reg();
	ltemp = Lhead;
	while(ltemp != NULL)
	{
		if(ltemp->type != TYPE_arg_int || ltemp->type != TYPE_arg_str || ltemp->type != TYPE_arg_ref_int || ltemp->type != TYPE_arg_ref_str)
		{
			fprintf(fp,"POP R%d\n",r1);
			line_count += 2;
		}
		ltemp = ltemp->next; 
	}
	
	free_reg();
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

	//printf("END GENERATE\n");			
}

	
int gen_func_call(struct tnode *t)
{//printf("IN GEN FUNC CALL\n");
	struct Paramstruct *ptemp;
	struct Gsymbol *gtemp;
	struct Lsymbol *ltemp;
	struct tnode *temp;
	int count = 0,r1,r2,f,res;

	while(count <= curr_reg)
	{
		//printf("IN WHILE 1\n");
		fprintf(fp,"PUSH R%d\n",count);
		count++;
		line_count += 2;
		//printf("END WHILE 1\n");
	}
	count--;

	gtemp = Gsearch(t->NAME);
	ptemp = gtemp->paramlist;
	temp = t->arglist;
	
	while(ptemp != NULL)
	{//printf("IN WHILE 2\n");
		if(ptemp->pass_type == TYPE_param_val)
		{//printf("IN PARAM VAL IF\n");
			r1 = codegen(temp->ptr2);
			fprintf(fp,"PUSH R%d\n",r1);
			line_count += 2;
			free_reg();
		//printf("END PARAM VAL IF\n");
		}
		
		else if(ptemp->pass_type == TYPE_param_ref)
		{
			if(temp->ptr2->Lentry != NULL)
			{
				r1 = get_reg();
				r2 = get_reg();
				ltemp = Lsearch(temp->ptr2->NAME);
				fprintf(fp,"MOV R%d,%d\n",r1,ltemp->binding);
				fprintf(fp,"MOV R%d,BP\n",r2);
				fprintf(fp,"ADD R%d,R%d\n",r1,r2);
				line_count += 6;

				if(temp->ptr2->Lentry->type == TYPE_arg_ref_int || temp->ptr2->Lentry->type == TYPE_arg_ref_str)
				{
					fprintf(fp,"MOV R%d,[R%d]\n",r1,r1);
					fprintf(fp,"PUSH R%d\n",r1);
					line_count += 4;
				}

				free_reg();
				free_reg();
			}

			else
			{
				r1 = get_reg();
				gtemp = Gsearch(temp->ptr2->NAME);
				fprintf(fp,"MOV R%d,%d\n",r1,gtemp->binding);
				fprintf(fp,"PUSH R%d\n",r1);
				line_count += 2;
				free_reg();
			}
		}	
		temp = temp->ptr1;
		ptemp = ptemp->next;
	}
	//printf("END WHILE 2\n");	
	r1 = get_reg();
	fprintf(fp,"PUSH R%d\n",r1);
	line_count += 2;
	free_reg();
	gtemp = Gsearch(t->NAME);
	//printf("BEFORE CALL\n");
	fprintf(fp,"CALL F%d\n",gtemp->binding);
	line_count += 2;	
	curr_reg = count;	
	r1 = get_reg();
	r2 = get_reg();
	fprintf(fp,"POP R%d\n",r1);
	line_count += 2;
	gtemp = Gsearch(t->NAME);
	ptemp = gtemp->paramlist;
	
	while(ptemp != NULL)
	{
		fprintf(fp,"POP R%d\n",r2);
		line_count += 2;
		ptemp = ptemp->next;
	}
	free_reg();
	
	while(count >= 0)
	{
		fprintf(fp,"POP R%d\n",count);
		line_count += 2;
		count--;
	}
	
	return r1;
}
	
void finalgen(FILE *fp,FILE *f)
{
	//printf("IN FINALGEN\n");
	char string[40],num[10],new_string[40];
	int pos=-1,no,i,addr,j,pos1=-1,inum;
	
	while(1)
	{
		//printf("IN WHILE\n");
		if(fgets(string,40,fp)==NULL) 
			break;            
		
		//printf("THE STRING READ : %s\n",string);
		pos = label_getposition(string);
		pos1 = func_getposition(string);
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
		else if(pos1 != -1)
		{
			j = 0;
			i = pos1+1;

			while(string[i]!='\0')
			 {
				num[j] = string[i];		
				j++;
				i++;
			} 
			num[j]='\0';
			//printf("*%s*\n",num);
			
			inum = atoi(num);

			addr = func_labelsearch(inum);
		
			i = 0;
			j = 0;
			
			while(i < pos1)
			 {
				new_string[j] = string[i];
				//printf("((%c))",new_string[j]);			
				j++;
				i++;
			}
				//printf("((%d))",j);	
			//new_string[pos]=addr;
			new_string[j]='\0';
		//printf("#%s#\n",new_string);
			fprintf(f,"%s%d\n",new_string,addr); 
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

int label_getposition(char* string)
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

int func_getposition(char *string)
{
	int i = 0;
	while(string[i] != '\0')
	{
		if(string[i] == 'F')
			return i;
		i++;
	}
	return -1;
}
	
