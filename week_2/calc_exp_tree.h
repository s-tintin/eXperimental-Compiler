typedef struct tnode
{
	int val;			//value of operand
	int val1;
	char *op;			//points to operator
	struct tnode *left,*right;  	//left and right branches
}tnode;

/*Make a leaf tnode and set the value of val field*/
struct tnode* makeLeafNode(int n);
struct tnode* makeLeafcharNode(char n);

/*Make a tnode with opertor, left and right branches set*/
struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r);

/*To evaluate an expression tree*/
int evaluate(struct tnode *t);
