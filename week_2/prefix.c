
struct tnode* makeLeafNode(int n)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->val = n;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = malloc(sizeof(char));
    *(temp->op) = c;
    temp->left = l;
    temp->right = r;
    return temp;
}

void preorder(struct tnode* root) 
{
   	if(root != NULL) 
	{
		if(root->op == NULL)
	      		printf("%d ",root->val);
		else
			printf("%c ",*(root->op));

	      preorder(root->left);
	      preorder(root->right);
   	}
}
    
