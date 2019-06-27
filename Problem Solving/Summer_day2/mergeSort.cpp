#include <stdio.h>
#include <stdlib.h>

struct node{
	int value;
	struct node *prev;
	struct node *next;
};

node* createNewNode(int data){
	node *newNode = (node*)malloc(sizeof(node));
	newNode->value = data;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

node* createDoubleLL(int *arr, int len){
	int i;
	node *head = createNewNode(arr[0]);
	node *curr = head;
	for (i = 1; i < len; i++){
		curr->next = createNewNode(arr[i]);
		curr->next->prev = curr;
		curr = curr->next;
	}
	return head;
}


node *merge(node *first, node *second)
{ 
	if (!first)
		return second;

	if (!second)
		return first;

	if (first->value < second->value)
	{
		first->next = merge(first->next, second);
		first->next->prev = first;
		first->prev = NULL;
		return first;
	}
	else
	{
		second->next = merge(first, second->next);
		second->next->prev = second;
		second->prev = NULL;
		return second;
	}
}


node* mergeSort(node* head){
	if (head->next == NULL){
		return head;
	}
	node *slow = head, *fast = head;

	while (fast->next!=NULL && fast->next->next!=NULL){
		fast = fast->next->next;
		slow = slow->next;
	}

	if (fast == slow && fast->next!=NULL){
		fast = fast->next;
		fast->prev->next = NULL;
		slow->next = NULL;
		return merge(mergeSort(slow), mergeSort(fast));
	}
	slow = slow->next;
	slow->prev->next = NULL;
	slow->prev = NULL;
	return merge(mergeSort(head), mergeSort(slow));
}

void main_mergeSort(){
	printf("\nMERGE SORT\n");
	int len, i;
	printf("Enter the number of elements: ");
	scanf_s("%d", &len);
	printf("Enter the elements :\n");
	if (len > 0){
		int *arr = (int*)malloc(sizeof(int)*len);
		for (i = 0; i < len; i++){
			scanf_s("%d", &arr[i]);
		}
		node *head = createDoubleLL(arr, len);
		head = mergeSort(head);
		printf("Sorted: \n");
		node *curr = head;
		while (curr != NULL){
			printf("%d ", curr->value);
			curr = curr->next;
		}
		free(head);
	}
	
}