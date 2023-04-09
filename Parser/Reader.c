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
* Purpose: This file is the main code for Reader (.c)
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
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

ReaderPointer readerCreate(learn_int size, learn_int increment, learn_int mode) {
	ReaderPointer readerPointer;
	/* Defensive programming */
	if (size <= 0)
		size = READER_DEFAULT_SIZE;
	if (increment <= 0)
		increment = READER_DEFAULT_INCREMENT;
	if (mode != MODE_FIXED)
		mode = MODE_FIXED;
	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	/* Defensive programming */
	if (!readerPointer)
		return NULL;
	readerPointer->content = (learn_char*)malloc(size);
	/* Defensive programming */
	if (!readerPointer->content) {
		free(readerPointer);
		return NULL;
	}
	/* Initialize the histogram */
	memset(readerPointer->histogram, 0, sizeof(readerPointer->histogram));
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	/* Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;
	/* The created flag must be signalized as EMP */
	readerPointer->flags |= EMP;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, learn_char ch) {
	learn_char* tempReader = NULL;
	learn_int newSize = 0;

	if (!readerPointer) {
		return NULL;
	}

	if (!ch) {
		readerPointer->numReaderErrors++;
		return readerPointer;
	}

	readerPointer->flags &= ~(1 << 5); /* Reset Relocation */

	if (readerPointer->position.wrte * (learn_int)sizeof(learn_char) < readerPointer->size) {
		readerPointer->flags &= ~(1 << 7); /* This buffer is NOT full */
	}
	else {
		readerPointer->flags &= ~(1 << 7); /* Reset Full flag */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			readerPointer->numReaderErrors++;
			return readerPointer;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			break;
		default:
			readerPointer->numReaderErrors++;
			return readerPointer;
		}

		tempReader = (learn_char*)realloc(readerPointer->content, newSize);
		if (!tempReader) {
			readerPointer->numReaderErrors++;
			return readerPointer;
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags |= (1 << 5); /* Check Relocation */
	}

	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->histogram[ch]++;
	return readerPointer;
}



/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_boln readerClear(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	/* Reset flags */
	readerPointer->flags &= ~(FUL | EMP);
	readerPointer->flags |= EMP;

	return LEARN_TRUE;
}

learn_boln readerFree(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	/* Free pointers */
	free(readerPointer->content);
	free(readerPointer);

	return LEARN_TRUE;
}


learn_boln readerIsFull(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	/* Check flag if buffer is FUL */
	return (readerPointer->flags & FUL) ? LEARN_TRUE : LEARN_FALSE;
}

learn_boln readerIsEmpty(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	/* Check flag if buffer is EMP */
	return (readerPointer->flags & EMP) ? LEARN_TRUE : LEARN_FALSE;
}

learn_boln readerSetMark(ReaderPointer const readerPointer, learn_int mark) {
	if (!readerPointer || mark<0 || mark > readerPointer->position.wrte)
		return LEARN_FALSE;

	/* Adjust mark */
	readerPointer->position.mark = mark;

	return LEARN_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerPrint(ReaderPointer const readerPointer) {
	learn_int cont = 0;
	learn_char c;
	if (!readerPointer || !readerPointer->content || readerPointer->position.wrte <= 0)
		return 0;

	/* Reset read position */
	readerPointer->position.read = 0;

	c = readerGetChar(readerPointer);
	while (readerPointer->position.read < readerPointer->position.wrte) {
		if (!isprint(c))
			return 0;

		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);

	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	learn_int size = 0;
	learn_char c;

	if (!readerPointer || !fileDescriptor)
		return READER_ERROR;

	c = (learn_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (learn_char)fgetc(fileDescriptor);
		size++;
	}

	if (ferror(fileDescriptor))
		return READER_ERROR;

	return size;
}



/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_boln readerRecover(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	readerPointer->position.read = readerPointer->position.mark;
	readerPointer->position.mark = -1;

	return LEARN_TRUE;
}

/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_boln readerRetract(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	if (readerPointer->position.read <= 0)
		return LEARN_FALSE;

	readerPointer->position.read--;
	return LEARN_TRUE;
}



/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_boln readerRestore(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return LEARN_FALSE;

	readerPointer->position.read = readerPointer->position.mark;

	return LEARN_TRUE;
}




/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_char readerGetChar(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	if (readerPointer->position.read >= readerPointer->position.wrte) {
		readerPointer->flags |= READER_SET_EOB;
		return READER_ERROR;	
	}

	readerPointer->flags &= READER_RESET_EOB;
	return readerPointer->content[readerPointer->position.read++];
}



/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_char* readerGetContent(ReaderPointer const readerPointer, learn_int pos) {
	if (!readerPointer || pos < 0)
		return NULL;

	return &readerPointer->content[pos];
}




/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetPosRead(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetPosWrte(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR;

	return readerPointer->position.wrte;
}



/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetPosMark(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	return readerPointer->position.mark;
}



/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetSize(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	return readerPointer->size;
}


/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetInc(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	return readerPointer->increment;
}


/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerGetMode(ReaderPointer const readerPointer) {
	if (!readerPointer)
		return READER_ERROR;

	return readerPointer->mode;
}



/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_byte readerGetFlags(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		// Handle error
		return 0;
	}

	/* TO_DO: Return flags */
	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerShowStat(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}

	/* TO_DO: Updates the histogram */
	learn_int histogram[FUL] = { 0 };
	for (learn_int i = 0; i < readerPointer->size; i++) {
		learn_char c = readerPointer->content[i];
		histogram[c]++;
	}

	printf("Histogram of characters in reader content:\n");
	for (learn_int i = 0; i < FUL; i++) {
		if (histogram[i] > 0) {
			printf("'%c': %d\n", i, histogram[i]);
		}
	}

	return LEARN_TRUE;
}


/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
learn_int readerNumErrors(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}

	/* TO_DO: Updates the histogram */
	return readerPointer->numReaderErrors;
}

