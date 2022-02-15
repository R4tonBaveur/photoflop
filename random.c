#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Generate a random number from a min to a max 

int randomNumber( int min, int max)
{
	//initialize the random number generator with the current line 
	// have to be sure that it is initialized only once 

	static bool init = false;
	if(!init)
	{
		init = true;
		srand(time(NULL));
	}

	// generate the random number 

	return rand() % max + min;
}

