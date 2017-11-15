////////////////////////////////////////////////////////////////////////
// Main File: cache2Dcols.c
// This File: cache2Dcols.c
// Other Files: cache1D.c, cache2Drows.c 
// Semester: CS 354, Spring 2017 
//
// Author: Sean Kim
// Email: skim658@wisc.edu
// CS Login: seank
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

int arr2Dcols[3000][500]; 

int main( int argc, char *argv[] ) {   

	for ( int i = 0; i < 500; i++ ) {
		for ( int j = 0; j < 3000; j++ ) {
			arr2Dcols[j][i] = i + j;	
		}
	}
	return 0;
}
