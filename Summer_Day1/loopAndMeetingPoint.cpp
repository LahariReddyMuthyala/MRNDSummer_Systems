#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node* next;
};

typedef struct node node;

node* createNewNode_loop(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

node* createLL_loop(int *arr, int len){
	node *head = createNewNode_loop(arr[0]);
	node *temp = head;
	int i;
	for (i = 1; i < len; i++){
		temp->next = createNewNode_loop(arr[i]);
		temp = temp->next;
		if (i == len - 1){
			temp->next = head->next->next->next;
		}
	}
	return head;
}

void display_loop(node *head){
	while (head != NULL){
		printf("%d ", head->data);
		head = head->next;
	}
}

void detectLoop(node *head){
	node *slow = head, *fast = head;
	do{
		slow = slow->next;
		fast = fast->next->next;
	} while (slow != fast && slow != NULL && fast != NULL);
	printf("Meeting point is at %d ", slow->data);
	slow = head;
	while (slow != fast){
		slow = slow->next;
		fast = fast->next;
	}
	printf("\nStarting point of loop is at %d\n", fast->data);
}

void test_loopAndMeetingPoint(){
	printf("\nDETECTING LOOP AND FINDING MEETING POINT IN LINKED LIST\n");
	int arr[9] = { 2, 1, 3, 4, 6, 5, 8, 7, 9 };
	node* head = createLL_loop(arr, 9);
	detectLoop(head);
}
