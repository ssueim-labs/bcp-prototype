/*
 @File: service_manager.cpp
 @Desc: service manager class impl.
 */
#include "service_manager.hpp"

ServiceManager::ServiceManager()
{
}

ServiceManager::~ServiceManager()
{
	cleanup() ;
}

void ServiceManager::cleanup( void )
{
	
}

bool ServiceManager::initialize( void )
{

	return true ;
}

bool ServiceManager::start_service( int argc, char** argv )
{
	return main( argc, argv ) > 0 ? true : false ;
}

bool ServiceManager::stop_service( void )
{
	return true ;
}

int ServiceManager::main( int argc, char** argv )
{

	return 1 ;
}




