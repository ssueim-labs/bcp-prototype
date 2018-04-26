#pragma once

#include <chrono>
#include <boost/thread.hpp>


#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#endif // WIN32

// namespace bryllite
namespace bryllite {

// get system time
inline time_t get_system_time( void ) {
	return std::chrono::duration_cast<std::chrono::milliseconds>( 
		std::chrono::high_resolution_clock::now().time_since_epoch() ).count() ;
};


// date time code
// e.g, "2018-04-26 05:24:30"
std::string get_date_time_code( const char* date_format = "%y-%m-%d", const char* time_format = "%h:%i:%s", time_t shift_in_sec = 0 ) ;
std::string get_date_code( const char* date_format = "%y-%m-%d", time_t shift_in_day = 0 ) ;
std::string get_time_code( const char* time_format = "%h:%i:%s", time_t shift_in_sec = 0 ) ;


// sleep in ms
inline void sleep_for( time_t ms ) {
	boost::this_thread::sleep( boost::posix_time::millisec( ms ) );
} ;


} // namespace bryllite

#ifndef WIN32
  // on windows: elapsed time since system boot
  // others: elased time since 1970
inline time_t timeGetTime() {
	return bryllite::get_system_time();
};

#else // WIN32

#endif // WIN32

