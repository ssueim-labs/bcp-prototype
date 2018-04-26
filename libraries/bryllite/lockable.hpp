/*
 @File: lockable.hpp
 @Desc: block scope lock class
 @Notes: using critical section on window, pthread mutext for else. need to migrated to boost::mutex
 */

#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include "bryllite/exceptions.hpp"
#include "bryllite/string.hpp"
#include "bryllite/compat/time.hpp"


// namespace bryllite
namespace bryllite {

// lockable class
class Lockable
{
	friend class AutoLock ;

public:
	// constructor
	Lockable() {
#ifdef WIN32
	InitializeCriticalSection( &m_cs ) ;
#else // WIN32
	pthread_mutex_init( &m_mtx, NULL ) ;
#endif // WIN32
	};

	// desctructor
	virtual ~Lockable() {
#ifdef WIN32
	DeleteCriticalSection( &m_cs ) ;
#else // WIN32
	pthread_mutex_destroy( &m_mtx ) ;
#endif // WIN32
	};

	bool is_locked( void ) {
#ifdef WIN32
	return ( m_cs.OwningThread != NULL ) ? true : false ;
#else // WIN32
	if ( pthread_mutex_trylock( &m_mtx ) == 0 ) {
		pthread_mutex_unlock( &m_mtx ) ;
		return false ;
	}
	return true ;
#endif // WIN32
	};

protected:
#ifdef WIN32
	CRITICAL_SECTION m_cs ;
#else // WIN32
	pthread_mutex_t m_mtx ;
	pthread_t m_lockedThreadId ;
#endif // WIN32

};


// locker class
class AutoLock 
{
public:
	enum lock_error_code {
		E_SUCCESS = 0,
		E_UNKNOWN,
		E_NULL_POINTER,
		E_TIMEOUT,

		E_END
	};

	class lock_exception : public bryllite::exception
	{
	public:

	public:
		lock_exception( int iErrCode, const char* errMessage ) : bryllite::exception( iErrCode, errMessage ) {
		};

		std::string what( void ) { return string_format( "errCode=%d, errMessage='%s'", m_errCode, m_errMessage.c_str() ) ; } ;
	};

public:
	AutoLock( Lockable* pLockable, time_t timeout, const char* file, int line ) 
	{
		if ( !( m_pLockable = pLockable ) ) throw lock_exception( lock_error_code::E_NULL_POINTER, "pLockable is null" ) ;

#ifdef WIN32
#if( _WIN32_WINNT >= 0x400 )
		time_t begin_time = timeGetTime() ;
		while( FALSE == TryEnterCriticalSection( &m_pLockable->m_cs ) ) {
			
			// lock timeout
			if ( timeout > 0 && timeGetTime() - begin_time > timeout ) {

//				std::cout << "autolock detects dead-lock" << std::endl ;

				m_pLockable = nullptr ;
				throw lock_exception( lock_error_code::E_TIMEOUT, string_format("Lock Timeout: (%d)msec", timeout).c_str() ) ;
			}

			SwitchToThread();
		};

#else // _WIN32_WINNT >= 0x400
		EnterCriticalSection( &m_pLockable->m_cs ) ;
#endif // _WIN32_WINNT >= 0x400

#else // WIN32
		time_t begin_time = timeGetTime() ;
		while( pthread_mutex_trylock( &m_pLockable->m_mtx ) != 0 ) {
			// lock timeout
			if ( timeout > 0 && timeGetTime() - begin_time > timeout ) {
				m_pLockable = nullptr ;
				throw lock_exception( lock_error_code::E_TIMEOUT, string_format( "Lock Timeout: (%d)msec", timeout).c_str() ) ;
			}

			boost::this_thread::sleep( boost::posix_time::milliseconds(0) ) ;
		};

//		pthread_mutex_lock( &m_pLockable->m_mtx ) ;
#endif // WIN32
	};

	virtual ~AutoLock()
	{
		if ( !m_pLockable ) return ;

#ifdef WIN32
		LeaveCriticalSection( &m_pLockable->m_cs ) ;
#else // WIN32
		pthread_mutex_unlock( &m_pLockable->m_mtx ) ;
#endif //WIN32
	};

protected:
	Lockable* m_pLockable ;
};


} // namespace bryllite

#define		AUTO_LOCK( lock, timeout )		bryllite::AutoLock __lock_object__( lock, timeout, __FILE__, __LINE__ )
