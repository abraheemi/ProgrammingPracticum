#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For usleep()

//ANSI escape codes to display output in color
#define RESET "\033[0m" /* Must be included after each color */
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

/* Example:
 printf(RED "This text is in red" RESET); 
*/

typedef enum {FALSE = 0, TRUE, UNVISITED = 0, VISITED} boolean;


typedef struct mazeStruct
{
 //char arr[32][32];  /* allows for a maze of size 30x30 plus outer walls */
 char **arr;
 int xsize, ysize;
 int xstart, ystart;
 int xend, yend;
} maze;


typedef struct mazeStack{
  int row;
  int col;
  struct mazeStack *next;
} Stack;


/*************************************************************************/
// Prints [rows, cols] in stack (the recursive function is used instead of this one)
void printStack(Stack* path){
	while(path != NULL){
		printf("[%d, %d]", path->row, path->col);
		if(path->next != NULL)
			printf("<-");
		path = path->next;
	}
	printf("\n");
}


/*************************************************************************/
// Prints stack in reverse using recursion
void printStackRecursion(Stack* path){
	if(path == NULL)
		return;
	printStackRecursion(path->next);
	printf("[%d, %d]->", path->row, path->col);
}


/*************************************************************************/
// Prints maze board
void printMaze(maze m1){
  int i, j;
  for (i = 0; i < m1.xsize+2; i++){
     for (j = 0; j < m1.ysize+2; j++)
		// Print in color
		if(m1.arr[i][j] == 's' || m1.arr[i][j] == 'e')
        	printf ( GREEN "%c" RESET, m1.arr[i][j]);
      	else if(m1.arr[i][j] == 'V')
        	printf ( YELLOW "%c" RESET, m1.arr[i][j]);
      	else if(m1.arr[i][j] == '*' || m1.arr[i][j] == 'X')
        	printf ( RED "%c" RESET, m1.arr[i][j]);
      	else if(m1.arr[i][j] == '.')
        	printf ( BOLDBLACK "%c" RESET, m1.arr[i][j]);
      	else
        	printf ("%c", m1.arr[i][j]);

     printf("\n");
    }
}


/*************************************************************************/
void pop(Stack** path, maze *m1){
	Stack *tmp = *path;
	m1->arr[(*path)->row][(*path)->col] = 'X';
	*path = (*path)->next;
	free(tmp);
}


/*************************************************************************/
void push(Stack** path, int newRow, int newCol){
  Stack *tmp = (Stack*)malloc(sizeof(Stack));
	tmp->row = newRow;
	tmp->col = newCol;
	tmp->next = *path;
	*path = tmp;
}


/*************************************************************************/
// Change direction
void changeDirection(int *n, int *i, int *j){
	if(*n >= 5){
		*i = 0; // Row offset 0
		*j = 1; // Col offset 1
		*n = 1; // Resets direction	
	}
	else if(*n == 1){ // Right
		*i = 0; *j = 1;
	}
	else if(*n == 2){ // Down
		*i = 1; *j = 0;
	}
	else if(*n == 3){ // Left
		*i = 0; *j = -1;
	}
	else if(*n == 4){ // Up
		*i = -1; *j = 0;
	}
}


/*************************************************************************/
void stackOperations(maze *m1, boolean debug){
  Stack *path = NULL;
  int i=0, j=1, k, n=1;
	
	push(&path, m1->xstart, m1->ystart);
	m1->arr[path->row][path->col] = 'V';

	while(path != NULL && (path->row != m1->xend || path->col != m1->yend) ){

		if(path->row == m1->xend && path->col == m1->yend){
			printf("End was found\n");
			exit(0);
		}
		for(k=0; k<4; k++){
			// If next spot is an empty spot
			if(m1->arr[path->row+i][path->col+j] == '.' || m1->arr[path->row+i][path->col+j] == 'e'){
				// Push the index position and mark it as V
				push(&path, path->row+i, path->col+j);
				m1->arr[path->row][path->col] = 'V';
				break; // Break out of for loop
			}
			// else change direction
			n++;
			changeDirection(&n, &i, &j);
		}
		if(k == 4){ // If no empty unvisited spot was found
			// Chack the visited spots
			for(k=0; k<4; k++){
				if(m1->arr[path->row+i][path->col+j] == 'V'){ //&& path.arr[row+i][col+j] != '*'){
					pop(&path, m1);
					break; // Break out of for loop
				}
				// else change direction
				n++;
				changeDirection(&n, &i, &j);
			}
			if( path->row == m1->xstart && path->col == m1->ystart && 
				m1->arr[path->row][path->col] == 'V'){

				pop(&path, m1);
			}
		}
		if(debug == TRUE){
			printMaze(*m1); // Prints the maze board
			printStackRecursion(path); // Prints the coordinates stored in the stack
			printf("NULL\n"); 
			usleep(90000); // Makes program wait before displaying output
			system("clear"); // Clears the screen after each output
		}
	}

	printMaze(*m1);
	if(path == NULL){
		printf("\nThe maze has no solutions\n");
	}
	else{
		printf("\nStack contains solution path d(^_^)b\n\n");
		printf("Path taken: ");
		printStackRecursion(path);
		printf("NULL\n");
	}
}


/*************************************************************************/
int main (int argc, char **argv)
{
  maze m1;

  int xpos, ypos;
  int i,j;
  char* temp;
  boolean debug = FALSE;

  FILE *src;

  /* verify the proper number of command line arguments were given */
  if(argc < 2) {
     printf("Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }
  if(argc >= 3){
	/*if( (strcmp(argv[1], "mazeInput.txt") == 0 && strcmp(argv[2], "-d") == 0) || 
		(strcmp(argv[2], "mazeInput.txt") == 0 && strcmp(argv[1], "-d") == 0) ){
		printf("Debugging Information\n");
		debug = TRUE;
	}*/
	if(strcmp(argv[1], "-d") == 0){
		argv[1] = argv[2]; // Saves us the trouble later on
		printf("Debugging Information\n");
		debug = TRUE;
	}
	else if( (strcmp(argv[2], "-d") == 0)){
		printf("Debugging Information\n");
		debug = TRUE;
	}
	else{
		printf("Error: Invalid arguments\nExiting program...\n");
		exit(-1);
	}
  }
   
  /* Try to open the input file. */
  if ( ( src = fopen( argv[1], "r" )) == NULL )
  {
    printf ( "Can't open input file: %s", argv[1] );
    exit(-1);
  }

  /* read in the size, starting and ending positions in the maze */
  while(1){ // Checks for size
	if(fscanf (src, "%d %d", &m1.xsize, &m1.ysize) == EOF){
		fprintf(stderr, "End of file reached. Exiting...\n");
		exit(-1);
	}
  	if(m1.xsize <= 0 || m1.ysize <= 0){
    	fprintf(stderr, "Error: incorrect size\n");
		continue;
	}
	break;
  }
  while(1){ // Checks for start position
	if(fscanf (src, "%d %d", &m1.xstart, &m1.ystart) == EOF){
		fprintf(stderr, "End of file reached. Exiting...\n");
		exit(-1);
	}
  	if( (m1.xstart < 1 || m1.xstart > m1.xsize) ){
    	fprintf(stderr, "Error: incorrect xstart position\n");
		continue;
	}
	if( (m1.ystart < 1 || m1.ystart > m1.ysize) ){
    	fprintf(stderr, "Error: incorrect ystart position\n");
		continue;
	}
	break;
  }
  while(1){ // Checks for end position
	if(fscanf (src, "%d %d", &m1.xend, &m1.yend) == EOF){
		fprintf(stderr, "End of file reached. Exiting...\n");
		exit(-1);
	}
  	if( (m1.xend < 1 || m1.xend > m1.xsize) ){
    	fprintf(stderr, "Error: incorrect xend position\n");
		continue;
	}
	if( (m1.yend < 1 || m1.yend > m1.ysize)){
    	fprintf(stderr, "Error: incorrect yend position\n");
		continue;
	}
	break;
  }

  /* print them out to verify the input */
  printf ("size: %d, %d\n", m1.xsize, m1.ysize);
  printf ("start: %d, %d\n", m1.xstart, m1.ystart);
  printf ("end: %d, %d\n", m1.xend, m1.yend);

int r, c;
r = m1.xsize + 2;
c = m1.ysize + 2; 
// For some reason, the code below does not deallocate in the end
// when I have m1.xsize+2 and m1.ysize+2. 
// That is why I assign them to the variables r and c
  m1.arr = (char **) malloc (sizeof(char *) * r);
  for (i = 0; i < r; i++)
      m1.arr[i] = (char *) malloc (sizeof(char ) * c);
  
  /* initialize the maze to empty */
  for (i = 0; i < m1.xsize+2; i++)
     for (j = 0; j < m1.ysize+2; j++)
       m1.arr[i][j] = '.';

  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.arr[i][0] = '*';
     m1.arr[i][m1.ysize+1] = '*';
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.arr[0][i] = '*';
     m1.arr[m1.xsize+1][i] = '*';
    }

  /* mark the starting and ending positions in the maze */
  m1.arr[m1.xstart][m1.ystart] = 's';
  m1.arr[m1.xend][m1.yend] = 'e';
		  
  /* mark the blocked positions in the maze with *'s */
  while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
	 if(xpos>m1.xsize || xpos<1 ){
		fprintf(stderr, "Invalid: row %d is outside range\n", xpos);
		continue;
	 }
	 if(ypos>m1.ysize || ypos<1){
		fprintf(stderr, "Invalid: column %d is outside range\n", ypos);
		continue;
	 }
	 if(xpos == m1.xstart && ypos == m1.ystart){
		fprintf(stderr, "Invalid: row/column [%d, %d] is the same as start position\n", xpos, ypos);
		continue;
	 }
     m1.arr[xpos][ypos] = '*';
    }

  /* print out the initial maze */
  for (i = 0; i < m1.xsize+2; i++){
     for (j = 0; j < m1.ysize+2; j++){
		if(m1.arr[i][j] == 's' || m1.arr[i][j] == 'e')
        	printf ( GREEN "%c" RESET, m1.arr[i][j]);
      	else if(m1.arr[i][j] == '*')
        	printf ( RED "%c" RESET, m1.arr[i][j]);
      	else if(m1.arr[i][j] == '.')
        	printf ( BOLDBLACK "%c" RESET, m1.arr[i][j]);
      	else
        	printf ("%c", m1.arr[i][j]);
	 }

     printf("\n");
    }
  stackOperations(&m1, debug);

  for(i=0; i < m1.xsize + 2; i++){
	free(m1.arr[i]);
  }
  free(m1.arr);
  printf("Memory freed\n");

return 0;
}

