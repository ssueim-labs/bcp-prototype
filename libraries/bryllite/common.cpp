/*
 @File: common.cpp
 @Author: okseok.kim@gmail.com
 */

#include <stdio.h>

#include "common.hpp"
#include "version.hpp"

namespace bryllite {

// version string.
char* version( void )
{
	return VERSION ;
};

// version value.
int version_number( void )
{
	return VERSION_NUMBER ;
};

// major version
int version_major( void ) 
{
	return VERSION_MAJOR ;
};

// minor version
int version_minor( void )
{
	return VERSION_MINOR ;
};

// patch number
int version_patch( void )
{
	return VERSION_PATCH ;
};


} // namespace bryllite
