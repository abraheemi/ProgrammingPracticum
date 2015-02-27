/***********************************
* Author: Abraheem Irheem
*
* Class: CS 211, FALL 2014
*
* System: g++ on Ubuntu 64bit
*
* Program: Restaurant
*
* This program implements a restaurant waiting list.
* The user is prompted to enter a command to perform
* a certain operation on the waiting list.
* Operations include adding a group, deleting a group,
* listing groups, etc...
* 
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;


typedef struct node{
	char Name[30];
	int Size;
	boolean Status;
	struct node *next;
} Node;


/* forward definition of functions */
void clearToEoln();


/* ------------------------------------------------------------- */
/* Read in until the first Non-White-Space character is Read */
/* The white space characters are:
 *      space, tab \t, newline \n, vertical tab \v, 
 *      form feed \f, and carriage return \r
 */ 
int getNextNWSChar (){
  int ch;

 	ch = getc(stdin);
 	if (ch == EOF)
   		return ch;
 	while (isspace (ch)){
	    ch = getc(stdin);
    	if (ch == EOF)
      	return ch;
   	}
 	return ch;
}


/* ------------------------------------------------------------- */
/* read in the next Positive Integer or error:    */
/* This is based on the Mathematical definition of a Postive Integer */
/*    zero is not counted as a positive number */ 
int getPosInt (){
  int value = 0;

 	/* clear white space characters */
 	int ch;
	ch = getc(stdin);
 	while (!isdigit(ch)){
    	if ('\n' == ch)  /* error \n ==> no integer given */
       		return 0;
    	if (!isspace(ch)) /* error non white space ==> integer not given next */{
       		clearToEoln();
       		return 0;
      	}
    	ch = getc(stdin);
   	}

 	value = ch - '0';
 	ch = getc(stdin);
 	while (isdigit(ch)){
    	value = value * 10 + ch - '0';
    	ch = getc(stdin);
   	}

 	ungetc (ch, stdin);  /* put the last read character back in input stream */

 	/* Integer value of 0 is an error in this program */
 	if (0 == value)
    	clearToEoln();
   
 	return value;
}


/* ------------------------------------------------------------- */
/* read in the name until the end of the input */
char *getName(){
  /* skip over the white space characters */
  int ch;
	ch = getc(stdin);
 	while (isspace(ch)){
	    if ('\n' == ch)  /* error \n ==> no integer given */
       	return NULL;
    	ch = getc(stdin);
   	}

  char *word;
  int size;
 	size = 10;
 	word = (char *) malloc (sizeof(char) * size);
  
 	// read in character-by-character until the newline is encountered
 	int count = 0;

 	while (ch != '\n'){
    	if (count+1 >= size){
       		// to grow an array to make it "dynamically sized" using malloc
       		char* temp;
       		int i;
       		size = size * 2;
       		temp = (char *) malloc (sizeof(char) * size);
       
       		// printf ("Growing array from size %d to size %d\n", count, size);
       		// copy the characters to the new array
       		for (i = 0 ; i < count ; i++)
           		temp[i] = word[i];

       		free (word);
       		word = temp;
      	}

    	word[count] = ch;
    	count++;
    	word[count] = '\0';

    	// read next character
    	ch = getc(stdin);
   	}

 	if (count > 30){
    	count = 30;
    	word[count] = '\0';
   	}
 
 	/* clear ending white space characters */
 	while (isspace (word[count-1])){
    	count--;
    	word[count] = '\0';
   	}

 	return word;
}


/* ------------------------------------------------------------- */
/* Clear input until next End of Line Character - \n */
void clearToEoln(){
  int ch;
 
	do {
     	ch = getc(stdin);
    }
 	while ((ch != '\n') && (ch != EOF));

}


/* ------------------------------------------------------------- */
/* Print out a list of the commands for this program */
void printCommands(){
 	printf ("The commands for this program are:\n\n");
 	printf ("q - to quit the program\n");
 	printf ("? - to list the accepted commands\n");
 	printf ("a <size> <name> - to add a group to the wait list\n");
 	printf ("c <size> <name> - to add a call-ahead group to the wait list\n");
 	printf ("w <name> - to specify a call-ahead group is now waiting in the restaurant\n");
 	printf ("r <table-size> - to retrieve the first waiting group that can fit at the available table size\n");
 	printf ("l <name> - list how many groups are ahead of the named group\n");
 	printf ("d - display the wait list information\n");
       
 	/* clear input to End of Line */
 	clearToEoln();
}


/* ------------------------------------------------------------- */
// Checks if name is in list
boolean doesNameExist(Node *root, char name[]){
	while(root != NULL){
		if( strcmp(root->Name, name) == 0 )
			return TRUE; // True if name already exists in list
		root = root->next;
	}
	return FALSE;
}


/* ------------------------------------------------------------- */
// Adds group to list
void addToList(Node **root, Node **end, char name[], int size, int status){
  int i;

	// If list is empty, create new node
	if(*root == NULL){
		*root = (Node*)malloc(sizeof(Node));
		*end = *root;
	}
	// Else, append to linked list
	else{
		(*end)->next = (Node*)malloc(sizeof(Node));
		*end = (*end)->next;
	}

	i = 0;
	// Copy new name into node element 'Name'
	while(name[i] != '\0'){
		(*end)->Name[i] = name[i];
		i++;
	}
	(*end)->Size = size;
	(*end)->Status = status;
	(*end)->next = NULL; // Tail->next always points to NULL
}


/* ------------------------------------------------------------- */
void doAdd (Node **root, Node **end){
  /* get group size from input */
  int size = getPosInt();
 	if (size < 1){
    	printf ("Error: Add command requires an integer value of at least 1\n");
    	printf ("Add command is of form: a <size> <name>\n");
    	printf ("  where: <size> is the size of the group making the reservation\n");
    	printf ("         <name> is the name of the group making the reservation\n");
    	return;
   	}

  	/* get group name from input */
  	char *name = getName();
 	if (NULL == name){
    	printf ("Error: Add command requires a name to be given\n");
    	printf ("Add command is of form: a <size> <name>\n");
    	printf ("  where: <size> is the size of the group making the reservation\n");
    	printf ("         <name> is the name of the group making the reservation\n");
    	return;
   	}

 	printf ("Adding group \"%s\" of size %d\n", name, size);

 	// add code to perform this operation here
	if( doesNameExist(*root, name) ){ // Adds group if group is not already in list
		printf("Error: Group name already exists\n");
	}
	else{
		int status = TRUE; // Means in restaurant
		addToList(root, end, name, size, status);
	}
}


/* ------------------------------------------------------------- */
void doCallAhead (Node **root, Node **end){
  	/* get group size from input */
  	int size = getPosInt();
 	if (size < 1){
    	printf ("Error: Call-ahead command requires an integer value of at least 1\n");
    	printf ("Call-ahead command is of form: c <size> <name>\n");
    	printf ("  where: <size> is the size of the group making the reservation\n");
    	printf ("         <name> is the name of the group making the reservation\n");
    	return;
   	}

 	/* get group name from input */
 	char *name = getName();
 	if (NULL == name){
    	printf ("Error: Call-ahead command requires a name to be given\n");
    	printf ("Call-ahead command is of form: c <size> <name>\n");
    	printf ("  where: <size> is the size of the group making the reservation\n");
    	printf ("         <name> is the name of the group making the reservation\n");
    	return;
   	}

 	printf ("Call-ahead group \"%s\" of size %d\n", name, size);

 	// add code to perform this operation here
	if( doesNameExist(*root, name) ){ // Adds group if group is not already in list
		printf("Error: Group name already exists\n");
	}
	else{
		int status = FALSE; // Means not in restaurant
		addToList(root, end, name, size, status);
	}
}


/* ------------------------------------------------------------- */
// Changes status to waiting in restaurant
boolean updateStatus(Node **root, char name[]){
  Node *p = *root;
	while(p != NULL){
		// If there is a match, strcmp will return 0
		if(strcmp(p->Name, name) == 0){ 
			if(p->Status == TRUE){
				printf("Error: Group '%s' is already waiting in restaurant\n", name);
				return FALSE;
			}
			p->Status = TRUE;
			return TRUE;
		}
		p = p->next;
	}
}


/* ------------------------------------------------------------- */
void doWaiting (Node **root){
  	/* get group name from input */
  	char *name = getName();
 	if (NULL == name){
    	printf ("Error: Waiting command requires a name to be given\n");
    	printf ("Waiting command is of form: w <name>\n");
    	printf ("  where: <name> is the name of the group that is now waiting\n");
    	return;
   	}

 	printf ("Waiting group \"%s\" is now in the restaurant\n", name);

 	// add code to perform this operation here
	if( doesNameExist(*root, name) )
		updateStatus(root, name);

}


/* ------------------------------------------------------------- */
// Remove group with size less than or equal to size of available table
char* retreiveAndRemove(Node **root, Node **end, char table_size){
  Node *p = *root;
  static char removedName[30];

	while(p != NULL){
		// If there's only one group in the list
		if( (*root == *end) && ((*root)->Status == 1) && ((*root)->Size <= table_size) ){
			strcpy( removedName, (*root)->Name );
			*root = NULL;
			*end = NULL;
			free(p);
			return removedName;
		}
		// If group is in the first node
		else if( ((*root)->Status == TRUE) && ( ((*root)->Size) <= table_size) ){
			strcpy( removedName, (*root)->Name );
			*root = (*root)->next;
			free(p);
			return removedName;
		}
		else if( (p->next->Status == TRUE) && (p->next->Size <= table_size) ){
			// If group is in the last node
			if(p->next == *end){			
				strcpy( removedName, (*end)->Name );
				*end = p;
				free(p->next);
				(*end)->next = NULL;
				return removedName;
			}
			// If the group is somewhere in the middle
			else{
				strcpy( removedName, p->next->Name );
				Node *tmp = p->next;
				p->next = tmp->next;
				free(tmp);
				return removedName;
			}
		}
		p = p->next;
		if(p->next == NULL){
			break;
		}
	}
	printf("Error: No available group can fit in that table\n");
	return NULL;
}


/* ------------------------------------------------------------- */
void doRetrieve (Node **root, Node **end){
  	/* get table size from input */
  	int size = getPosInt();
 	if (size < 1){
    	printf ("Error: Retrieve command requires an integer value of at least 1\n");
    	printf ("Retrieve command is of form: r <size>\n");
    	printf ("  where: <size> is the size of the group making the reservation\n");
    	return;
   	}
 	clearToEoln();
 	printf ("Retrieve (and remove) the first group that can fit at a tabel of size %d\n", size);

	char* removedName = retreiveAndRemove(root, end, size); 
 	// add code to perform this operation here
	if(removedName != NULL)
		printf("Removed group '%s'\n", removedName);
}


/* ------------------------------------------------------------- */
// Counts groups waiting ahead of a given group
int countGroupsAhead(Node *root, char name[]){
  int i = 1;
	while(root != NULL){
		if(strcmp(root->Name, name) == 0)
			return i-1; // Returns i-1 because i will be one value ahead
		root = root->next;
		i++;
	}	
}


/* ------------------------------------------------------------- */
// Prints sizes of groups waiting ahead of a given group
void displayGroupSizeAhead(Node *root, int count){
  int i;
	for(i=1; i<=count; i++){
		printf("Group %d:\n\tName: %s\n\tsize: %d\n", i, root->Name, root->Size);
		root = root->next;
	}
}


/* ------------------------------------------------------------- */
void doList (Node *root){
  	/* get group name from input */
  	char *name = getName();
 	if (NULL == name){
    	printf ("Error: List command requires a name to be given\n");
    	printf ("List command is of form: l <name>\n");
    	printf ("  where: <name> is the name of the group to inquire about\n");
    	return;
   	}

 	printf ("Group \"%s\" is behind the following groups\n", name);

 	// add code to perform this operation here
	int count = 0, size;
	if( doesNameExist(root, name) ){
		count = countGroupsAhead(root, name);
		displayGroupSizeAhead(root, count);
		printf("There is a total of %d group(s) ahead of group '%s'\n", count, name);
	}
	else
		printf("Error: Group '%s' is not in the list\n", name);
}


/* ------------------------------------------------------------- */
void displayListInformation(Node *root){
  int i = 1;
	while(root != NULL){
		printf("Group %d:\n\tName: %s\n\tSize: %d\n", i, root->Name, root->Size);
		if( root->Status == TRUE )
			printf("\tStatus: In-restaurant\n\n");
		else
			printf("\tStatus: Call-ahead\n\n");
		root = root->next;
		i++;
	}
	printf("There is a total of %d groups in the wait list\n", i-1);
}

 
/* ------------------------------------------------------------- */
void doDisplay (Node *root){
	clearToEoln();
 	printf ("Display information about all groups\n");

 	// add code to perform this operation here
	displayListInformation(root);
}


/* ------------------------------------------------------------- */
int main (int argc, char **argv){

  Node *root = NULL; // First node
  Node *end = NULL;  // Last node
  char *input;
  int ch, debug = FALSE;

	if(argc >= 2){
		if( strcmp(argv[1], "-d") == 0 ){
			debug = TRUE;
			printf("\nDebugging information\n\n");
		}
	}

 	printf ("Starting Restaurant Wait List Program\n\n");
 	printf ("Enter command (<?> for available commands): ");

 	while ((ch = getNextNWSChar ()) != EOF){
    	/* check for quit */
    	if ('q' == ch){
       		printf ("Quitting Program\n");
       		return (0);
      	}
    	else if ('?' == ch){ // Information
       		printCommands();
      	}
    	else if('a' == ch){ // Append new group as available in restaurant
       		doAdd(&root, &end);
      	} 
    	else if('c' == ch){ // Append new group as call-ahead reservation
       		doCallAhead(&root, &end);
      	} 
    	else if('w' == ch){ // Change group status to available in restaurant
       		doWaiting(&root);
      	} 
    	else if('r' == ch){ // Retrieve first group that can fit in available table
       		doRetrieve(&root, &end);
      	} 
    	else if('l' == ch){ // List all groups ahead of a given group name
       		doList(root);
      	} 
    	else if('d' == ch){ // Display entire waiting list
       		doDisplay(root);
      	} 
    	else{
       		printf ("%c - in not a valid command\n", ch);
       		printf ("For a list of valid commands, type ?\n");
       		clearToEoln();
      	}
		if(debug == TRUE)
			displayListInformation(root);
    	printf ("\nEnter command (<?> for available commands): ");
   	}

	printf ("Quiting Program - EOF reached\n");
  return 1;
}
