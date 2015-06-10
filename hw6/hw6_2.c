#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "bst.h"
#include "rb.h"

#define MAX_LENGTH 128
#define TOTAL_STR 32

void preorder_string_avl(const struct avl_node *node){
  if (node == NULL)
    return;
  printf("%s ", (char*)node->avl_data);
  if (node->avl_link[0] != NULL || node->avl_link[1] != NULL){
    putchar('(');
    preorder_string_avl(node->avl_link[0]);
    putchar(',');
    putchar(' ');
    preorder_string_avl(node->avl_link[1]);
    putchar(')');
  }
}

void preorder_string_bst(const struct bst_node *node){
  if (node == NULL)
    return;
  printf("%s ", (char*)node->bst_data);
  if (node->bst_link[0] != NULL || node->bst_link[1] != NULL){
    putchar('(');
    preorder_string_bst(node->bst_link[0]);
    putchar(',');
    putchar(' ');
    preorder_string_bst(node->bst_link[1]);
    putchar(')');
  }
}

void preorder_string_rb(const struct rb_node *node){
  if (node == NULL)
    return;
  printf("%s ", (char*)node->rb_data);
  if (node->rb_link[0] != NULL || node->rb_link[1] != NULL){
    putchar('(');
    preorder_string_rb(node->rb_link[0]);
    putchar(',');
    putchar(' ');
    preorder_string_rb(node->rb_link[1]);
    putchar(')');
  }
}

int str_compare(const void *pa, const void *pb, void *param) {
  const char* a = (const char *)pa;
  const char* b = (const char *)pb;
  int res = strcmp(a, b);
  if (res < 0) return -1;
  else if (res == 0) return 0;
  else return +1;
}

int main(){
  char istr[TOTAL_STR][MAX_LENGTH];
  int i;
  int len;
  for (i=0; i<TOTAL_STR; ++i){
    fgets(istr[i], MAX_LENGTH+1, stdin);
    len = (int)strlen(istr[i]);
    if (istr[i][len - 1] == '\n')
      istr[i][len - 1] = '\0';
  }

  struct avl_table *tree1;
  tree1 = avl_create(str_compare, NULL, NULL);

  struct bst_table *tree2;
  tree2 = bst_create(str_compare, NULL, NULL);

  struct rb_table *tree3;
  tree3 = rb_create(str_compare, NULL, NULL);

  for (i=0; i<TOTAL_STR; ++i) {
    char* element = (char*)malloc(sizeof(char)*MAX_LENGTH);
    strcpy(element, istr[i]);
    void **p = avl_probe(tree1, element);

    element = (char*)malloc(sizeof(char)*MAX_LENGTH);
    strcpy(element, istr[i]);
    p = bst_probe(tree2, element);

    element = (char*)malloc(sizeof(char)*MAX_LENGTH);
    strcpy(element, istr[i]);
    p = rb_probe(tree3, element);
  }
  preorder_string_avl(tree1->avl_root);
  puts("");
  preorder_string_bst(tree2->bst_root);
  puts("");
  preorder_string_rb(tree3->rb_root);
  puts("");

  return 0;
}