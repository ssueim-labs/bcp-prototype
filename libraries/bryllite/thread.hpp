/*
 @File: thread.hpp
 @Desc: Thread manager class 
 */
#pragma once

#include "bryllite/singleton.hpp"
#include "bryllite/lockable.hpp"

// namespace bryllite::thread
namespace bryllite { namespace thread {

class ThreadManager ;
class Thread ;
class Thread_ ;

const short int THREAD_IDX_NULL = -1 ;
const short int THREAD_IDX_ALL = 0xffff ;
const short int MAX_THREAD_CNT_PER_OBJECT = 32 ;
const short int MAX_THREAD_CNT = 1024 ;

// thread parameter context
typedef class THREAD_CONTEXT
{
public:
	THREAD_CONTEXT()
	{
	};

	virtual ~THREAD_CONTEXT()
	{
	};

public:
	int m_idx ;					// thread index
	Thread* m_pThread ;			// thread
	unsigned int m_threadId ;	// thread id

}*LPTHREAD_CONTEXT ;


// thread internal class
class Thread_
{
};


// thread class
class Thread 
{
protected:
	friend class ThreadManager ;
	friend unsigned int __stdcall _ThreadHandler( void* pContext ) ;

public:
	Thread() ;
	virtual ~Thread() ;

	bool initialize_thread( int thread_count ) ;
	void cleanup_thread( void ) ;

protected:
	virtual unsigned int onThreadMain( LPTHREAD_CONTEXT pContext ) = 0 ;		// pure virtual. must be implemented.
	virtual unsigned int onThreadBegin( LPTHREAD_CONTEXT pContext ) ;
	virtual unsigned int onThreadEnd( LPTHREAD_CONTEXT pContext ) ;

	Thread_* get_thread( int idx ) {
		if ( idx < 0 || idx >= MAX_THREAD_CNT_PER_OBJECT ) return nullptr ;
		return m_lstpThreads[ idx ] ;
	};

protected:
	Lockable m_lock ;
	std::vector< Thread_* > m_lstpThreads ;
	bool m_initialized ;
};


// thread manager class
class ThreadManager : public Lockable, public Singleton< ThreadManager >
{
	friend class Singleton< ThreadManager >;
protected:
	ThreadManager() ;
	virtual ~ThreadManager() ;

	void cleanup( void ) ;

public:

	bool add_thread( Thread* pThread ) ;
	bool remove_thread( Thread* pThread ) ;

//	bool create_thread( unsigned int ( __stdcall* address )(LPVOID), LPTHREAD_CONTEXT pContext, unsigned int flags ) ;
	size_t get_thread_count( void ) { return m_lstThreads.size() ; } ;

	// wait for all thread join
	unsigned int wait_for_all_thread_join( time_t timeout, time_t sleep ) ;

protected:
	bool is_thread_exists( Thread* pThread ) ;

protected:
	std::vector< Thread* > m_lstThreads ;		// thread lists
};

}} // namespace bryllite::thread