#include <stdint.h>
#include <stdbool.h>
#include "test_controller.h"


int main ( void )
{
	// Initilaize test controller with test input data
	tc_init ( &tc_init_data );
	
	for ( ;; )
	{
		// Run test controller tasks
		tc_tasks ();
		
		/* something else */
	}
	
	return 0;
}



/*** end of file ***/
