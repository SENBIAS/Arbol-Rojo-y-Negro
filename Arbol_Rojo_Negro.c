#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BLACK 0
#define RED 1
#define NILkey -2147483647

struct nodeRBTree
{
	int key;
	int color;
	int pos;
	struct nodeRBTree *left;
	struct nodeRBTree *right;
	struct nodeRBTree *p;
};

struct nodeRBTree *AssignNilLeaf()
{
	struct nodeRBTree *z;
	z = (struct nodeRBTree *)malloc(sizeof(struct nodeRBTree));
	z->key = NILkey;
	z->color = BLACK;
	z->p = NULL;
	z->left = NULL;
	z->right = NULL;
	return z;
}

void RB_InorderTreeWalk(struct nodeRBTree *x)
{
	if(x->key != NILkey)
	{
		RB_InorderTreeWalk(x->left);
		if(x->color == RED)
			printf(" (%d, Red)", x->key);
		else
			printf(" (%d, Black)", x->key);
		RB_InorderTreeWalk(x->right);
	}
}

struct nodeRBTree *RB_TreeSearch(struct
					nodeRBTree *x, int k)
{
	if((x->key == NILkey) || (k == x->key))
		return x;
	if(k < x->key)
		return RB_TreeSearch(x->left, k);
	else
		return RB_TreeSearch(x->right, k);
}

struct nodeRBTree *RB_TreeMinimum(struct
							nodeRBTree *x)
{
	while(x->left->key != NILkey)
		x = x->left;
	return x;
}

struct nodeRBTree *RB_TreeMaximum(struct
							nodeRBTree *x)
{
	while(x->right->key != NILkey)
		x = x->right;
	return x;
}

struct nodeRBTree *RB_TreePredeccessor(struct
								nodeRBTree *x)
{
	struct nodeRBTree *y;
	if(x->left->key != NILkey)
		return RB_TreeMaximum(x->left);
	y = x->p;
	while((y->key != NILkey) && (x == y->left))
	{
		x = y;
		y = y->p;
	}
	return y;
}

struct nodeRBTree *RB_TreeSuccessor(struct
							nodeRBTree *x)
{
	struct nodeRBTree *y;
	if(x->right->key != NILkey)
		return RB_TreeMinimum(x->right);
	y = x->p;
	while((y->key != NILkey) && (x == y->right))
	{
		x = y;
		y = y->p;
	}
	return y;
}

struct nodeRBTree *LeftRotate(struct nodeRBTree *T,
							struct nodeRBTree *x)
{
	struct nodeRBTree *y;
	y = x->right;
	x->right = y->left;
	y->left->p = x;
	y->p = x->p;

	if(x->p->key == NILkey)
		T = y;
	else
	{
		if(x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;
	}
	y->left = x;
	x->p = y;
	return T;
}


struct nodeRBTree *RightRotate(struct nodeRBTree *T,
							struct nodeRBTree *x)
{
	struct nodeRBTree *y;
	y = x->left;
	x->left = y->right;
	y->right->p = x;
	y->p = x->p;

	if(x->p->key == NILkey)
		T = y;
	else
	{
		if(x == x->p->right)
			x->p->right = y;
		else
			x->p->left = y;
	}
	y->right = x;
	x->p = y;
	return T;
}

struct nodeRBTree *RB_InsertFixup(struct nodeRBTree *T,
								struct nodeRBTree *z)
{
	struct nodeRBTree *x, *y;
	while(z->p->color == RED)
	{
		if(z->p == z->p->p->left)
		{
			y = z->p->p->right;
			if(y->color == RED)
			{
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else
			{
				if(z == z->p->right)
				{
					z = z->p;
					T = LeftRotate(T, z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				T = RightRotate(T, z->p->p);
			}
		}
		else
		{
			y = z->p->p->left;
			if(y->color == RED)
			{
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else
			{
				if(z == z->p->left)
				{
					z = z->p;
					T = RightRotate(T, z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				T = LeftRotate(T, z->p->p);
			}
		}
	}

	T->color = BLACK;
	return T;
}


struct nodeRBTree *RB_TreeInsert(
	struct nodeRBTree *T, struct nodeRBTree *z)
{
	struct nodeRBTree *x, *y;

	if(T->key != NILkey)
        y = T->p;
    else
        y = T;

	x = T;
	while(x->key != NILkey)
	{
		y = x;
		if(z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if(y->key == NILkey)
		T = z;
	else
	{
		if(z->key < y->key)
		{
			free(y->left);
			y->left = z;
		}
		else
		{
			free(y->right);
			y->right = z;
		}
	}
	z->left = AssignNilLeaf();
	z->left->p = z;
	z->right = AssignNilLeaf();
	z->right->p = z;
	z->color = RED;
	return RB_InsertFixup(T, z);
}

struct nodeRBTree *RB_DeleteFixup(
	struct nodeRBTree *T, struct nodeRBTree *x)
{
	struct nodeRBTree *w;
	while((x != T) && (x->color == BLACK))
	{
		if(x == x->p->left)
		{
			w = x->p->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				T = LeftRotate(T, x->p);
				w = x->p->right;
			}
			if((w->left->color == BLACK) &&
				(w->right->color == BLACK))
			{
				w->color = RED;
				x = x->p;
			}
			else
			{
				if(w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					T = RightRotate(T, w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				T = LeftRotate(T, x->p);
				x = T;
			}
		}
		else
		{
			w = x->p->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				T = RightRotate(T, x->p);
				w = x->p->left;
			}
			if((w->left->color == BLACK) &&
				(w->right->color == BLACK))
			{
				w->color = RED;
				x = x->p;
			}
			else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					T = LeftRotate(T, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				T = RightRotate(T, x->p);
				x = T;
			}
		}
	}
	x->color = BLACK;
	return T;
}

struct nodeRBTree *RB_TreeDelete(
	struct nodeRBTree *T, struct nodeRBTree *z)
{
	struct nodeRBTree *x, *y;
	if((z->left->key == NILkey) || (z->right->key == NILkey))
		y = z;
	else
		y = RB_TreeSuccessor(z);
	if(y->left->key != NILkey)
		x = y->left;
	else
		x = y->right;
	x->p = y->p;
	if(y->p->key == NILkey)
		T = x;
	else
	{
		if(y == y->p->left)
			y->p->left = x;
		else
			y->p->right = x;
	}

	if(y != z)
		z->key = y->key;

	if(y->color == BLACK)
		T = RB_DeleteFixup(T, x);

	if(y->left->p == y)
		free(y->left);

	if(y->right->p == y)
		free(y->right);

	free(y);
	return T;
}


struct nodeRBTree *RB_DeleteWholeTree(struct nodeRBTree *T)
{
    while(T->key != NILkey)
        T = RB_TreeDelete(T, T);

    return T;
}
