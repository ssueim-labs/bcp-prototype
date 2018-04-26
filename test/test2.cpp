#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "bryllite/common.hpp" 

int main( int argc, char** argv )
{
	std::cout << "Hello, Bryllite! " << bryllite::version() << std::endl ;

	return 0 ;
}
