#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "bryllite/common.hpp" 
#include "bryllite/compat/time.hpp"
#include "bryllite/lockable.hpp"
#include "bryllite/log.hpp"
#include "bryllite/compat/time.hpp"

#include <boost/thread.hpp>


int main( int argc, char** argv )
{
	init_log( bryllite::LOG_TYPE_ALL ) ;

	applogs( "%s, %s!", "Hello", "Bryllite" ) ;
	applogs( "today is '%s'", bryllite::get_date_time_code().c_str() ) ;

//	applog( "%s, %s!\n", "Hello", "Bryllite" ) ;
//	bryllite::LogManager::getInstance()->app( __FILE__, __LINE__, bryllite::LOG_TYPE_ALL, "this is log %s", "va_args" ) ;

//	applog( "this is applog" );
//	syslog( "this is syslog" );

	applogs( "get_system_time(): %ld", timeGetTime() ) ;

	time_t begin_time = timeGetTime() ;

	boost::this_thread::sleep_for( boost::chrono::milliseconds( 1500 ) ) ;
	time_t end_time = timeGetTime() ;

	applogs( "elased time: %ld(msec)", end_time - begin_time ) ;

	bryllite::Lockable lock ;


	return 0 ;
}
