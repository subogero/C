#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 31

struct t_bt {
	struct t_bt *left;
	struct t_bt *right;
	int leaf;
	char has_leaf;
	char height;
};

void print_leaf(int leaf, int level)
{
	printf("%5d level %2d\n", leaf, level);
}

/*
 * Create empty tree node
 */
struct t_bt *create_bt(void) {
	struct t_bt *new = malloc(sizeof(struct t_bt));
	new->left = NULL;
	new->right = NULL;
	new->has_leaf = 0;
	new->height = -1;
	return new;
}

/*
 * Calculate height of tree node
 */
void calc_height(struct t_bt *bt)
{
	int height = -1;
	if (bt->left != NULL && bt->left->height > height)
		height = bt->left->height;
	if (bt->right != NULL && bt->right->height > height)
		height = bt->right->height;
	bt->height = height + 1;
}

/*
 * Left rotate tree node - return left as root
 */
struct t_bt *left_rotate(struct t_bt *bt)
{
	printf("%5d Rotate left\n", bt->leaf);
	struct t_bt *root = bt->left;

	bt->left = root->right;
	calc_height(bt);

	root->right = bt;
	calc_height(root);

	return root;
}

/*
 * Right rotate tree node - return right as root
 */
struct t_bt *right_rotate(struct t_bt *bt)
{
	printf("%5d Rotate right\n", bt->leaf);
	struct t_bt *root = bt->right;

	bt->right = root->left;
	calc_height(bt);

	root->left = bt;
	calc_height(root);

	return root;
}

/*
 * Insert int into btree, lower value to 1st branch
 */
struct t_bt *insert_bt(struct t_bt *bt, int leaf, int level)
{
	level++;
	if (bt == NULL)
		bt = create_bt();
	if (!bt->has_leaf) {
		level--;
		bt->leaf = leaf;
		bt->has_leaf = 1;
		printf("%5d Insert\n", leaf);
	} else if (leaf < bt->leaf) {
		printf("%5d Pass left\n", leaf);
		bt->left = insert_bt(bt->left, leaf, level);
		/* Balance if necessary */
		char r_height = bt->right ? bt->right->height : -1;
		if (bt->left->height >= r_height + 2) {
			if (leaf >= bt->left->leaf)
				bt->left = right_rotate(bt->left);
			return left_rotate(bt);
		}
	} else {
		printf("%5d Pass right\n", leaf);
		bt->right = insert_bt(bt->right, leaf, level);
		/* Balance if necessary */
		char l_height = bt->left ? bt->left->height : -1;
		if (bt->right->height >= l_height + 2) {
			if (leaf < bt->right->leaf)
				bt->right = left_rotate(bt->right);
			return right_rotate(bt);
		}
	}

	calc_height(bt);
	return bt;
}

/*
 * Traverse btree depth-1st recursively, AKA sort
 * Last param function, taking leaf and level values
 */
void dtravers_bt(struct t_bt *bt, int level, void (*fn)(int, int))
{
	if (bt == NULL)
		return;
	dtravers_bt(bt->left, level+1, fn);
	(*fn)(bt->leaf, level);
	dtravers_bt(bt->right, level+1, fn);
}

/*
 * Search btree, return node with searched number
 */
struct t_bt *search_bt(struct t_bt *bt, int val)
{
	if (bt == NULL)
		return NULL;
	if (bt->leaf == val)
		return bt;
	struct t_bt *left_hit = search_bt(bt->left, val);
	if (left_hit != NULL)
		return left_hit;
	struct t_bt *right_hit = search_bt(bt->right, val);
	if (right_hit != NULL)
		return right_hit;
	return NULL;
}

int main(int argc, char *argv[])
{
	struct t_bt *bt = create_bt();
	int fd_rand = open("/dev/urandom", O_RDONLY);
	int arr[N];
	int i;
	for (i = 0; i < N; ++i) {
		arr[i] = 0;
		read(fd_rand, arr + i, 2);
		bt = insert_bt(bt, arr[i], 0);
	}
	close(fd_rand);

	printf("=== Depth-first traversal\n");
	dtravers_bt(bt, 0, &print_leaf);

	printf("=== Search %5d\n", arr[N/2]);
	struct t_bt *hit = search_bt(bt, arr[N/2]);
	printf("%5d found\n", hit != NULL ? hit->leaf : -1);

	return 0;
}
