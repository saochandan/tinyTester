/** @file test_app.c
 * 
 * @brief This file implements a queue based on circular buffer. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2021 company_xyz ltd. All rights reserved.
 */
 
/******************************************************************************
 * 							Include files
******************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "sut/circular_queue.h"
#include "test_controller.h"

/******************************************************************************
 * 					Common typedef / macro definitions
******************************************************************************/

// Note: This section can be moved to .h file in case of multiple test app

#define Q_SIZE CQ_SIZE

/* Defines test case run states
*/
typedef enum TEST_CASE_STATES {
	
	TEST_CASE_IDLE = 0, 		/* test case is not running */
	TEST_CASE_INIT = 1, 		/* test case initialize */
	TEST_CASE_RUN = 2, 			/* wait for test case initialize to complete */
	TEST_CASE_VERIFY = 3, 		/* run test case */
	TEST_CASE_LOG_RESULT = 4 	/* wait for test case execution to complete */
		
} test_case_states_t;

/******************************************************************************
 * 						Private variable declarations
******************************************************************************/

static cq_t q[2]; /* Create 2 circular queues */
static cq_t *tc_data; /* pointer to data for test cases */
static test_case_states_t test_case_state; /* test case run states */
static bool test_result; /* stores test result */

/******************************************************************************
 * 						Private function declarations
******************************************************************************/

static bool test_case_init ( void *test_input_data );
static void test_case_1_run ( void );
static void test_case_2_run ( void );
static void test_case_3_run ( void );
static void test_case_4_run ( void );
static void test_case_5_run ( void );
static void test_case_6_run ( void );
static void test_case_7_run ( void );
static void test_case_8_run ( void );
static void test_case_9_run ( void );



/******************************************************************************
 * 							Test case List
******************************************************************************/

test_case_t test_case_list_1 [] = {
	
	/* Q1 *******************************************/
	/* Test case to test cq_init functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_1_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test cq_enqueue functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_2_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test cq_dequeue functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_3_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test cq_is_empty functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_4_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test return value of cq_enqueue as CQ_OK */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_5_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test return value of cq_enqueue as CQ_IS_FULL */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_6_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test return value of cq_dequeue as CQ_OK */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_7_run,
		.p_input_data = (void *)&q[0]
	},
	
	/* Test case to test return value of cq_dequeue as CQ_IS_EMPTY */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_8_run,
		.p_input_data = (void *)&q[0]
	},
	
	
	/* Q2 *******************************************/
	/* Test case to test cq_init functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_1_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test cq_enqueue functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_2_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test cq_dequeue functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_3_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test cq_is_empty functionality */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_4_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test return value of cq_enqueue as CQ_OK */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_5_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test return value of cq_enqueue as CQ_IS_FULL */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_6_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test return value of cq_dequeue as CQ_OK */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_7_run,
		.p_input_data = (void *)&q[1]
	},
	
	/* Test case to test return value of cq_dequeue as CQ_IS_EMPTY */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_8_run,
		.p_input_data = (void *)&q[1]
	},
	
	/*******************************************************/
	/* Verify writing to one queue doesn't affect the other */
	{
		.p_tc_init_fn = test_case_init,
		.p_tc_run_fn = test_case_9_run,
		.p_input_data = (void *)q
	},
	
	
};

// test case init data
tc_init_t tc_init_data = { .tc = test_case_list_1, 
							.total_test_cases = sizeof(test_case_list_1)/sizeof(test_case_list_1[0]) 
						};

/******************************************************************************
 * 								Test case Init 
******************************************************************************/

/*!
 * @brief this function initializes test case.
 *	This can include test case specific data assignment 
 *	code to satisfy pre-condition
 *  test state machine, test result initialization, etc.
 * @param[in] None.
 *
 * @return initilaization status. true or false.
 */
static bool test_case_init ( void *test_input_data )
{
	tc_data = (cq_t *) test_input_data;
	test_case_state = TEST_CASE_INIT;
	test_result = false;
	
	/* return false; if need to wait */
	return true;
}


/******************************************************************************
 * 								Test case Run
******************************************************************************/

/*! UNIT TESTING
 * @brief this function tests cq_init().
 *	Pre-condition: none (pre-conditions can be satisfied in test_case_init() )
 *  Description: call cq_init() and pass queue pointer
 *  Expected Output: cq_init() should make wr and rd indexes equal
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_1_run ( void )
{
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* call function to be tested */
			cq_init ( tc_data );
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			/* after calling cq_init, 
			queue wr and rd indexs should be equal */
			if ( tc_data->rd == tc_data->wr )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR",  "cq_init() didn't reset queue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}


/*! UNIT TESTING
 * @brief this function tests cq_enqueue().
 *	Pre-condition: reset wr/rd index to 0
 *  Description: Write '10' at index = 0 using function to be tested
 *  Expected Output: value '10' should be written and enqueued at wr index 
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_2_run ( void )
{
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			tc_data->wr = 0;
			tc_data->rd = 0;
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* call function to be tested */
			cq_enqueue ( tc_data, (cq_val_t)10 );
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			/* after calling cq_enqueue, data at index 0 should be 10
			and queue size should be 1 */
			if ( (cq_val_t)10 == tc_data->buff[tc_data->rd] && /* verify the content*/
				abs(tc_data->wr - tc_data->rd) == 1 /* verify the enqueue state */
				)
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR",  "cq_enqueue() didn't write correct value or\
						it didn't enqueue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			/* Note: These two verifications can be further divided into two test cases */
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}


/*! UNIT TESTING
 * @brief this function tests cq_dequeue().
 *	Pre-condition: set wr index to 1, rd index to 0 and 
 *	Write '20' at index = 0 
 *  Description: read data at index = 0 using function to be tested
 *  Expected Output: value '20' should be read and dequeued from rd index
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_3_run ( void )
{
	static cq_val_t val;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			val = 0;
			tc_data->wr = 1;
			tc_data->rd = 0;
			tc_data->buff[0] = (cq_val_t)20;
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* call function to be tested */
			cq_dequeue ( tc_data, &val );
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			/* after calling cq_dequeue, read data should be 20
			and queue size should be 0 */
			if ( (cq_val_t)20 == val && /* verify the content*/
					(tc_data->wr == tc_data->rd) /* verify the dequeue state */
				)
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR",  "cq_dequeue() didn't read correct value or\
						it didn't dequeue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			/* Note: These two verifications can be further divided into two test cases */
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}

/*! SOFTWARE INTEGRATION/QUALIFICATION TESTING
 * @brief this function tests cq_dequeue().
 *	Pre-condition: set wr index to 1, rd index to 0 and 
 *	Write '20' at index = 0 
 *  Description: read data at index = 0 using function to be tested
 *  Expected Output: value '20' should be read and dequeued from rd index
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_4_run ( void )
{
	static cq_val_t val;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			
			val = 0;
			cq_init (tc_data); 
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			/* fill queue completely */
			for ( int v=0; v<Q_SIZE; v++ )
			{
				cq_enqueue( tc_data, (cq_val_t)v );
			}
			
			/* queue should be completely full */
			if ( cq_is_empty ( tc_data ) == false )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR",  "cq_is_empty() didn't return 'false' for completelt filled queue");
			}
			/* now dequeue by 1 */
			cq_dequeue( tc_data, &val );
			
			/* queue should be empty */
			if ( cq_is_empty ( tc_data ) == false )
			{
				test_result = false;
			}
			
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}



/*! UNIT TESTING
 * @brief this function tests return value of cq_enqueue().
 *	Pre-condition: set wr index to 0, rd index to 0 and 
 *	Fill the CQ completely.
 *  Description: Tests return value of cq_enqueue() as CQ_OK
 *  Expected Output: return value should be CQ_OK
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_5_run ( void )
{
	static cq_status_t stat;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			tc_data->wr = 5;
			tc_data->rd = 5;
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* fill queue completely */
			for ( int v=0; v<Q_SIZE; v++ )
			{
				stat = cq_enqueue( tc_data, (cq_val_t)v );
				if ( CQ_OK != stat )
				{
					break;
				}
			}
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			if ( CQ_OK == stat )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR", "cq_enqueue() didn't return OK when attempting to write to an empty queue");
			}
			
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}



/*! UNIT TESTING
 * @brief this function tests return value of cq_enqueue().
 *	Pre-condition: set wr index to 0, rd index to 0 and 
 *	Fill the CQ completely. Attempt to fill one more item.
 *  Description: Tests return value of cq_enqueue() as CQ_IS_FULL
 *  Expected Output: return value should be CQ_IS_FULL
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_6_run ( void )
{
	static cq_status_t stat;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			tc_data->wr = 0;
			tc_data->rd = 0;
			
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* fill queue completely */
			for ( int v=0; v<Q_SIZE; v++ )
			{
				stat = cq_enqueue( tc_data, (cq_val_t)v );
			}
			/* add one more */
			stat = cq_enqueue( tc_data, (cq_val_t)10 );
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			if ( CQ_IS_FULL == stat )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR", "cq_enqueue() didn't return FULL when attempting to write to completely filled queue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}



/*! UNIT TESTING
 * @brief this function tests return value of cq_dequeue().
 *	Pre-condition: set wr index to Max, rd index to 0 and 
 *	Empty the CQ completely.
 *  Description: Tests return value of cq_dequeue() as CQ_OK
 *  Expected Output: return value should be CQ_OK
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_7_run ( void )
{
	static cq_status_t stat;
	cq_val_t val;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			
			val = 0;
			tc_data->wr = Q_SIZE; // fill completely
			tc_data->rd = 0;
			
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* empty queue completely */
			for ( int v=0; v<Q_SIZE; v++ )
			{
				stat = cq_dequeue( tc_data, &val );
				if ( CQ_OK != stat )
				{
					break;
				}
			}
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			if ( CQ_OK == stat )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR", "cq_dequeue() didn't return OK when attempting to read from a non-empty queue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
}


/*! UNIT TESTING
 * @brief this function tests return value of cq_dequeue().
 *	Pre-condition: set wr index to 0, rd index to 0 and 
 *	Empty the CQ completely. Attempt to empty one more item.
 *  Description: Tests return value of cq_dequeue() as CQ_IS_EMPTY
 *  Expected Output: return value should be CQ_IS_EMPTY
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_8_run ( void )
{
	static cq_status_t stat;
	cq_val_t val;
	
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			
			val = 0;
			tc_data->wr = Q_SIZE; 
			tc_data->rd = 0;
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			/* empty queue completely */
			for ( int v=0; v<Q_SIZE; v++ )
			{
				stat = cq_dequeue( tc_data, &val); 
			}
			/* dequeue one more */
			stat = cq_dequeue( tc_data, &val );
			
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			if ( CQ_IS_EMPTY == stat )
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR", "cq_dequeue() didn't return EMPTY when attempting to read from a empty queue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
	
}


/*! UNIT TESTING
 * @brief this function tests cq_is_empty() with 2 CQs.
 *	Pre-condition: set wr index to Max, rd index to 0 for CQ1 and 
 *	set wr index to 0, rd index to 0 for CQ2 
 *  Description: Verify that writing to CQ1 doesn't affect CQ2 using 
 *				 cq_is_empty()
 *  Expected Output: cq_is_empty() should return false for CQ1 and 
 *					true for CQ2
 * @param[in] None.
 *
 * @return None.
 */
static void test_case_9_run ( void )
{
	switch ( test_case_state )
	{
		case TEST_CASE_INIT:
			/* Initialize test case */
			
			tc_data[0].wr = Q_SIZE; 
			tc_data[0].rd = 0; 
			tc_data[1].wr = 0; 
			tc_data[1].rd = 0; 
			
			test_case_state = TEST_CASE_RUN;
			break;
		
		case TEST_CASE_RUN:
			test_case_state = TEST_CASE_VERIFY;
			break;
		
		case TEST_CASE_VERIFY:
			if ( ( cq_is_empty( &tc_data[0] ) == false ) &&
				 ( cq_is_empty( &tc_data[1] ) == true )
				)
			{
				test_result = true;
			}
			else
			{
				tc_log_message ("ERROR", "cq_is_empty() didn't return 'false' for non-empty queue or 'true' for empty queue");
			}
			test_case_state = TEST_CASE_LOG_RESULT;
			break;
		
		case TEST_CASE_LOG_RESULT:
			/* log test case result */
			tc_log_result ( test_result );
			test_case_state = TEST_CASE_IDLE;
			break;
		
		case TEST_CASE_IDLE:
			break;
		
		default:
			break;
	}
	
	
}

/*** end of file ***/


