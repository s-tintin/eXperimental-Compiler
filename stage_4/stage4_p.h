struct Tnode
{
	int type;
	int nodetype;
	int value;
	char name;
	struct Tnode *ptr1,*ptr2;
};

struct Tnode *TreeCreate(int TYPE,int NODETYPE,int VALUE,char NAME, struct Tnode *PTR1, struct Tnode *PTR2);

int evaluate (struct Tnode *t);

int variable[26];
int i;

