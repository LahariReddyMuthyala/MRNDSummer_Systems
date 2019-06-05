#include <stdio.h>
#include <map>
#include <stdlib.h>

struct node{
	int data;
	struct node *next;
	struct node *rand;
};

typedef struct node node;

node* createNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = NULL;
	newNode->rand = NULL;
	return newNode;
}

node* clone_with_hashmap(node *head){
	node *curr = head;
	node *newhead = NULL;
	node *nextNode = NULL;

	std :: map <node*, node*> addressMap;
	while (curr){
		if (curr == head){
			newhead = createNode(curr->data);
			nextNode = newhead;
			addressMap[curr] = newhead;
			curr = curr->next;
		}
		else{
			nextNode->next = createNode(curr->data);
			nextNode = nextNode->next;
			addressMap[curr] = nextNode;
			curr = curr->next;

		}
	}

	curr = head;
	nextNode = newhead;
	while (curr){
		addressMap[curr]->rand = curr->rand;
		curr = curr->next;
	}
	return newhead;
}


void test_cloneHash(){
	printf("\nCLONE HASH\n");

	node* head = createNode(1);
	node* n2 = createNode(2);
	node* n3 = createNode(3);
	node* n4 = createNode(4);
	node* n5 = createNode(5);
	node* n6 = createNode(6);

	head->next = n2;
	n2->next = n3;
	n3->next = n4;
	n4->next = n5;
	n5->next = n6;
	n6->next = NULL;

	head->rand = n3;
	n2->rand = n5;
	n3->rand = n2;
	n4->rand = n5;
	n5->rand = n6;
	n6->rand = NULL;
	node *clone = clone_with_hashmap(head);
	while (clone)
	{
		printf("%d ", clone->data);
		clone = clone->next;
	}


}