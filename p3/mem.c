#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct block_tag{

  int size_status;
  
 /*
  * Size of the block is always a multiple of 4
  * => last two bits are always zero - can be used to store other information
  *
  * LSB -> Least Significant Bit (Last Bit)
  * SLB -> Second Last Bit 
  * LSB = 0 => free block
  * LSB = 1 => allocated/busy block
  * SLB = 0 => previous block is free
  * SLB = 1 => previous block is allocated/busy
  * 
  * When used as the footer the last two bits should be zero
  */

 /*
  * Examples:
  * 
  * For a busy block with a payload of 24 bytes (i.e. 24 bytes data + an additional 4 bytes for header)
  * Header:
  * If the previous block is allocated, size_status should be set to 31
  * If the previous block is free, size_status should be set to 29
  * 
  * For a free block of size 28 bytes (including 4 bytes for header + 4 bytes for footer)
  * Header:
  * If the previous block is allocated, size_status should be set to 30
  * If the previous block is free, size_status should be set to 28
  * Footer:
  * size_status should be 28
  * 
  */

} block_tag;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
block_tag *first_block = NULL;

/* Global variable - Total available memory */
int total_mem_size = 0;

/* 
 * Function for allocating 'size' bytes
 * Returns address of the payload in the allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - If size is less than equal to 0 - Return NULL
 * - Round up size to a multiple of 4 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks when possible 
 * Tips: Be careful with pointer arithmetic 
 */
void* Mem_Alloc(int size){

	// check if passed in size is too small or too big and return NULL if invalid
	if ( size <= 0 || size >= total_mem_size ) {
		return NULL;
	}

	int allocSize = size;                 // allocSize is the size of the payload + header to be allocated
	if ( ( size%4 ) != 0 ) {
		allocSize = allocSize + ( 4 - ( size%4 ) );
	}	
	allocSize = allocSize + 4;            // add header to the allocation size

	int currWastedMem = total_mem_size;    // keeps track of the current block's wasted memory 
	int leastWastedMem = total_mem_size;   // keeps track of the least wasted memory
	int leastWastedBlockSize = 0;          // keeps track of the least wastful block size 
	block_tag *leastWastedMemPos = first_block;   // keeps track of the position that wastes the least amount of memory 
	block_tag *currPointer = first_block;  // pointer used to traverse through memory 
	int currSize = 0;                      // keeps track of current tag size 
	long mask = 4294967292;		       // mask to extract bits[2 - 31] from size status (2^32 - 4)	 

	int foundEmptyFit = 0;                 // boolean to check if the traversal found at least one free block large enough 	

	// while loop traverses heap and keeps track of valid blocks of memory that fit the data amount requested 
	while ( currPointer < ( block_tag* )( ( char* )first_block + total_mem_size ) ) {
		
		currSize = ( currPointer->size_status & mask );

		if ( ( currPointer->size_status & 1 ) == 0 && currSize >= allocSize ) { 
			currWastedMem = currSize - allocSize;  
			foundEmptyFit = 1;
			if ( currWastedMem < leastWastedMem ) {
				leastWastedMem = currWastedMem; 
				leastWastedMemPos = currPointer;
				leastWastedBlockSize = currSize;  
			}   	
		}
		
		// increment pointer based off of zero indexing 
		if ( ( ( int )currPointer & 1 ) == 0 ) {
			currPointer = ( block_tag* )( ( char* )currPointer + currSize );
		} else {
			currPointer = ( block_tag* )( ( char* )currPointer + currSize + 1 );  
		}
	} 

	// check if chosen block is large enough to split and split if large enough
	block_tag *temp = leastWastedMemPos; 
	if ( leastWastedMem >= 8 && foundEmptyFit == 1 ) {

		int splitBlockSize = leastWastedBlockSize - allocSize;

		struct block_tag splitHeader;
		splitHeader.size_status = splitBlockSize + 2;
 
		struct block_tag splitFooter;
		splitFooter.size_status = splitBlockSize; 
 		
		if ( ( ( int )temp & 1 ) == 0 ) {
			temp = ( block_tag* )( ( char* )temp + allocSize );
		} else {
			temp = ( block_tag* )( ( char* )temp + allocSize + 1 );
		}
		*( temp ) = splitHeader;   

		temp = leastWastedMemPos;

		temp = ( block_tag* )( ( char* )temp + ( splitBlockSize - 4 ) ); 
		*( temp ) = splitFooter;  
	
	// if the block is larger than the requested size but not large enough to split add padding and adjust the next block's header  
	} else if ( leastWastedMem > 0 && leastWastedMem < 8 && foundEmptyFit == 1 ) {
		
		allocSize = allocSize + leastWastedMem; 
		
		block_tag *nextHeader;
		if ( ( ( int )leastWastedMemPos & 1 ) == 0 ) {
			nextHeader = ( block_tag* )( ( char* )leastWastedMemPos + ( leastWastedMemPos->size_status & mask ) );
		} else {
			nextHeader = ( block_tag* )( ( char* )leastWastedMemPos + ( leastWastedMemPos->size_status & mask ) + 1 );
		}
		block_tag nextHeaderTag;
		nextHeaderTag.size_status = nextHeader->size_status + 2;
		*( nextHeader ) = nextHeaderTag;

	}

	// create new block tag for the header 
	struct block_tag newTag;

	// if the previous block is free, add one, add three if the previous block is allocated
	if ( ( leastWastedMemPos->size_status & 2 ) == 0 ) {
		newTag.size_status = allocSize + 1;
	} else if ( ( leastWastedMemPos->size_status & 2 ) == 2 ) {
		newTag.size_status = allocSize + 3;
	} 
	
	// put the new header in memory
 	*( leastWastedMemPos ) = newTag;	

	// if no free block was found that is large enough, return NULL 
	if ( foundEmptyFit == 0 ) {
		return NULL;
	}  

	// return pointer to the new block payload 
	leastWastedMemPos = ( block_tag* )( ( char* )leastWastedMemPos + 4 );
	return leastWastedMemPos;

}

/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the payload of the allocated block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not within the range of memory allocated by Mem_Init()
 * - Return -1 if ptr is not 4 byte aligned
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Mem_Free(void *ptr){
	
	// check if the passed in pointer is NULL or if the address it points to is past the last heap position
	if ( ptr == NULL || ( block_tag* )ptr >= ( block_tag* )( ( char* )first_block + total_mem_size ) ) {
		return -1; 
	}

	// check if pointer is 4 byte aligned and if the address it points to is before the first heap position
	if ( ( ( int )ptr%4 ) != 0 || ( block_tag* )ptr < first_block ) {
		return -1; 
	}

	// mask used to extract bits[2-31] for the size of the block
	int mask = 4294967292;

	// create new block_tag pointer and get the size status and size from header
	block_tag *header = ( block_tag* )( ( char* )ptr - 4 );
	int sizeStatus = header->size_status;
	int size = ( header->size_status & mask );

	// create block_tag pointers for the beginning of the new block and the beginning of the footer for the new block
	// also keep track of the new block's total size 
	block_tag *freeBlockStart = header; 
	block_tag *freeBlockFooterStart = ( block_tag* )( ( char* )header + ( size - 4 ) );  
	int totalSize = size; 

	// create new block_tag pointer to store the next block's header 
	block_tag *nextHeader;
	if ( ( ( int )header & 1 ) == 0 ) {
		nextHeader = ( block_tag* )( ( char* )header + size );
	} else {
		nextHeader = ( block_tag* )( ( char* )header + size + 1 ); 
	}

	// check if the next block is free, if it is, add it's size to total size and set the new footer to be the end of the block
	// if it is not free, change the next blocks header to show the previous block is free
	if ( ( nextHeader->size_status & 1 ) == 0 ) {
		totalSize = totalSize + ( nextHeader->size_status & mask );
		freeBlockFooterStart = ( block_tag* )( ( char* )nextHeader + ( ( nextHeader->size_status & mask ) - 4 ) ); 
	} else {
		block_tag newTag;
		newTag.size_status = nextHeader->size_status - 2;
		*( nextHeader ) = newTag;
	} 

	// check if previous block is free, if it is, add it's size to total size and set new header to be previous block header 
	// addresss 
	if ( ( sizeStatus & 2 ) == 0 ) {
		block_tag *prevBlock = ( block_tag* )( ( char* )header - 4 );  
		int prevBlockSize = prevBlock->size_status & mask;
		totalSize = totalSize + prevBlockSize;  
		freeBlockStart = ( block_tag* )( ( char* )prevBlock - ( prevBlockSize - 4 ) ); 
	} 

	// make the new header for the freed and possibly coalesced block and set it's size status 
	block_tag newHeader;
	newHeader.size_status = totalSize + 2;

	// make the new footer for the freed and possibly coalesced block and set it's size 
	block_tag newFooter;
	newFooter.size_status = totalSize;  

	// place the new header and footer into memory
	*( freeBlockStart ) = newHeader; 
	*( freeBlockFooterStart ) = newFooter; 

	// return 0 on success
	return 0; 	

}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Mem_Init(int sizeOfRegion){
  int pagesize;
  int padsize;
  int fd;
  int alloc_size;
  void* space_ptr;
  static int allocated_once = 0;
  
  if(0 != allocated_once){
    fprintf(stderr,"Error:mem.c: Mem_Init has allocated space during a previous call\n");
    return -1;
  }
  if(sizeOfRegion <= 0){
    fprintf(stderr,"Error:mem.c: Requested block size is not positive\n");
    return -1;
  }

  // Get the pagesize
  pagesize = getpagesize();

  // Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize
  padsize = sizeOfRegion % pagesize;
  padsize = (pagesize - padsize) % pagesize;

  alloc_size = sizeOfRegion + padsize;

  // Using mmap to allocate memory
  fd = open("/dev/zero", O_RDWR);
  if(-1 == fd){
    fprintf(stderr,"Error:mem.c: Cannot open /dev/zero\n");
    return -1;
  }
  space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (MAP_FAILED == space_ptr){
    fprintf(stderr,"Error:mem.c: mmap cannot allocate space\n");
    allocated_once = 0;
    return -1;
  }
  
  allocated_once = 1;
  
  // Intialising total available memory size
  total_mem_size = alloc_size;

  // To begin with there is only one big free block
  first_block = (block_tag*) space_ptr;
  
  // Setting up the header
  first_block->size_status = alloc_size;
  // Marking the previous block as busy
  first_block->size_status += 2;

  // Setting up the footer
  block_tag *footer = (block_tag*)((char*)first_block + alloc_size - 4);
  footer->size_status = alloc_size;
  
  return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header)(including the header/footer)
 */ 
void Mem_Dump() {
  int counter;
  char status[5];
  char p_status[5];
  char *t_begin = NULL;
  char *t_end = NULL;
  int t_size;

  block_tag *current = first_block;
  counter = 1;

  int busy_size = 0;
  int free_size = 0;
  int is_busy = -1;

  fprintf(stdout,"************************************Block list***********************************\n");
  fprintf(stdout,"No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
  fprintf(stdout,"---------------------------------------------------------------------------------\n");
  
  while(current < (block_tag*)((char*)first_block + total_mem_size)){

    t_begin = (char*)current;
    
    t_size = current->size_status;
    
    if(t_size & 1){
      // LSB = 1 => busy block
      strcpy(status,"Busy");
      is_busy = 1;
      t_size = t_size - 1;
    }
    else{
      strcpy(status,"Free");
      is_busy = 0;
    }

    if(t_size & 2){
      strcpy(p_status,"Busy");
      t_size = t_size - 2;
    }
    else strcpy(p_status,"Free");

    if (is_busy) busy_size += t_size;
    else free_size += t_size;

    t_end = t_begin + t_size - 1;
    
    fprintf(stdout,"%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n",counter,status,p_status,
                    (unsigned long int)t_begin,(unsigned long int)t_end,t_size);
    
    current = (block_tag*)((char*)current + t_size);
    counter = counter + 1;
  }
  fprintf(stdout,"---------------------------------------------------------------------------------\n");
  fprintf(stdout,"*********************************************************************************\n");

  fprintf(stdout,"Total busy size = %d\n",busy_size);
  fprintf(stdout,"Total free size = %d\n",free_size);
  fprintf(stdout,"Total size = %d\n",busy_size+free_size);
  fprintf(stdout,"*********************************************************************************\n");
  fflush(stdout);
  return;
}
