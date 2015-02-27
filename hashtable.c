/***********************************
* Author: Abraheem Irheem
*
* Class: CS 211, FALL 2014
*
* System: g++ on Ubuntu 64bit
*
* Program: Hash Table
*
* 
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;

typedef struct node{
	int val;
	struct node *next;
} Node;


void clearToEoln(){
  int ch;

    do {
        ch = getc(stdin);
    }
    while ((ch != '\n') && (ch != EOF));

}


void printCommands(){
	printf("The commands for this program are:\n\n");
	printf("q - to quit the program\n");
	printf("? - to list the accepted commands\n");
	printf("i <int> - to insert the integer value into hash table\n");
	printf("d <int> - to delete the integer value from the hash table\n");
	printf("c <int> - to check whether the given integer value is in the hash table\n");
	printf("r <int> - to resize the hash table\n");
	printf("l - to list all elements contained in the hash table\n");
	printf("e - to erase all elements currently in the hash table\n");

	clearToEoln();
}


void printHashTable(Node *h, int i){
	if(h == NULL)
		return;
	else{
		printf("hash[%d] -> ", i);
		while(h != NULL){
			printf("%d ", h->val);
			h = h->next;
		}
		printf("\n");
	}
}


// Prints all items contained in the hash table
void list(Node** hash, int size){
  int i;
  Node **h = hash;
	printf("\n");
	for(i=0; i<size; i++)
		printHashTable(h[i], i);
	printf("\n");
}


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


// Checks whether a given value exists in the hash table
boolean valueExists(Node* hash, int value){
	while(hash != NULL){
		if((hash)->val == value)
			return TRUE; // Value exists
		hash = (hash)->next;
	}
	return FALSE; // Value does not exist
}


// In here we check if the index in the hash table is NULL or not (if the pointer is pointing
// to NULL or if its pointing to an existing linked list)
boolean insert(Node* *hash, int newVal){
    if (newVal < 0){
        printf ("Error: Insert command requires an integer value of at least 0\n");
        printf ("Insert command is of form: i <int>\n");
        printf ("  where: <int> is the value you want to insert\n");
        return FALSE;
    }

	if(valueExists(*hash, newVal)){
		printf("\nError: Value already exists\n");
	}
	else{ // Value is not already in hash table
		if(*hash == NULL){ // If its the first value
			*hash = (Node*)malloc(sizeof(Node));
			(*hash)->val = newVal;
			(*hash)->next = NULL;
		}
		else{
			Node *temp = (Node*)malloc(sizeof(Node));
			temp->val = newVal;
			temp->next = *hash;
			*hash = temp;
		}
		return TRUE;
	}
	return FALSE;
}


// Deletes a given value from the hash table
boolean delete(Node* *hash, int size, int value){
	if(!valueExists(*hash, value))
		printf("\nError: value %d is not in the hash table\n", value);
	else{
		Node *p = *hash;
		// If removing from head of list
		if((*hash)->val == value){ 
			*hash = (*hash)->next;
			free(p);
		}
		else{
			// Traverse the list
			while(p->next->val != value){
				p = p->next;
			}
			Node *temp = p->next;
			p->next = p->next->next;
			free(temp);
		}
		return TRUE;
	}
	return FALSE;
}


// Prints message to show whether a given value is in the hash table or not
boolean contained(Node* hash, int value){

	if(valueExists(hash, value)){
		printf("\nValue %d exists in the hash table\n", value);
		return TRUE;
	}
	else
		printf("\nValue %d does not exist in the hash table\n", value);
	return FALSE;
}


// Deletes all values but size stays the same. 
// This will not delete the array of pointers
void erase(Node** *hash, int size){
  int i;
	for(i=0; i<size; i++){
		while((*hash)[i] != NULL){
			Node *p = (*hash)[i]->next;
			free((*hash)[i]);
			(*hash)[i] = p;
		}
	}
}


// Creates new hash table with new size and deletes the old one
void resize(Node** *hash, int *size, int newSize){
  Node **tempHash = (Node**)malloc(sizeof(Node*)*newSize);
  int i, hashIndex;
	// Initialize temp head pointers to NULL
	for(i=0; i<newSize; i++){
		tempHash[i] = NULL;
	}
	for(i=0; i<(*size); i++){
		Node *p = (*hash)[i];
		while(p != NULL){
			hashIndex = (p->val) % newSize;
			insert(&tempHash[hashIndex], p->val);
			p = p->next;
		}
	}
	// Delete old hash table
	erase(hash, *size); // Deletes all nodes from old hash table
	free(*hash);  // Deletes the old array of pointers

	*size = newSize;  // Update size
	*hash = tempHash; // Update hash table
}


int main(int argc, char* argv[]){
  Node **hash;
  int ch, i, size=8;
  int hashIndex, newVal, value, newSize;
  boolean debug = FALSE;

	if(argc >= 2){
		if(strcmp(argv[1], "-d") == 0){
			debug = TRUE;
			printf("Debugging information\n");
		}	
	}
	// Create initial hash table
	hash = (Node**)malloc(sizeof(Node*)*size);
	for(i=0; i<size; i++){
		// We don't allocate a node for each pointer yet until
		// we need to insert a new value, but we have to initialize
		// each pointer to NULL
		hash[i] = NULL;
	}
  int check;
printf ("Enter command (<?> for available commands): ");

    while ((ch = getNextNWSChar ()) != EOF){
        // check for quit 
        if ('q' == ch){
            printf ("Quitting Program\n");
            return (0);
        }
        else if ('?' == ch){
            printCommands();
        }   
        else if('i' == ch){
  			newVal = getPosInt();
			hashIndex = newVal%size;
            check = insert(&hash[hashIndex], newVal);
			if(debug && check)
				printf("Inserted %d into hash index %d", newVal, hashIndex);
        }   
        else if('d' == ch){
  			value = getPosInt();
			hashIndex = value%size;
			check = delete(&hash[hashIndex], size, value);
			if(debug && check)
				printf("\nDeleted %d from hash index %d", value, hashIndex);
        }   
        else if('c' == ch){
  			value = getPosInt();
			hashIndex = value%size;
			check = contained(hash[hashIndex], value);
			if(debug && check)
				printf("Value %d is in hash index %d", value, hashIndex);
        }   
        else if('r' == ch){
  			newSize = getPosInt();
			resize(&hash, &size, newSize);
			if(debug && TRUE)
				printf("\nHash table resized to %d\n", size);
        }
        else if('l' == ch){
			printf("\nDisplaying current values in hash table\n");
			list(hash, size);
        }
        else if('e' == ch){
			erase(&hash, size);
			if(debug == TRUE)
				printf("\nAll values have been erased from hash table\n");
        }
        else{
            printf ("%c - in not a valid command\n", ch);
            printf ("For a list of valid commands, type ?\n");
            clearToEoln();
        }
		if(debug == TRUE){
			list(hash, size);
		}
		printf ("\nEnter command (<?> for available commands): ");
	}
	printf ("Quiting Program - EOF reached\n");
	return 0;
}

