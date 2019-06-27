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

node* createLL(int arr[], int len){
	node *head = createNewNode(arr[0]);
	node *temp = head;
	int i;
	for (i = 1; i < len; i++){
		temp->next = createNewNode(arr[i]);
		temp = temp->next;
	}
	return head;
}

void display(node *head){
	while (head != NULL){
		printf("%d ", head->value);
		head = head->next;
	}
}

int length(node *head){
	int len = 0;
	while (head != NULL){
		len++;
		head = head->next;
	}
	return len;
}

void insertAtSecond(int data, node* head){
	node* temp = createNewNode(data);
	if (head->next == NULL){
		head->next = temp;
	}
	else{
		temp->next = head->next;
		head->next = temp;
	}

}

int addNodes(node *longer, node *shorter, int diff, node* sum){
	if (shorter == NULL && longer == NULL)
		return 0;
	if (diff > 0){
		int sumTemp = longer->value + addNodes(longer->next, shorter, diff - 1, sum);
		insertAtSecond(sumTemp % 10, sum);
		return sumTemp / 10;
	}
	else{
		int sumTemp = shorter->value + longer->value + addNodes(longer->next, shorter->next, -1, sum);
		insertAtSecond(sumTemp % 10, sum);
		return sumTemp / 10;
	}
}

node* getLL(node *longer, node*shorter, int diff){
	node *sumLL = createNewNode(0);
	int carry = addNodes(longer, shorter, diff, sumLL);
	if (carry > 0){
		sumLL->value = carry;
	}
	else{
		sumLL = sumLL->next;
	}
	return sumLL;
}

void getSum(node* num1, node* num2){
	node* longer = length(num1) >= length(num2) ? num1 : num2;
	node* shorter = length(num1) < length(num2) ? num1 : num2;
	display(longer);
	printf(" + ");
	display(shorter);
	printf(" = ");
	int diff = length(num1) >= length(num2) ? length(num1) - length(num2) : length(num2) - length(num1);
	node *sum = getLL(longer, shorter, diff);
	display(sum);
}

void testAddition(){
	printf("\nADDITION OF TWO LINKED LISTS\n");
	int number1[3] = { 9,9,9 };
	int number2[3] = { 9,8,7 };
	int len1 = 3, len2 = 3, i;
	node *num1 = createLL(number1, len1);
	node *num2 = createLL(number2, len2);
	display(num1);
	printf("\n");
	display(num2);
	printf("\n");
	getSum(num1, num2);
}
