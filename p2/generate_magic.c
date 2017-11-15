////////////////////////////////////////////////////////////////////////////////
// Main File:        generate_magic.c
// This File:        generate_magic.c
// Other Files:      verify_magic.c
// Semester:         CS 354 Spring 2017
//
// Author:           Sean Kim
// Email:            skim658@wisc.edu
// CS Login:         seank
//
/////////// IF PAIR PROGRAMMING IS ALLOWED, COMPLETE THIS SECTION //////////////
//
// Pair Partner:     (name of your pair programming partner)
// Email:            (email address of your programming partner)
// CS Login:         (partner's CS login name)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

int get_square_size();
Square * generate_magic(int size);
void write_to_file(Square * square, char *filename);

int main(int argc, char *argv[])
{       
	// Check input arguments to get filename
	char *fileName = argv[1];

	if ( argv[1] == NULL ) {
		printf( "%s\n", "Please enter output file" );
		exit( 1 );
	}

	if ( argc > 2 ) {
		printf( "%s\n", "Too many input variables" );
		exit( 1 );
	}

	// Get size from user
	int size = get_square_size();	

	// Generate the magic square
	struct _Square *square = generate_magic( size );

	// Write the square to the output file
	write_to_file( square, fileName );
	
	// Free up dynamically allocated 2D array
	int **squareArray = square->array;
	for ( int i = 0; i < size; i++ ) {
		free( *( squareArray + i ) );
	}

	return 0;
}

/* get_square_size prompts the user for the magic square size
 * checks if it is an odd number >= 3 and returns the number
 */
int get_square_size()
{
	// Holds the square size
	int size = 0;

	// Prompts user to enter magic square size 
	printf( "%s\n ", "Enter size of magic square, must be odd"  );
	// Stores user input in size 
	scanf( "%d", &size );
	// Check if scanf returned a negative number meaning it failed
	if ( size <= 0 ) {
		printf( "%s\n", "Failed to read input" );
		exit( 1 );
	}

	// Keep prompting user for input as long as size is less than 3 or an even number
	while ( size < 3 || size % 2 == 0 ) {
		printf("%s\n ", "Size must be an odd number >=3.");
		size = 0;
		scanf( "%d", &size );
	}	
	
	// Return square size
	return size;

}

/* generate_magic constructs a magic square of size n
 * using the Siamese algorithm and returns the Square struct
 */
Square * generate_magic(int n)
{
	// 2D array that holds the square as it is generated
	int **squareArray;

	// Initialize 2D array to size of n and check malloc return value
	squareArray = malloc( sizeof( int ) * n );
	if ( squareArray == NULL ) {
		printf( "%s\n", "Cannot allocate memory" );
		exit( 1 );
	}
	for ( int i = 0; i < n; i++ ) {
		*( squareArray + i ) = malloc( sizeof( int ) * n );
	}
	 
	// Place a 1 at the center column of the topmost row
	*( *( squareArray + 0 ) + ( ( ( n + 1 ) / 2 ) - 1 ) ) = 1;

	// Total number of squares to be filled not including the 1 spot
	int total = ( n * n ) - 1;

	// currNumber will store next number to be filled in magic square
	int currNumber = 2; 

	// Keeps track of current position in the square
	int currRow = 0;
	int currCol = ( ( n + 1 ) / 2 ) - 1 ;	  

	// Populate the squareArray using siamese method
	for ( int i = 0; i < total; i++ ) {

		// Check if one row up is outside the square and one column to the right is inside the square
		if ( currRow - 1 < 0 && currCol + 1 < n ) {
			if ( *( *( squareArray + ( n - 1 ) ) + ( currCol + 1 ) ) != 0 )  {
				currRow = currRow + 1;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
			else {
				currRow = n - 1;
				currCol = currCol + 1;
				*( *( squareArray + currRow ) + currCol ) = currNumber; 
			} 				
		}
		// Check if one row up and one column to right are both inside the square 
		else if ( currRow - 1 < n && currRow - 1 >= 0 && currCol + 1 < n ) {
			if ( *( *( squareArray + ( currRow - 1 ) ) + ( currCol + 1 ) ) != 0 ) {
				currRow = currRow + 1;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
			else {
				currRow = currRow - 1;
				currCol = currCol + 1;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
		}
		// Check if one row up is inside the square and one column to the right is outside the square
		else if ( currRow - 1 >= 0 && currCol + 1 >= n ) {
			if ( *( *( squareArray + ( currRow - 1 ) ) + 0 ) != 0 ) {	
				currRow = currRow + 1;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
			else { 
				currRow = currRow - 1;
				currCol = 0;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
		}
		// Check is one row up and one column to right are both outside the square 
		else if ( currRow - 1 < 0 && currCol + 1 >= n ) {
			if ( *( *( squareArray + ( n - 1 ) ) + 0 ) != 0 ) {
				currRow = currRow + 1; 
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
			else {
				currRow = n - 1;
				currCol = 0;
				*( *( squareArray + currRow ) + currCol ) = currNumber;
			}
		}
  
		// Increment the next number to be put into the square
		currNumber = currNumber + 1;
 	
	}

	// Create a square structure pointer to return
	Square square = { n, squareArray };
	struct _Square *returnPointer;
	returnPointer = &square;

	return returnPointer;
}

/* write_to_file opens up a new file(or overwrites the existing file)
 * and writes out the square in the format expected by verify_magic.c
 */
void write_to_file(Square * square, char *filename)
{
	int commaCount = 0;
	int size = square->size;
	int **squareArray = square->array;

	// Create file
	FILE *fp;
	fp = fopen( filename, "w+" );
	if ( fp == NULL ) {
		printf( "%s\n", "Cannot open output file" );
		exit( 1 );
	}

	// Write the square size and square to the file 
	fprintf( fp, "%d\n", size );
	for ( int a = 0; a < size; a++ ) {
		for ( int b = 0; b < size; b++ ) {
			if ( commaCount < size - 1 ) {
				fprintf( fp, "%d%s", *( *( squareArray + a ) + b ), ", " ) ;
				commaCount = commaCount + 1;
			}
			else {
				fprintf( fp, "%d", *( *( squareArray + a ) + b ) );
				commaCount = 0;
			}
		}
		fprintf( fp, "\n" );
	}
	
	// CLose the file 
	fclose( fp );
}
