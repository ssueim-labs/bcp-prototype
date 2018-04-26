#include <boost/filesystem.hpp>

#include "log.hpp"
#include "string.hpp"
#include "common.hpp"
#include "lockable.hpp"

#ifndef WIN32
#include <syslog.h>
#endif


// namespace bryllite
namespace bryllite { 

#ifdef WIN32

// EVENT log
bool report_event( WORD type, WORD category, DWORD eventId, const char* msg )
{
	bool result = true ;
	HANDLE hEvent = RegisterEventSource( NULL, "bryllite" ) ;
	if ( NULL == hEvent ) return false ;

	if ( !ReportEvent( hEvent, type, category, eventId, NULL, 1, 0, &msg, NULL ) )
		result = false ;

	DeregisterEventSource( hEvent ) ;
	return result ;
};

// LOG_TYPE -> EVENTLOG_TYPE
WORD to_event_log_type( LOG_TYPE logType ) {
	
	switch( logType ) {
	case LOG_TYPE_CRITICAL:
	case LOG_TYPE_ERROR:
		return EVENTLOG_ERROR_TYPE ;

	case LOG_TYPE_WARNING:
		return EVENTLOG_WARNING_TYPE ;

	default: break ;
//	case LOG_TYPE_DEBUG:
//	case LOG_TYPE_INFO:
//		return EVENTLOG_INFORMATION_TYPE ;
	};

	return EVENTLOG_INFORMATION_TYPE ;
};

#else // WIN32

// LOG_TYPE -> syslog priority
int to_priority( LOG_TYPE logType ) {
	switch( logType ) {
		case LOG_TYPE_CRITICAL:
			return LOG_CRIT;
		case LOG_TYPE_ERROR:
			return LOG_ERR;
		case LOG_TYPE_WARNING:
			return LOG_WARNING;
		case LOG_TYPE_DEBUG:
			return LOG_DEBUG;
		case LOG_TYPE_INFO:
			return LOG_INFO;

		default: break;
	};

	return LOG_NOTICE;
};


#endif // WIN32

// log manager constructor
LogManager::LogManager()
{
	m_logFilter = LOG_TYPE_NULL ;
};

// log manager destructor
LogManager::~LogManager()
{
};

void LogManager::cleanup() 
{
};

bool LogManager::initialize( LOG_TYPE filter, const char* logFile )
{
	m_logFilter = filter ;
	
	if ( logFile != nullptr ) m_logFile = logFile ;

	return true ;
};

// log type -> string
const char* LogManager::log_type_to_string( LOG_TYPE logType )
{
	switch( logType ) {
		case LOG_TYPE_CRITICAL: return "CRITICAL";
		case LOG_TYPE_ERROR: return "ERROR";
		case LOG_TYPE_WARNING: return "WARNING";
		case LOG_TYPE_INFO: return "INFO";
		case LOG_TYPE_USER: return "USER";
		case LOG_TYPE_DEBUG: return "DEBUG";
		default: break;
	};

	return "Unknown";
};


bool LogManager::app( const char* filepath, int line, LOG_TYPE logType, std::string format, ... )
{
	// filter check
	if ( !( logType & m_logFilter ) ) return true ;

	// filename only
	std::string filename = boost::filesystem::path(filepath).filename().string() ;

	// build-up formatted msg
	int size = ( format.size() * 2 ) + 1;
	std::string msg;
	msg.resize( size );

	va_list ap;
	va_start( ap, format );
	int len = vsnprintf( ( char* )msg.data(), size, format.c_str(), ap );
	if( len >= size )
	{
		size = len + 1;
		msg.resize( size );
		len = vsnprintf( ( char* )msg.data(), size, format.c_str(), ap );
	}
	va_end( ap );

	try {
		AUTO_LOCK( this, 0 ) ;
		std::cout << string_format( "[%s:%s]: %s code:%s(%d)", get_date_time_code().c_str(), log_type_to_string(logType), msg.c_str(), filename.c_str(), line ) << std::endl ;
	} catch( AutoLock::lock_exception& e ) {
		std::cout << e.what() << std::endl ;
	};

	return true ;
}


bool LogManager::sys( const char* filepath, int line, LOG_TYPE logType, std::string format, ... )
{
	// filter check
	if( !( logType & m_logFilter ) ) return true;

	// filename only
	std::string filename = boost::filesystem::path( filepath ).filename().string();

	// build-up formatted msg
	int size = ( format.size() * 2 ) + 1;
	std::string msg;
	msg.resize( size );

	va_list ap;

	va_start( ap, format );
	int len = vsnprintf( ( char* )msg.data(), size, format.c_str(), ap );
	if( len >= size )
	{
		size = len + 1;
		msg.resize( size );
		len = vsnprintf( ( char* )msg.data(), size, format.c_str(), ap );
	}
	va_end( ap );

	try {
		AUTO_LOCK( this, 0 ) ;
#ifdef WIN32
	report_event( to_event_log_type( logType ), 1000, 100, string_format( "[%s]: (%s:%d): %s", get_date_time_code().c_str(), filename.c_str(), line, msg.c_str() ).c_str() ) ;
#else // WIN32
	syslog( to_priority( logType ), string_format( "(%s:%d): %s", filename.c_str(), line, msg.c_str() ).c_str() ) ;
#endif // WIN32
	} catch( AutoLock::lock_exception& e ) {
		std::cout << e.what() << std::endl ;
	};

	return true;
}



} // namespace bryllite
