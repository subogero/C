#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 100

struct t_bt {
	struct t_bt *branch1;
	struct t_bt *branch2;
	int leaf;
	char has_leaf;
};

void print_leaf(int leaf, int level)
{
	printf("%5d on level %2d\n", leaf, level);
}

/*
 * Create empty tree node
 */
struct t_bt *create_bt(void) {
	struct t_bt *new = malloc(sizeof(struct t_bt));
	new->branch1 = NULL;
	new->branch2 = NULL;
	new->has_leaf = 0;
	return new;
}

/*
 * Insert int into btree, lower value to 1st branch
 */
void insert_bt(struct t_bt *bt, int leaf, int level)
{
	level++;
	if (!bt->has_leaf) {
		level--;
		bt->leaf = leaf;
		bt->has_leaf = 1;
		print_leaf(bt->leaf, level);
	} else if (leaf < bt->leaf) {
		if (bt->branch1 == NULL) {
			bt->branch1 = create_bt();
		}
		insert_bt(bt->branch1, leaf, level);
	} else {
		if (bt->branch2 == NULL) {
			bt->branch2 = create_bt();
		}
		insert_bt(bt->branch2, leaf, level);
	}
}

/*
 * Traverse btree depth-1st recursively, AKA sort
 * Last param function, taking leaf and level values
 */
void dtravers_bt(struct t_bt *bt, int level, void (*fn)(int, int))
{
	if (bt == NULL)
		return;
	dtravers_bt(bt->branch1, level+1, fn);
	(*fn)(bt->leaf, level);
	dtravers_bt(bt->branch2, level+1, fn);
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
		insert_bt(bt, arr[i], 0);
	}
	close(fd_rand);

	printf("\nDepth-first traversal\n");
	dtravers_bt(bt, 0, &print_leaf);
	return 0;
}
