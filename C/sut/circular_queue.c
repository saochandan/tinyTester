/** @file circular_queue.c
 * 
 * @brief This file implements a queue based on circular buffer. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2021 company_xyz ltd. All rights reserved.
 */
/******************************************************************************
 * 							Include files
******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "circular_queue.h"

/******************************************************************************
 * 						Private function declarations
******************************************************************************/

static cq_val_t _next_idx ( cq_val_t curr_idx );

/******************************************************************************
 * 						Global variable declarations
******************************************************************************/


/******************************************************************************
 * 						Public function definitions
******************************************************************************/


/* This function initializes circular queue by resetting write and read indexes
*/
void cq_init ( cq_t *q )
{
	/* Initialize queue parameters */
	q->rd = 0;
	q->wr = 0;
}

/* This function enqueues by writing passed value if an empty space available, 
 * else, it returns queue status.
*/
cq_status_t cq_enqueue ( cq_t *q, cq_val_t val )
{
	cq_status_t stat = CQ_OK;
	
	/* Make sure an empty space is available */
	if ( cq_is_empty ( q ) == true )
	{
		/* update queue */
		q->buff[q->wr] = val;
		
		/* update wr index */
		q->wr = _next_idx ( q->wr );
	}
	else
	{
		/* No empty space to enqueue */
		stat = CQ_IS_FULL; 
	}
	
	return stat;
}

/* This function dequeues by reading value into a given location if available, 
 * else, it returns queue status.
*/
cq_status_t cq_dequeue ( cq_t *q, cq_val_t *val )
{
	cq_status_t stat = CQ_OK;
	
	/* Make sure an value available to read and dequeue */
	if ( q->wr == q->rd )
	{
		/* No available element to dequeue */
		stat = CQ_IS_EMPTY; 
	}
	else
	{
		/* read queue */
		*val = q->buff[q->rd];
		
		/* update rd index */
		q->rd = _next_idx ( q->rd );
	}
	
	return stat;
}

/* This function returns true if queue has empty space, else false.
*/
bool cq_is_empty ( cq_t *q )
{
	bool stat = true;
	
	if ( _next_idx ( q->wr ) == q->rd )
	{
		stat = false;
	}
	
	return stat;
}


/******************************************************************************
 * 						Private function definitions
******************************************************************************/
/* This function updates index, rolls over when buffer size reaches to max.
*/
static cq_val_t _next_idx ( cq_val_t curr_idx )
{
	curr_idx++;
	if ( curr_idx > CQ_SIZE )
	//if ( curr_idx >= CQ_SIZE ) // Introduce a bug
	{
		curr_idx = 0;
	}
	
	return curr_idx;
}



/******************************************************************************
 * 						Debug/Test function definitions
******************************************************************************/
/* We may implement functions to obtain access to internal variables in read 
*  mode for testing purposes, can be removed in final code via compiler directives  
*/

// #define TESTING_MODE

#ifdef TESTING_MODE

#endif


/*** end of file ***/
