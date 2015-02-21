#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack{
	char *arr;
	int size;
	int top;
}Stack;


void stackInit(Stack *s){
	s->size = 2;
	s->top = -1;
	s->arr = (char*)malloc(sizeof(char)*s->size);	
} 


void printStack(Stack s){
  int i = 0;
	printf("   Stack: ");
	while(i <= s.top){
		printf("%c ", s.arr[i]);
		i++;
	}
	printf("\n");

}


void userInput(char input[]){
  int len;  // Later used to delete return character

	printf("Enter symbols to check. Q/q to quit: ");
	fgets(input, 300, stdin); // Gets user input from stdin

	// Now to get rid of the return character in the end
	len = strlen(input);	
	input[len-1] = '\0';

	// If input is q or Q, then exits program
	if( (strcmp(input, "q") == 0) || (strcmp(input, "Q") == 0 )){
		printf("Exiting program...\n"); 
		exit(0);
	}
}


void freeStack(Stack *s){
	free(s->arr);
	stackInit(s);
}


void grow(Stack *s, int debug){
  char *temp;
  int i;

	temp = (char*)malloc(sizeof(char)*(s->size)+2);
	i = 0;
	while(i < s->size){
		temp[i] = s->arr[i];
		i++;
	}
	free(s->arr);
	s->arr = temp;
	s->size += 2;

	// For debugging
	if(debug == 1){
		printf("SIZE increased from %d to %d\n", s->size-2, s->size);
		printf("Number of values copied = %d\n", i);
	}
}


void push(Stack* s, char newChar, int debug){
	
	if(s->top == s->size-1) {
		grow(s, debug);
	}
	s->top = s->top + 1;
	s->arr[s->top] = newChar;
	if(debug == 1){
		printf("Push %c\n", newChar);
		printStack(*s);
	}
}


int pop(Stack s, int debug){
	if(s.top == -1)
		return s.top;
	s.top = s.top - 1;

	if(debug == 1){
		printf("Pop %c\n", s.arr[s.top+1]);
		printStack(s);
	}
	
	return s.top;
}


// If stack has a missing open symbol, it returns it to print in main
char getMissingChar(char c){
	if(c == ')')
		return '(';
	if(c == '}')
		return '{';
	if(c == ']')
		return '[';
	if(c == '>')
		return '<';
}


// Returns the missing ending symbol
char neededMatch(char c){
	if(c == '(')
		return ')';
	if(c == '{')
		return '}';
	if(c == '[')
		return ']';
	if(c == '<')
		return '>';
}


// Checks if the stack is empty
int checkIfStackEmpty(Stack s){
	if(s.top >= 0){
		printf("Stack not empty\n");
		return 1;
	}
	else{
		printf("Stack is empty\n");
		return 0;
	}
}


// Calls push and pop functions
int compareSymbols(Stack *s, char input[], char *missingChar, int debug){
  int i = 0; // Counter
  int mistakeIndex = 0; // Stores index of symbol that's missing an open
  char c; // Stores the symbol itself that's missing an open

	// Loop to check the character being read 
	while(input[i] != '\0'){
		// If open symbol, then push to the stack
		if( input[i] == '(' || input[i] == '{' || 
			input[i] == '[' || input[i] == '<'  ) 
			push(s, input[i], debug); // Push to the stack
		else if( input[i] == ')' || input[i] == '}' || 
				 input[i] == ']' || input[i] == '>'  ){
			// If matching symbols, then pop from the stack
			if( (s->arr[s->top] == '(' && input[i] == ')') ||
				(s->arr[s->top] == '{' && input[i] == '}') ||
				(s->arr[s->top] == '[' && input[i] == ']') ||
				(s->arr[s->top] == '<' && input[i] == '>')  ){
				s->top = pop(*s, debug); // Pop the top element
			}
			else{
				mistakeIndex = i; // Stores index of symbol that's missing an open
				c = input[i]; // Stores the symbol itself that's missing an open
				*missingChar = getMissingChar(c); // Gets the symbol that was supposed to match c
				s->top = pop(*s, debug); //
				break; // Breaks as soon as a mismatch was found
			}
		}
		i++;
	}
	return mistakeIndex;
}


int main(int argc, char* argv[]){
  
  Stack s; // Instance of stack struct
  char input[300]; // Takes input from stdin
  char missingChar, ch; // Store the characters that were missing
  int i, j, k, x; // Loop counters
  int index, debug = 0, missingEnd; // Boolians 
  int size; // Stores the size before stack is deleted

	stackInit(&s);
	
	// Command line argument test
	// If true, then this program will run in debugger mode
	if(argc >= 2){
		if(strcmp(argv[1], "-d") == 0){
			debug = 1;
			printf("Running in debugger mode\n\n");
		}
	}

	while(1){
		userInput(input);
		index = 0;
		missingEnd = 0;
		// Returns index of incorrect matching symbols or 0
		index = compareSymbols(&s, input, &missingChar, debug);
		missingEnd = checkIfStackEmpty(s);
		ch = neededMatch(s.arr[0]); // The gets missing match
		size = s.size; // Stores size before stack is deleted
		freeStack(&s); // Empty the stack
		j=0;
		while(input[j] != '\0'){
			printf(" %c", input[j]);
			j++;
		}
		printf("\n");
		
		if(index == 0 && missingEnd == 0)
			printf("Expression is balanced\n\n"); 		

		// If close symbol is missing an open symbol
		if(index != 0){
			k=0;
			while(input[k] != '\0'){
				if(k == index)
					printf(" ^ missing %c", missingChar);
				else
					printf("  ");
				k++;
			}
			printf("\n");
		}
		// If open symbol is missing a close symbol
		else if(missingEnd != 0){
			x = 0;
			while(input[x] != '\0'){
				printf("  ");
				x++;
			}
			printf(" ^ expecting %c\n", ch);
		}
	}


  return 0;
} 

