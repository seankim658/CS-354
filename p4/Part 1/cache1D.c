///////////////////////////////////////////////////////////////////////
// Main File:         cache1D.c 
// This File:         cache1D.c
// Other Files:       cache2Drows.c, cache2Dcols.c
// Semester:          CS 354 Spring 2017
//
// Author:            Sean Kim
// Email:             skim658@wisc.edu
// CS Login:          seank
//
///////////////////////////////////////////////////////////////////////

#include <stdio.h> 

int arr[100000]; 

int main( int argc, char *argv[] ) {
	for ( int i = 0; i < 100000; i++ ) {
		arr[i] = i;
	}
	return 0;
}
