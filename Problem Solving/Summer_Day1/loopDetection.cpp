#include<stdio.h>
#include<stdlib.h>

struct node{
	int value;
	struct node *next;
};

typedef struct node node;

node* createNewNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->value = data;
	newNode->next = NULL;
	return newNode;
}

node* createLL(int *arr, int len){
	node *head = createNewNode(arr[0]);
	node *temp = head;
	int i;
	for (i = 1; i < len; i++){
		temp->next = createNewNode(arr[i]);
		temp = temp->next;
	}
	temp->next = head->next->next;
	return head;
}

node* detectLoop(node *head){
	node *fast=head, *slow=head;
	while (fast != slow){
		fast = fast->next->next;
		slow = slow->next;
	}

}
