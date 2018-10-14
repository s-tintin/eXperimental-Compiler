#define TYPE_int 1
#define TYPE_str 2
#define TYPE_void 3
#define TYPE_intarray 4
#define TYPE_strarray 5
#define NODE_NUM 6
#define NODE_IDENTIFIER 7
#define NODE_PLUS 8
#define NODE_MINUS 9
#define NODE_MUL 10
#define NODE_DIV 11
#define NODE_MOD 12
#define NODE_ARRAY 13
#define NODE_True 14
#define NODE_False 15
#define NODE_LT 16
#define NODE_GT 17
#define NODE_EQ 18
#define NODE_NEQ 19
#define NODE_LTE 20
#define NODE_GTE 21
#define NODE_AND 22
#define NODE_OR 23
#define NODE_NOT 24
#define NODE_STMT 25
#define NODE_ASGN 26
#define NODE_ARRAYASGN 27
#define NODE_READ 28
#define NODE_READARRAY 29
#define NODE_WRITE 30
#define NODE_IF 31
#define NODE_ELSE 32
#define NODE_WHILE 33
#define TYPE_boolean 34
#define TYPE_func_int 35
#define TYPE_func_str 36
#define TYPE_arg_int 37
#define TYPE_arg_str 38
#define NODE_FUNCTION 39
#define NODE_RET 40
#define NODE_MAIN 41
#define TYPE_arg_ref_int 42
#define TYPE_arg_ref_str 43
#define TYPE_param_val 44
#define TYPE_param_ref 45
#define TYPE_bool 46
#define NODE_PARAMETERS 47

struct Gsymbol
{
	int type;
	int size;
	int binding;
	char *name;
	struct Paramstruct *paramlist;
	struct Gsymbol *next;
};

struct Gsymbol *Ghead;

struct Gsymbol *Gsearch(char* name);

void Ginstall(char* name,int type,int size,struct Paramstruct *paramlist);

struct Paramstruct
{
	int ptype;
	char *pname;
	int pass_type;
	struct Paramstruct *next;
};

struct Paramstruct* install_list(int p_type,char *p_name,int passtype);

struct Lsymbol
{
	char *name;
	int type;
	int binding;
	struct Lsymbol *next;
};

struct Lsymbol *Lhead;

struct Lsymbol *Lsearch(char *name);

void Linstall(char *name,int type);

void reinitialise_lvar();

struct tnode 
{
	int TYPE;
	int NODETYPE;
	char* NAME; 
	int VALUE; 
	struct tnode *ptr1, *ptr2, *ptr3;
	struct Gsymbol *Gentry;
	struct Lsymbol *Lentry;
	struct tnode *arglist;
};

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char* NAME,struct tnode *ptr1, struct tnode *ptr2,struct tnode *ptr3,struct tnode *arglist);

struct tnode* makeLeafNode(int n);

struct tnode* makeOperatorNode(int op,struct tnode *l,struct tnode *r);

int get_staticaddr(int size);

struct label_node
{
	int label_num;
	int label_addr;
	struct label_node *next;
};

struct label_node *lroot;

void labelentry(int num,int addr);

int labelsearch(int num);

struct label_node_func
{
	int flabel_num;
	int flabel_addr;
	struct label_node_func *next;
};

struct label_node_func *flroot;

void func_labelentry(int fnum,int faddr);

int func_labelsearch(int fnum);

int get_reg();

void free_reg();

int get_label();

FILE *fp;
FILE *f;

int codegen(struct tnode *root);

void finalgen(FILE *fp,FILE *f);

int label_getposition(char* string);

int func_getposition(char *string);

int get_bind(struct tnode *t);

int gen_func_call(struct tnode *t);

void codegen_func(char *name,struct tnode *t);

int generate(struct tnode *t);
