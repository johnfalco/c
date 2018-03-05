/* Lab 6 for CSIS 381
**
** @author			John Falco
** @id			  	jpfalco
** @course			CSIS 381
** @assignment		Add positional arguments to xref.c
**					0 Arguments: read from stdin, write to stdio
**					1 Arguments: read from file, write to stdio
**					2 Arguments: read from 1st file, write to 2nd file
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define FALSE      0
#define TRUE      1
#define MAXWORD  100000
#define BUFSIZE 100000

int currentLine = 1;
int bufp = 0;
char buf[BUFSIZE];

static FILE *in = NULL;
static FILE *out = NULL;

/* Node structure, used for tree
*/
struct node {
/* Holds the word itself */
  	char *word;
/* Holds the position of the word */
  	int position;
/* Holds the left descendant node */
  	struct node *l;
/* Holds the right descendant node */
  	struct node *r;
};

/* Determine if writing to file, then print accordingly
** @param str Pointer to the string being printed
** @param line Line number being printed
** @param word Word being printed to the string 
*/
void printstr(const char *str, int line, const char *word) {
/*
	printf("write");
*/
	if (out == NULL) {
/*		
		printf("console");
*/
		printf(str, line, word);
	} else {		
/*	
		printf("file");
*/
		fprintf(out, str, line, word);
	}
}

/* Determine if reading from a file, then read accordingly
** @return The read data
*/
int readchar() {
	int c;
	if (in == NULL) {
		c = getchar();
/*		
		printf("%d\n", c); 
*/
		return c;
	} else {
		c = fgetc(in);
/*		
		printf("%d\n", c); 
*/
		return c;
	}
}

/* Detects if a character is a space character, excluding the \n character
** @param i the character being checked
** @return TRUE if it's a space character (excluding \n)
*/
char isnonlinespace( char i ){
    if ( i == ' '
      || i == '\f'
      || i == '\r'
      || i == '\t'
      || i == '\v') {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Gets the next word or character from input
** @param word Pointer to the word being gotten
** @param lim Limits the size of the word
** @return The word's first character for sorting
*/
int getWord(char *word, int lim) {
    int c;
    int getch(void);
    void ungetch(int);
    char *wrd = word;
    while(isnonlinespace( c = getch()))
          ;
      if(c != EOF){
          *wrd++ = c;
      }
      
      if(c == '\n'){
          ++currentLine;
      }
      
      if(!isalpha(c)){
          *wrd = '\0';
          return c;
      }
      for(; --lim > 0; wrd++) {
          if(!isalnum(*wrd = getch())){
              ungetch(*wrd);
              break; }
      }
      *wrd = '\0';
      return word[0];
}

/* Get data from the buffer
** @return The next data from the buffer
*/
int getch(void) {
		return (bufp > 0) ? buf[--bufp] : readchar();
}

/* Puts data back onto the buffer
** @param c Data put back onto the buffer
*/
void ungetch(int c) {
    if(bufp >= BUFSIZE)
        printf("ERROR: Too much data to ungetch\n");
    else
        buf[bufp++] = c;
}

/* Determines if the word is a reserved data type word
** @param word Pointer for the word being referenced
** @return TRUE if the word is a data type
*/
int dataType( char *wrd ){
    char *dTypes[] = { "char", "double",
                       "float", "int",
                       "long", "short", };
    int loc = TRUE;
    int stop  = 0;
    int low = 0;
    int diff  = 0;
    int high  = sizeof dTypes / sizeof dTypes[0] - 1;
    int where = ( high / 2 );
    if(wrd != NULL){
        while(loc && !stop){
            diff = strcmp(dTypes[where], wrd);
            if( 0 == diff) {
                loc = FALSE;
            }
            if( 0 < diff ) {
                high = where - 1;
            } else {
                low = where + 1;
            }
            if( high <= ( low - 1 )) {
                stop = 1;
            } else {
                where = (( high + low ) / 2);
            }
        }
    }
    return loc;
}

/* Makes sure that the string is a valid C Identifier
** @param str String being checked
** @return TRUE is it is a valid C identifier
*/
int validIdentifier( char *str ){
    if(isdigit(str[0])){
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Creates and allocates the memory for a new tree node
** @return The created tree node
*/
struct node *nodeallocate(void) {
    return (struct node *) malloc(sizeof(struct node));
}

/* Create or add a tree node in relation to the given tree node
** @param addNode Pointer to the current tree node being calculated from
** @param word Pointer to the word being added
*/
struct node *addtree( struct node *addNode, char *word ){
    int conditional;
    if(addNode == NULL){
        addNode = nodeallocate();
        addNode -> word = strdup(word);
        addNode -> position = currentLine;
        addNode -> l = addNode -> r = NULL;
    } else if ((conditional = strcmp(word,  addNode -> word )) < 0 ){
        addNode -> l  = addtree(addNode -> l, word);
    } else {
        addNode -> r = addtree(addNode -> r, word);
    }
    return addNode;
}

/* Prints the tree using inorder traversal
** @param add Pointer to the root tree node being traversed from
*/
void printTree( struct node *n ){
	if(n != NULL){
		printTree( n -> l  );
		printstr("  %4d     %s\n", n -> position, n -> word);
		printTree( n -> r );
	}
}

/* Prints all data from the node
** @param n Pointer to the node being printed from
*/
void printNodeData( struct node *n ){
	printstr("   Line / Identifier\n", 0, NULL);
	printTree(n);
}

/* Handles opening of files 
** @param filePath Pointer to the filepath being opened
** @param reading TRUE if reading, otherwise writing
** @return The file accessed
*/
FILE *openFile(const char *filePath, int reading) {
	FILE *file;
	
	if (reading == TRUE) {
		file = fopen(filePath, "r");
	} else {
		file = fopen(filePath, "w");
	}
	
	/* If there's a problem, abort mission */
    if(file == NULL) {
        printf("ERROR: File had an issue!");
		exit(0);
	}
    return(file);
}

/* Runs the program
** @param argc Holds the number of arguments
** @param argv Pointer to the array of args
*/ 
int main(int argc, char *argv[]) {
	if(argc <= 1){
        /* Function normally */
	}else if( argc <= 2 ){
		if(strcmp(argv[1], "-i")== 0 ){
			printf("ERROR: -i needs file name");
		} else if (strcmp(argv[1], "-o") == 0){
			printf("ERROR: -o needs a file name");
		} else {
			printf("Bad Input: incorrect input\n");
		}
	}else if( argc <= 3 ){
		if(strcmp(argv[1], "-i") == 0){
			in = openFile(argv[2], TRUE);
		}else if(strcmp(argv[1], "-o") == 0){
			out = openFile(argv[2], FALSE);
		}
	}else if(argc <= 4){
		printf("ERROR: Missing arguments or too many arguments");
	}else if(argc <= 5){
		if(strcmp(argv[1], "-i") == 0){
			if(strcmp(argv[3], "-o") == 0){
                in = openFile(argv[2], TRUE);
                out = openFile(argv[4], FALSE);
			}
		}else if(strcmp(argv[1], "-o") ==0){
			if(strcmp(argv[3], "-i") == 0){
                out = openFile(argv[2], FALSE);
                in = openFile(argv[4], TRUE);
			}
		}
	}else if(argc > 5){
		printf("ERROR: Too many args");
        exit(0);
    }

	struct node *root;
    char word[MAXWORD];
    root = NULL;
    while( getWord(word, MAXWORD) != EOF ){
        if(dataType(word) == FALSE){
            if( getWord(word, MAXWORD) != EOF ) {
                if( isalpha(word[0])) {
                    root = addtree( root, word );
                }
            }
        }
    }
    printNodeData(root);
	fclose(in);
	fclose(out);
    exit(0);
}