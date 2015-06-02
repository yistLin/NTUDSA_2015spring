#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include "bst.h"
#include "rb.h"

void preorder_integer_avl(const struct avl_node *node){
	if (node == NULL)
		return;
	printf("%d ", *((int*)node->avl_data));
	if (node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		putchar('(');
		preorder_integer_avl(node->avl_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_avl(node->avl_link[1]);
		putchar(')');
	}
}

void preorder_integer_bst(const struct bst_node *node){
	if (node == NULL)
		return;
	printf("%d ", *((int*)node->bst_data));
	if (node->bst_link[0] != NULL || node->bst_link[1] != NULL){
		putchar('(');
		preorder_integer_bst(node->bst_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_bst(node->bst_link[1]);
		putchar(')');
	}
}

void preorder_integer_rb(const struct rb_node *node){
	if (node == NULL)
		return;
	printf("%d ", *((int*)node->rb_data));
	if (node->rb_link[0] != NULL || node->rb_link[1] != NULL){
		putchar('(');
		preorder_integer_rb(node->rb_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_rb(node->rb_link[1]);
		putchar(')');
	}
}

int int_compare(const void *pa, const void *pb, void *param) {
	int a = *(const int *)pa;
	int b = *(const int *)pb;
	if (a < b) return -1;
	else if (a > b) return +1;
	else return 0;
}

int main(){
	struct avl_table *tree1;
	tree1 = avl_create(int_compare, NULL, NULL);

	struct bst_table *tree2;
	tree2 = bst_create(int_compare, NULL, NULL);

	struct rb_table *tree3;
	tree3 = rb_create(int_compare, NULL, NULL);

	int i;
	for (i=0; i<16; ++i){
		int* element = (int*)malloc(sizeof(int));
		*element = i;
		void **p = avl_probe(tree1, element);
	}
	preorder_integer_avl(tree1->avl_root);
	puts("");

	for (i=0; i<16; ++i){
		int* element = (int*)malloc(sizeof(int));
		*element = i;
		void **p = bst_probe(tree2, element);
	}
	preorder_integer_bst(tree2->bst_root);
	puts("");

	for (i=0; i<16; ++i){
		int* element = (int*)malloc(sizeof(int));
		*element = i;
		void **p = rb_probe(tree3, element);
	}
	preorder_integer_rb(tree3->rb_root);
	puts("");

	return 0;
}