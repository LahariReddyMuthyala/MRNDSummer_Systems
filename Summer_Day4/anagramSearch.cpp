#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MaxChild 27

struct nodeLL{
	char *word;
	struct nodeLL *next;
};

struct Node{
	struct Node **children;
	struct nodeLL *dictionaryWords;
};


typedef struct Node node;

void displayLL(nodeLL *head){
	while (head != NULL){
		printf("%s ", head->word);
		head = head->next;
	}
}

node* createNewNode(){
	node *newNode = (node*)malloc(sizeof(node));
	newNode->children = (node**)calloc(MaxChild, sizeof(node*));
	newNode->dictionaryWords = NULL;
	return newNode;
}

nodeLL* createNewNodeLL(char *str){
	if (strlen(str) == 0) return NULL;
	nodeLL* newNode = (nodeLL*)malloc(sizeof(nodeLL));
	newNode->next = NULL;
	newNode->word = (char*)calloc(20, sizeof(char));
	strcpy(newNode->word, str);
	return newNode;
}

void insertWord(node *head, char *word, int index, nodeLL *headLL){
	if (index == strlen(word)-1){
		head->dictionaryWords = headLL;
		return;
	}
	else{
		node *curr = head;
		if (word[index] == '-'){
			if (curr->children[26] == NULL){
				curr->children[26] = createNewNode();
				curr = curr->children[26];
			}
			else{
				curr = curr->children[26];
			}

		}
		else if (curr->children[word[index] - 'a'] == NULL){
			curr->children[word[index] - 'a'] = createNewNode();
			curr = curr->children[word[index] - 'a'];

		}
		insertWord(curr, word, index + 1, headLL);
	}
}

nodeLL* getLL(char *str){
	char *firstWord = (char*)calloc(20, sizeof(char));
	int i = 0;
	while (str[i] != ' ' && str[i] != '\0') firstWord[i] = str[i++];
	firstWord[i] = '\0';
	i++;
	nodeLL *head = createNewNodeLL(firstWord);
	nodeLL *curr = head;
	char *buffer = (char*)calloc(20, sizeof(char));
	int k = 0;
	while(str[i] != '\0'){
		if (str[i] == ' ' || str[i] == '\t'){
			buffer[k] = '\0';
			curr->next = createNewNodeLL(buffer);
			curr = curr->next;
			k = 0;
			i++;
		}
		else{
			buffer[k++] = str[i++];
		}
	}
	buffer[k] = '\0';
	curr->next = createNewNodeLL(buffer);
	return head;
}

void sort(char *str){
	int i = 0, j = 0;
	char temp;
	for (i = 0; i < strlen(str) - 1; i++) {
		for (j = i + 1; j < strlen(str); j++) {
			if (str[i] > str[j]) {
				temp = str[i];
				str[i] = str[j];
				str[j] = temp;
			}
		}
	}
}

void readAndCreateTrie(node *head, char *fileName){
	FILE *fptr = fopen(fileName, "r+");
	char ch; int i = 0, k = 0;
	char *buffer = (char*)malloc(60*sizeof(char));
	char *buffer2 = (char*)calloc(15, sizeof(char));
	char *sortedWord = (char*)calloc(30, sizeof(char));
	char *words = (char*)calloc(75, sizeof(char));
	while (!feof(fptr)){
		k = 0;
		do{
			ch = fgetc(fptr);
			buffer[k++] = ch;
		} while (ch != '\n' && !feof(fptr));
		buffer[k] = '\0';
		k = 0; i = 0;
		while (buffer[k] != ' ') sortedWord[i++] = buffer[k++];
		sortedWord[i] = '\0';
		k++; i = 0;
		while (buffer[k] != '\0') words[i++] = buffer[k++];
		words[i] = '\0';
		nodeLL *headLL = getLL(words);
		insertWord(head, sortedWord, 0, headLL);
	}
}

nodeLL* search(node *head, char *str, int index){
	if (head == NULL){
		return NULL;
	}
	if (index == strlen(str)-1){
		return head->dictionaryWords;
	}
	printf("%c", str[index]);
	if (str[index] == '-'){
		return search(head->children[26], str, index + 1);
	}
	return search(head->children[str[index]-'a'], str, index + 1);
}



void test_anagramSearch(){
	node *head = createNewNode();
	readAndCreateTrie(head, "prep.txt");
	char *word = (char*)calloc(20, sizeof(char));
	printf("Enter the word: ");
	scanf("%s", word);
	sort(word);
	nodeLL *headLL = search(head, word, 0);
	if (headLL == NULL){
		printf("No anagrams in dictionary\n");
	}
	else{
		printf("Anagrams : ");
		displayLL(headLL);
		printf("\n");
	}
}