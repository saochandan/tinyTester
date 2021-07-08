/** @file test_controller.h
 * 
 * @brief This file provides public interface functions and data structures for
 *        test_controller.c 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2021 company_xyz ltd.  All rights reserved.
 */ 

#ifndef TEST_CONTROLLER_H
#define TEST_CONTROLLER_H


/* test case initialize function pointer*/
typedef bool (*tc_init_fn_t) ( void *test_input_data );

/* test case run function pointer*/
typedef void (*tc_run_fn_t) ( void );

/*
*/
typedef struct TEST_CASE {
	
	tc_init_fn_t 	p_tc_init_fn; 
	tc_run_fn_t 	p_tc_run_fn;
	void 			*p_input_data;
	
	/* Additionally, test case name/id, 
	log message buffer can be added */
	
} test_case_t;

/*
*/
typedef struct TEST_CASES {
	
	test_case_t *tc;
	uint32_t 	total_test_cases;
	
} tc_init_t;

/*
*/
typedef enum TC_STATES {
	
	TC_IDLE = 0, /* est case is not running */
	TC_INIT = 1, /* test case initialize */
	TC_INIT_WAIT = 2, /* wait for test case initialize to complete */
	TC_RUN = 3, /* run test case */
	TC_RUN_WAIT = 4, /* wait for test case execution to complete */
	TC_COMPLETE = 5 /* test case completed */
		
} tc_state_t;

/*!
 * @brief Initializes test controller. 
 * 	configures test controller with test case details. 
 *
 * @param[in] tc_init  contains test case details.
 *
 * @return None.
 */
void tc_init ( tc_init_t *tc_init );

/*!
 * @brief Executes test controller tasks. 
 * 	maintains state machine and executes test cases from the list. 
 *
 * @param[in] None.
 *
 * @return None.
 */
void tc_tasks ( void );

/*!
 * @brief Logs test case result passed by test app.
 *
 * @param[in] pass  = true, if test case passed else failed.
 *
 * @return None.
 */
void tc_log_result ( const bool pass );

/*!
 * @brief Prints messages with tag on console.
 *
 * @param[in] tag  tag string.
 * @param[in] msg  message.
 *
 * @return None.
 */
void tc_log_message ( const char *tag, const char *msg );

/* Test case list data to initialize
*/
extern tc_init_t tc_init_data;

#endif /* TEST_CONTROLLER_H */

/*** end of file ***/
