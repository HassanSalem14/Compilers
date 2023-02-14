/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: Hassan Salem
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@          /       ==============================        /          @@
@         ////           @@       @@ #@                  ////         @
@         ////*          @@ @%  @ @@    @@  @ #@#       /////         @
@     ((( ////))))   @@  @% @%  @ @@ #@ @@  @ #@@#   ///////( ///     @
@     ((((,/ )))))    @@@@   @@@  @@ ##  @@@   #@#   ///// ,/////     @
@     (((((/)))((    ------------------------------    ((((./////*    @
@    //*./ /  .///   ---  PROGRAMMING LANGUAGE  ---  ////   / ( (//   @
@    ///// / /////   ==============================  * ////* / ////   @
@     ///// ///// ((                               (( ///// /////     @
@    ((((   / , (((((                             (((((  //   /(((    @
@    (((((((/ . (((((                          (((((* / (((((((       @
@      (((((( //((((/((((                    *((((/((((/( (((((/      @
@       .//,   * /   (((((                   (((((  ///    .//.       @
@     ,////////./(  (((((* ////         (///(((((( ,/(,////////       @
@         //////// ,// ((( /////,     ////// ((( //  /////// *        @
@            (((((((((,// * /////     /////   (//(((((((((            @
@            ((((((((((((/////         //.///  (((((((((((.           @
@                   (///////// //(   (//.//////////                   @
@                  (/////////             //////////                  @
@                                                                     @
@@          A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W          @@
@@@@                                                               @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */
#define LRN_FAIL1 -1         /* operation failure return value 1 */
#define LRN_FAIL2 -2         /* operation failure return value 2 */
#define LRN_LOAD_FAIL -2       /* load fail return value 2 */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 
#define TRUE 1
#define FALSE 0
#define SET_REALLOC_FLAG 1       /* realloc flag value */


#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define FUL 0x080
#define EMP 0x040
#define REL 0x20
#define READER_DEFAULT_FLAG 0x000 	/* (0000.0000)_2 = (000)_10 */
#define READER_SET_EOB  0x0002 /*0000.0000.0000.0010*/
#define READER_RESET_EOB 0xFFFD /*1111.1111.1111.1101*/
#define READER_CHECK_EOB 0x0002 /*0000.0000.0000.0010*/
#define READER_SET_REALLOC_FLAG 0x0001 /*0000.0000.0000.0001*/
#define READER_RESET_REALLOC_FLAG 0xFFFE /*1111.1111.1111.1110*/
#define READER_CHECK_REALLOC_FLAG 0x0001 /*0000.0000.0000.0001*/

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	learn_int mark;			/* the offset to the mark position (in chars) */
	learn_int read;			/* the offset to the get a char position (in chars) */
	learn_int wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	learn_char*	content;			/* pointer to the beginning of character array (character buffer) */
	learn_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	learn_int		increment;			/* character array increment factor */
	learn_int		mode;				/* operational mode indicator */
	learn_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	learn_int		histogram[NCHAR];	/* Statistics of chars */
	learn_int		numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(learn_int, learn_int, learn_int);
ReaderPointer	readerAddChar		(ReaderPointer const, learn_char);
learn_boln		readerClear		    (ReaderPointer const);
learn_boln		readerFree		    (ReaderPointer const);
learn_boln		readerIsFull		(ReaderPointer const);
learn_boln		readerIsEmpty		(ReaderPointer const);
learn_boln		readerSetMark		(ReaderPointer const, learn_int);
learn_int		readerPrint		    (ReaderPointer const);
learn_int		readerLoad			(ReaderPointer const, FILE* const);
learn_boln		readerRecover		(ReaderPointer const);
learn_boln		readerRetract		(ReaderPointer const);
learn_boln		readerRestore		(ReaderPointer const);
/* Getters */
learn_char		readerGetChar		(ReaderPointer const);
learn_char*	readerGetContent	(ReaderPointer const, learn_int);
learn_int		readerGetPosRead	(ReaderPointer const);
learn_int		readerGetPosWrte	(ReaderPointer const);
learn_int		readerGetPosMark	(ReaderPointer const);
learn_int		readerGetSize		(ReaderPointer const);
learn_int		readerGetInc		(ReaderPointer const);
learn_int		readerGetMode		(ReaderPointer const);
learn_byte	readerGetFlags		(ReaderPointer const);
learn_int		readerShowStat		(ReaderPointer const);
learn_int		readerNumErrors		(ReaderPointer const);

#endif
