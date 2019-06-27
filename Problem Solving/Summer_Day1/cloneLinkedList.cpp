#include<stdio.h>
#include<stdlib.h>

struct Node{
	int data;
	struct Node *next;
	struct Node *random;
};

typedef struct Node node;

node* createNewNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->random = NULL;
	newNode->next = NULL;
	return newNode;
}

void display(node* head){
	if (head == NULL)
		return;
	node* temp = head;
	//printf("%d ", temp->data);
	while (temp != NULL){
		printf("%d ", temp->data);
		temp = temp->next;
	}
}

node* getClone(node* head){
	node* curr = head;
	node* temp, *cloneHead;
	while (curr != NULL){
		temp = createNewNode(curr->data);
		temp->next = curr->next;
		curr->next = temp;
		curr = temp->next;
	}
	curr = head;
	while (curr->next != NULL && curr->next->next != NULL){
		if (curr->random != NULL)
			curr->next->random = curr->random->next;
		curr = curr->next->next;
	}

	curr = head;
	cloneHead = curr->next;
	while (curr->next != NULL && curr->next->next != NULL){
		temp = curr->next;
		curr->next = temp->next;
		temp->next = temp->next->next;
		curr = curr->next;
	}
	curr->next = NULL;
	return cloneHead;
}

void cloneLL(){
	printf("\nCLONE LINKED LIST WITH RANDOM POINTER\n");
	node* head = createNewNode(1);
	head->next = createNewNode(2);
	head->next->next = createNewNode(3);
	head->next->next->next = createNewNode(4);
	head->next->next->next->next = createNewNode(5);
	head->next->next->next->next->next = createNewNode(6);
	head->random = head->next->next;
	head->next->random = head->next->next->next->next;
	head->next->next->next->random = head->next->next->next->next;
	head->next->next->next->next->next->random = head->next->next->next;
	node* clone = getClone(head);
	printf("original : ");
	display(head);
	printf("\ncloned : ");
	display(clone);

}
