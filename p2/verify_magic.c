////////////////////////////////////////////////////////////////////////////////
// Main File:        verify_magic.c
// This File:        verify_magic.c
// Other Files:      generate_magic.c
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
#include <string.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

Square * construct_square(char *filename);
int verify_magic(Square * square);

int main(int argc, char *argv[])
{	
	// Check input arguments to get filename
	char *fileName = argv[1];
	if( argv[1] == NULL ) {
		printf ("%s\n", "Cannot open input file" );
		exit( 1 );
	}

	// Make sure user entered the right amount of input files
	if ( argc > 2 || argc == 1 ) {
		printf( "%s\n", "Please enter one input file" );
		exit( 1 );
	}

	// Construct square
	struct _Square *square = construct_square( fileName );

	// Verify if it's a magic square and print true or false
	if ( verify_magic( square ) == 1 ) {
		printf( "%s\n", "true" );
	}
	else {
		printf( "%s\n", "false" );
	}

	int size = square->size;

	for( int i = size - 1; i >= 0; i-- ) {
		free( *( square->array + i ) );
	}

	return 0;
}

/* construct_square reads the input file to initialize a square struct
 * from the contents of the file and returns the square.
 * The format of the file is defined in the assignment specifications
 */
Square * construct_square(char *filename)
{

	// Open and read the file
	FILE *file;
	// Open file and make sure it is valid
	file = fopen( filename, "r" );
	if ( file == NULL ) {
		printf( "%s\n", "Cannot open input file" );
		exit( 1 );
	}	

	// Buffer and integer for storing square size
	int size = 0;	
	char str[80];

	// Check fgets return value
	if ( fgets( str, 5, file ) == NULL ) {
		printf( "%s\n", "Error reading file" );
		exit( 1 );
	}
	// Store square size in the integer size
	size = atoi( str );
	
	// Create token used for strtok
	char *token;

	// Create the 2D array using the size
	int **squareArray;
	squareArray = malloc( sizeof( int ) * size );
	if ( squareArray == NULL ) {
		printf( "%s\n", "malloc is NULL" );
		exit( 1 );
	}
	for ( int i = 0; i < size; i++ ) {
		*( squareArray + i ) = malloc( sizeof( int ) * size );
	}

	// Populate the 2D array
	for ( int i = 0; i < size; i++ ) {
		int k = 0;
		fgets( str, 200, file );
		str[ strcspn( str, "\n" ) ] = 0;
		token = strtok( str, ", " );
		while ( token != NULL ) {
			*( *( squareArray + i ) + k ) = atoi( token );
			token = strtok( NULL, ", " );
			k = k + 1;
		}
	}

	// Close file
	fclose( file );
	
	// Initialize new square pointer and return it
	Square square = { size, squareArray };
	struct _Square *squarePointer;
	squarePointer = &square;

	return squarePointer;
}

/* verify_magic verifies if the square is a magic square
 * 
 * returns 1(true) or 0(false)
 */
int verify_magic(Square * square)
{
	int sum = 0;
	int tempSum = 0;
	int **squareArray = square->array;
	int size = square->size;	

	// Get sum of first col to compare to later
	for ( int i = 0; i < size; i++ ) {
		sum = sum + ( *( *( squareArray + i ) + 0 ) );
	}

	// Check all rows sum to same number
	for ( int i = 0; i < size; i++ ) {
		for ( int k = 0; k < size; k++ ) {
			tempSum = tempSum + *( *( squareArray + i ) + k );
		}
		if ( tempSum != sum ) {
			return 0;
		}
		tempSum = 0;
	}

	// Check all cols sum to same number
	for ( int i = 0; i < size; i++ ) {
		for ( int k = 0; k < size; k++ ) {
			tempSum = tempSum + *( *( squareArray + k ) + i );
		}
		if ( tempSum != sum ) {
			return 0;
		}
		tempSum = 0;
	}

	// Check main diagonal
	for ( int i = 0; i < size; i++ ) {
		tempSum = tempSum + *( *( squareArray + i ) + i );
	}
	if ( tempSum != sum ) {
		return 0;
	}
	tempSum = 0;

	// Check secondary diagonal
	for ( int i = size - 1; i > -1; i-- ) {
		tempSum = tempSum + *( *( squareArray + i ) + i );
	}
	if ( tempSum != sum ) {
		return 0;
	}	

	return 1;

}
