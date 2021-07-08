/** @file circular_queue.h
 * 
 * @brief This file provides public interface functions and data structures for
 *        circular_queue.c 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2021 company_xyz ltd.  All rights reserved.
 */ 

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H


/* Defines type of value to be stored in queue
*/
typedef uint8_t cq_val_t;

/* Defines type of queue index and it's size
*/
typedef uint8_t cq_size_t;

/* Defines queue size (statically allocated)
*/
#define CQ_SIZE	(cq_size_t)10

/* Defines queue parameters
*/
typedef struct CQ {
	
	cq_val_t	buff[CQ_SIZE]; 	/* Buffer where data will be stored */
	cq_size_t	wr; 			/* Enqueue (write) index */
	cq_size_t	rd; 			/* Dequeue (read) index */
	
} cq_t;

/* Defines queue status
*/
typedef enum CQ_STATUS {
	
	CQ_IS_EMPTY = 1, 	/* Completely empty, not available to read */
	CQ_IS_FULL = 2, 	/* Completely full, not available to write */
	CQ_OK = 0 			/* Denotes write/read was successful */
		
} cq_status_t;

/*!
 * @brief Initializes circular queue.
 *
 * @param[in] q  Pointer object of queue to be initialized.
 *
 * @return None.
 */
void cq_init ( cq_t *q );

/*!
 * @brief enqueues circular queue.
 *
 * @param[in] q  Pointer object of queue to be enqueued.
 * @param[in] val  Value to be stored in queue.
 *
 * @return Queue status.
 */
cq_status_t cq_enqueue ( cq_t *q, cq_val_t val );

/*!
 * @brief dequeues circular queue.
 *
 * @param[in] q  Pointer object of queue to be dequeued.
 * @param[in] val  Address to store read value.
 *
 * @return Queue status.
 */
cq_status_t cq_dequeue ( cq_t *q, cq_val_t *val );

/*!
 * @brief Provides queue empty status.
 *
 * @param[in] q  Pointer object of queue.
 *
 * @return true if empty else false.
 */
bool cq_is_empty ( cq_t *q );




#endif /* CIRCULAR_QUEUE_H */

/*** end of file ***/
