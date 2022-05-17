#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT 1
#define FILE_INPUT 2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int data;
	struct node *left;
	struct node *right;
} NODE;

typedef struct
{
	NODE *root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create(void);

/* Deletes all data in tree and recycles memory
 */
void BST_Destroy(TREE *pTree);

/* internal function (not mandatory)
 */
static void _destroy(NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE *pTree, int data);

/* internal function (not mandatory)
 */
static void _insert(NODE *root, NODE *newPtr);

NODE *_makeNode(int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete(NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve(TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, int key);

/* prints tree using inorder traversal
 */
void BST_Traverse(TREE *pTree);
static void _traverse(NODE *root);

/* Print tree using inorder right-to-left traversal
 */
void printTree(TREE *pTree);
/* internal traversal function
 */
static void _inorder_print(NODE *root, int level);

/*
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}

	FILE *fp;

	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else
		mode = FILE_INPUT;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf("Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert(numbers > 0);

		fprintf(stdout, "Inserting: ");

		srand(time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3)

			fprintf(stdout, "%d ", data);

			// insert function call
			int ret = BST_Insert(tree, data);
			if (!ret)
				break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf(stdout, "Inserting: ");

		while (fscanf(fp, "%d", &data) != EOF)
		{
			fprintf(stdout, "%d ", data);

			// insert function call
			int ret = BST_Insert(tree, data);
			if (!ret)
				break;
		}
		fclose(fp);
	}

	fprintf(stdout, "\n");

	if (BST_Empty(tree))
	{
		fprintf(stdout, "Empty tree!\n");
		BST_Destroy(tree);
		return 0;
	}

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left inorder traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		if (scanf("%d", &num) == EOF)
			break;

		int ret = BST_Delete(tree, num);
		if (!ret)
		{
			fprintf(stdout, "%d not found\n", num);
			continue;
		}

		// print tree with right-to-left inorder traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

		if (BST_Empty(tree))
		{
			fprintf(stdout, "Empty tree!\n");
			break;
		}
	}

	BST_Destroy(tree);

	return 0;
}

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create(void)
{
	TREE *newTree = (TREE *)malloc(sizeof(TREE));
	// NODE* newNode = (NODE*)malloc(sizeof(NODE));
	// newNode -> data

	if (newTree == NULL)
	{
		return NULL;
	}
	newTree->root = NULL;
	return newTree;
}

/* Deletes all data in tree and recycles memory
 */
void BST_Destroy(TREE *pTree)
{
	_destroy(pTree->root);
}

/* internal function (not mandatory)
 */
static void _destroy(NODE *root)
{
	if (root->left != NULL)
	{
		_destroy(root->left);
	}
	if (root->right != NULL)
	{
		_destroy(root->right);
	}

	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE *pTree, int data)
{
	NODE *newPtr = _makeNode(data);
	if (newPtr == NULL)
		return 0;

	if (pTree->root == NULL)
	{
		pTree->root = newPtr;
		return 1;
	}
	_insert(pTree->root, newPtr);
	return 1;
}

/* internal function (not mandatory)
 */
static void _insert(NODE *root, NODE *newPtr) // root에 NULL값이 들어오면 위치 알려줄 수 없음. 함수의 parameter는 함수의 stack에 저장됨.
{
	NODE *p;
	NODE *t;

	p = root;

	while (p != NULL)
	{
		if (newPtr->data < p->data)
		{
			t = p;
			p = p->left;
		}
		else
		{
			t = p;
			p = p->right;
		}
	}

	if (newPtr->data < t->data)
		t->left = newPtr;
	else
		t->right = newPtr;
	return;
}

NODE *_makeNode(int data)
{
	NODE *new = (NODE *)malloc(sizeof(NODE));
	if (new == NULL)
	{
		return NULL;
	}
	new->data = data;
	new->left = NULL;
	new->right = NULL;
	return new;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE *pTree, int dltKey)
{
	int success;

	if(pTree -> root != NULL){
		_delete(pTree->root, dltKey, &success);
	}
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete(NODE *root, int dltKey, int *success)
{
	if(root == NULL){
		*success = 0;
		return NULL;
	}

	if (dltKey < root->data)
		_delete(root->left, dltKey, success);
	else if (dltKey > root->data)
		_delete(root->right, dltKey, success);
	else 
		if (root->left == NULL)
		{
			root->data = root->right->data;
			*success = 1;
			free(root->right);
			return root;
		}
		else if (root->right == NULL)
		{
			root->data = root->left->data;
			*success = 1;
			free(root->left);
			return root;
		}
		else
		{
			NODE *right_smallest;
			right_smallest = root->right;
			while (right_smallest->left != NULL)
			{
				right_smallest = right_smallest -> left;
			}
			root->data = right_smallest->data;
			
		}
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
// BST_Retrieve를 insert와 delete에 사용?
int *BST_Retrieve(TREE *pTree, int key)
{
	NODE *find = (NODE *)malloc(sizeof(NODE));
	if (find == NULL)
	{
		return 0;
	}

	if (pTree != NULL)
		find = _retrieve(pTree->root, key);
	if (find == NULL)
		return NULL;

	return &(find->data); // local 변수 반환 X. 포인터 찾자
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, int key)
{
	if (!root)
	{
		return NULL;
	}

	if (key < root->data)
		return _retrieve(root->left, key);
	else if (key > root->data)
		return _retrieve(root->right, key);
	else
		return root;
}

/* prints tree using inorder traversal
 */
void BST_Traverse(TREE *pTree)
{
	if (pTree->root == NULL)
	{
		return;
	}
	_traverse(pTree->root);
}
static void _traverse(NODE *root)
{
	if (root->left != NULL)
		_traverse(root->left);

	printf("%d ", root->data);

	if (root->right != NULL)
		_traverse(root->right);
}

/* Print tree using inorder right-to-left traversal
 */
void printTree(TREE *pTree)
{
	int level = 0;
	if (pTree->root == NULL)
	{
		return;
	}
	_inorder_print(pTree->root, level);
}
/* internal traversal function
 */
static void _inorder_print(NODE *root, int level)
{
	if (root->right != NULL)
	{
		_inorder_print(root->right, level + 1);
	}

	for (int i = 0; i < level; i++)
	{
		printf("\t");
	}
	printf("%d \n", root->data);

	if (root->left != NULL)
	{
		_inorder_print(root->left, level + 1);
	}
}

/*
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE *pTree)
{
	if (pTree->root == NULL)
		return 1;
	return 0;
}