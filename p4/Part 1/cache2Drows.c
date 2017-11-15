//////////////////////////////////////////////////////////////
// Main File:		 cache2Drows.c
// This File:		 cache2Drows.c
// Other Files:		 cache1D.c, cache2Dcols.c  
// Semester:		 CS 354, Spring 2017 
//
// Author:		 Sean Kim
// Email:		 skim658@wisc.edu
// CS Login:		 seank
//
//////////////////////////////////////////////////////////////

#include <stdio.h> 

int arr2Drows[3000][500];

int main( int argc, char *argv[] ) {

	for ( int i = 0; i < 3000; i++ ) {
		for ( int k = 0; k < 500; k++ ) {
			arr2Drows[i][k] = i + k;
		}
	}
	return 0;
}
