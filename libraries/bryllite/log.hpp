#pragma once

#include <string>
#include <iostream>
#include <stdarg.h>

#include <bryllite/singleton.hpp>
#include <bryllite/lockable.hpp>


// namespace bryllite
namespace bryllite { 

// log type
enum LOG_TYPE {
	LOG_TYPE_NULL = 0x0000,

	LOG_TYPE_CRITICAL = 0x0001,
	LOG_TYPE_ERROR = 0x0002,
	LOG_TYPE_WARNING = 0x0004,
	LOG_TYPE_INFO = 0x0008,
	LOG_TYPE_USER = 0x0010,
	LOG_TYPE_DEBUG = 0x0020,

	LOG_TYPE_RELEASE = LOG_TYPE_CRITICAL | LOG_TYPE_ERROR | LOG_TYPE_WARNING | LOG_TYPE_INFO | LOG_TYPE_USER,
	LOG_TYPE_ALL = 0xffff
};

// log manager class
class LogManager : public bryllite::Lockable, public bryllite::Singleton< LogManager >
{
	friend class bryllite::Singleton< LogManager > ;
protected:
	LogManager() ;
	virtual ~LogManager() ;

public:
	bool initialize( LOG_TYPE filter, const char* logFile ) ;
	void cleanup( void ) ;

	bool app( const char* filename, int line, LOG_TYPE logType, std::string format, ... ) ;
	bool sys( const char* filename, int line, LOG_TYPE logType, std::string format, ... ) ;

	bool toFile( const char* filename, int line, LOG_TYPE logType, std::string msg ) ;

	// enable file log with logFile. disalbled if logFile is null.
	bool enable_file_log( const char* logFile ) { 
		m_logFile = logFile ; 
	} ;

	const char* log_type_to_string( LOG_TYPE logType ) ;

protected:
	LOG_TYPE m_logFilter ;		// log type filter
	std::string m_logFile ;		// log file path
};

} // namespace bryllite


// using macros

#define logmanager()				bryllite::LogManager::getInstance()
#define	logmanager_cleanup()		bryllite::LogManager::releaseInstance()

#define	applogs( format, ... )		bryllite::LogManager::getInstance()->app( __FILE__, __LINE__, bryllite::LOG_TYPE_USER, format, ##__VA_ARGS__ ) 
#define	syslogs( format, ... )		bryllite::LogManager::getInstance()->sys( __FILE__, __LINE__, bryllite::LOG_TYPE_USER, format, ##__VA_ARGS__ ) 

// init log
inline bool init_log( bryllite::LOG_TYPE logFilter = bryllite::LOG_TYPE_ALL, const char* logFile = nullptr ) {
	return bryllite::LogManager::getInstance()->initialize( logFilter, logFile ) ;
} ;

// enable file log
inline bool enable_file_log( const char* filename ) {
	return bryllite::LogManager::getInstance()->enable_file_log( filename ) ;
} ;
