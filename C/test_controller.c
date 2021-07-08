
/******************************************************************************
 * 							Include files
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "test_controller.h"

/******************************************************************************
 * 						Private variable declarations
******************************************************************************/

static test_case_t *p_test_list;
static uint32_t total_tests;
static uint32_t test_counter;
static bool test_result_logged;
static tc_state_t tc_state;

/******************************************************************************
 * 						Public function definitions
******************************************************************************/

/* This function initializes test cases by pointing at the beginning of the 
 * test case list. It also initializes test controller state and variables.
 */
void tc_init ( tc_init_t *tc_init )
{
	p_test_list = tc_init->tc;
	total_tests = tc_init->total_test_cases;
	test_counter = 0;
	test_result_logged = false;
	
	if ( total_tests > 0 )
	{
		tc_state = TC_INIT;
	}
	else
	{
		tc_state = TC_IDLE;
	}
}

/* This function maintains the test exeution states and performs respective
 * operations at each state.
 */
void tc_tasks ( void )
{
	switch ( tc_state )
	{		
		case TC_INIT:
			printf ("Executing test number: %d of %d\r\n", 
					(unsigned int)(test_counter+1), (unsigned int)total_tests);
			tc_state = TC_INIT_WAIT;
			test_result_logged = false;
			break;
			
		case TC_INIT_WAIT:
			/* Initilaize next test case in the list */
			if ( p_test_list->p_tc_init_fn( p_test_list->p_input_data ) == true )
			{
				tc_state = TC_RUN_WAIT;
			}
			break;
			
		case TC_RUN_WAIT:
			/* Runs test case logic, stay in this state as long as 
			   result is not logged */
			if (  false == test_result_logged )
			{
				p_test_list->p_tc_run_fn();
			}
			else
			{
				tc_state = TC_COMPLETE;
			}
			break;
			
		case TC_COMPLETE:
			/* Mark current test case completed and go to next test case */
			test_counter++;
			p_test_list++;
			printf ("Test %d completed\r\n", (unsigned int)test_counter);
			if ( test_counter  < total_tests )
			{
				tc_state = TC_INIT;
			}
			else
			{
				tc_state = TC_IDLE;
			}
			break;
		
		case TC_IDLE:
			/* No pending test cases */
			break;
					
		default:
			break;
	}
}

/* This function logs test case result
 */
void tc_log_result ( const bool pass )
{
	test_result_logged = true;
	printf ("Test Result: ");
	if ( true == pass )
	{
		printf ("PASS\r\n");
	}
	else
	{
		printf ("FAIL\r\n");
	}
		
}

/* This function logs messages
*/
void tc_log_message ( const char *tag, const char *msg )
{
	printf ("[%s] %s\r\n", tag, msg);
}

/*** end of file ***/


