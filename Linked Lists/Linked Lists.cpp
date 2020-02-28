#include <stdio.h>
#include <stdlib.h>

// an ordinary node with val, and pointer to the next
typedef struct intNode {
	int val;
	struct intNode *next;
} node_int;
// headerNode is a special node, in order to store the size of list 
typedef struct intHeaderNode {
	int size;
	struct intNode *first;
} int_headerNode;
// push will push an element to the tail of the list
void int_push(int_headerNode *head, int val) {
	if (head->size == 0) {	// size 0 need another way to do since headerNode only contains a pointer to node
		head->first = (node_int *)malloc(sizeof(node_int));
		head->first->val = val;
		head->first->next = NULL;
	}
	else {
		node_int *current = head->first;
		while (current->next != NULL) {
			current = current->next;	// finding the tail
		}
		current->next = (node_int *)malloc(sizeof(node_int));
		current->next->val = val;
		current->next->next = NULL;
	}
	head->size++;
}
// pop will remove the last node of the list and return the value of last node
int int_pop(int_headerNode *head) {
	int val = 0;
	if (head->size == 1) {	// size 1 need another way to do since headerNode only contains a pointer to node
		val = head->first->val;
		free(head->first);
		head->first = NULL;
	}
	else if (head->size > 1) {
		node_int *current = head->first;
		while (current->next->next != NULL) {
			current = current->next;	// finding the element before tail
		}
		val = current->next->val;
		free(current->next);
		current->next = NULL;
	}
	head->size--;
	return val;
}
// clear, clears whole list
void int_clear(int_headerNode *head) {
	if (head->size == 0)
		return;
	node_int *forward = head->first;
	node_int *current = head->first;
	while (current->next != NULL) {
		forward = current;
		current = current->next;	// finding the tail
		free(forward);
	}
	free(current);
	head->size = 0;
	/*int eles = head->size;
	for (int i = 0; i < eles; i++)
		int_pop(head);*/
}
// assign, lhs = rhs
void int_assign(int_headerNode *lhs, const int_headerNode *rhs) {
	int_clear(lhs);
	node_int *rhsCurrent = rhs->first;
	while (rhsCurrent != NULL) {
		int_push(lhs, rhsCurrent->val);
		rhsCurrent = rhsCurrent->next;
	}
}
// determine if two lists are equal and return true, otherwise return false
int int_equal(const int_headerNode *lhs, const int_headerNode *rhs)
{
	if (lhs->size != rhs->size)
		return 0;
	else {
		node_int *lhsCurrent = lhs->first;
		node_int *rhsCurrent = rhs->first;
		while (lhsCurrent != NULL) {
			if (lhsCurrent->val != rhsCurrent->val)
				return 0;
			lhsCurrent = lhsCurrent->next;
			rhsCurrent = rhsCurrent->next;
		}
		return 1;
	}
}
// to access the list like an array
node_int* int_get(const int_headerNode *head, int key)
{
	node_int *current = head->first;
	for (int i = 0; i < key; i++)
		current = current->next;
	return current;
}

// an ordinary node with cotent, and pointer to the next
typedef struct intlistNode {
	struct intHeaderNode content;
	struct intlistNode *next;
} node_intlist;
// intlistHeaderNode is a special node, in order to store the size of intlist 
typedef struct intlistHeaderNode {
	int size;
	struct intlistNode *first;
} intlist_headerNode;

// push will push an element to the tail of the list
// not using pass by point is in order to create new copy of the content
void intlist_push(intlist_headerNode *head, intHeaderNode *content) {
	if (head->size == 0) {	// size 0 need another way to do since headerNode only contains a pointer to node
		head->first = (node_intlist *)malloc(sizeof(node_intlist));
		head->first->content.size = 0;
		for (int i = 0; i < content->size; i++) {
			int_push(&head->first->content, int_get(content, i)->val);
		}
		head->first->next = NULL;
	}
	else {
		node_intlist *current = head->first;
		while (current->next != NULL) {
			current = current->next;	// finding the tail
		}
		current->next = (node_intlist *)malloc(sizeof(node_intlist));
		current->next->content.size = 0;
		for (int i = 0; i < content->size; i++) {
			int_push(&current->next->content, int_get(content, i)->val);
		}
		current->next->next = NULL;
	}
	head->size++;
}
// pop will remove the last node of the list and return the value of last node
intHeaderNode intlist_pop(intlist_headerNode *head) {
	intHeaderNode content = { 0, NULL };
	if (head->size == 1) {	// size 1 need another way to do since headerNode only contains a pointer to node
		content = head->first->content;
		int_clear(&head->first->content);
		free(head->first);
		head->first = NULL;
	}
	else if (head->size > 1) {
		node_intlist *current = head->first;
		while (current->next->next != NULL) {
			current = current->next;	// finding the element before tail
		}
		content = current->next->content;
		int_clear(&current->next->content);
		free(current->next);
		current->next = NULL;
	}
	head->size--;
	return content;
}
// clear, clears whole list
void intlist_clear(intlist_headerNode *head) {
	int eles = head->size;
	for (int i = 0; i < eles; i++)
		intlist_pop(head);
}
// determine if two lists are equal and return true, otherwise return false
int intlist_equal(const intlist_headerNode *lhs, const intlist_headerNode *rhs)
{
	if (lhs->size != rhs->size)
		return 0;
	else {
		node_intlist *lhsCurrent = lhs->first;
		node_intlist *rhsCurrent = rhs->first;
		while (lhsCurrent != NULL) {
			if (int_equal(&lhsCurrent->content, &rhsCurrent->content))
				return 0;
			lhsCurrent = lhsCurrent->next;
			rhsCurrent = rhsCurrent->next;
		}
		return 1;
	}
}
// to access the list like an array
node_intlist* intlist_get(const intlist_headerNode *head, int key)
{
	node_intlist *current = head->first;
	for (int i = 0; i < key; i++)
		current = current->next;
	return current;
}

int main(int argc, char *argv[])
{
	int_headerNode FIr = { 0, NULL };
	for (int i = 0; i < 10; i++) {
		int_push(&FIr, i);
	}
	int_headerNode FIr1 = { 0, NULL };
	int_assign(&FIr1, &FIr);
	int_headerNode FIr2 = { 0, NULL };
	for (int i = 20; i < 31; i++) {
		int_push(&FIr2, i);
	}
	int_assign(&FIr2, &FIr);

	intlist_headerNode intL = { 0, NULL };

	intlist_push(&intL, &FIr);
	printf("%d\n", intL.size);

	int_pop(&FIr);
	intlist_push(&intL, &FIr);
	printf("%d\n", intL.size);

	printf("%d\n", intlist_get(&intL, 0)->content.size);
	int_pop(&intlist_get(&intL, 0)->content);
	printf("%d\n", intlist_get(&intL, 0)->content.size);
	int_pop(&FIr2);
	printf("%d\n", int_equal(&FIr, &FIr2));
	
	int_clear(&FIr);
	printf("%d\n", FIr.size);
	printf("%d\n", int_equal(&intlist_get(&intL, 0)->content, &intlist_get(&intL, 1)->content));
}