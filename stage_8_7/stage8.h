#define TYPE_int 1
#define TYPE_bool 2
#define TYPE_void 3
#define TYPE_intarray 4
#define TYPE_boolarray 5
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

struct Gsymbol 
{
    int type;   
    int size; 
    int binding;
    char* name; 
    struct Gsymbol *next; 
};

struct Gsymbol *Ghead;

struct Gsymbol *Gsearch(char* name);

void Ginstall(char* name,int type,int size);

struct tnode 
{
	int TYPE;
	int NODETYPE;
	char* NAME; 
	int VALUE; 
	struct tnode *ptr1, *ptr2, *ptr3;
	struct Gsymbol *Gentry;
};

struct tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char* NAME,struct tnode *ptr1, struct tnode *ptr2,struct tnode *ptr3);

struct tnode* makeLeafNode(int n);

struct tnode* makeOperatorNode(int op,struct tnode *l,struct tnode *r);

struct label_node
{
	int label_num;
	int label_addr;
	struct label_node *next;
};

struct label_node *lroot;

void labelentry(int num,int addr);

int labelsearch(int num);

int get_reg();

void free_reg();

int get_label();

int get_staticaddr(int size);

FILE *fp;
FILE *f;

void Generate(struct tnode *t,FILE *fp);

int codegen(struct tnode *root);

void finalgen(FILE *fp,FILE *f);

int getposition(char* string);
