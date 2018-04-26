/*
 @File: service_manager.hpp
 @Desc: service manager class
 */
#pragma once

#include "bryllite/singleton.hpp"


// Service Manager class
class ServiceManager : public bryllite::Singleton< ServiceManager >
{
	friend class bryllite::Singleton< ServiceManager > ;
protected:
	ServiceManager() ;
	virtual ~ServiceManager() ;

public:
	bool initialize( void ) ;
	void cleanup( void ) ;

	bool start_service( int argc, char** argv ) ;
	bool stop_service( void ) ;

protected:
	int main( int argc, char** argv ) ;

protected:
};



