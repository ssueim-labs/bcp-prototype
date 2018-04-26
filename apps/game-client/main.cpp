/*
 @File: main.cpp
 @Desc: Bryllite Game Client simulator main entrance
 @Author: jade@bryllite.com
 @License: Bryllite Ltd. All rights reserved.
 @CDate: 2018-04-24
 @UDate: 2018-04-24
 */

#include <iostream>
#include <string>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/thread.hpp>

#ifdef WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

#include "bryllite/common.hpp"
#include "bryllite/singleton.hpp"
#include "bryllite/log.hpp"
#include "bryllite/string.hpp"

#include "service_manager.hpp"



// shutdown service
void shutdown_service( void ) ;


 // client emulator class
class ClientEmulator 
{
public:
	ClientEmulator() {
		m_bRun = false;
		m_pThread = nullptr;
	};

	virtual ~ClientEmulator() {
		if( m_pThread != nullptr ) {
			delete m_pThread;
			m_pThread = nullptr;
		}
	};

	// run client thread
	bool run( int clientId, void* pContext ) {
		if ( m_pThread == nullptr ) {
			m_pThread = new boost::thread( boost::bind( &ClientEmulator::t_worker, this, clientId, pContext ) );
			return m_pThread != nullptr ? true : false ;
		}

		return false ;
	};

	// thread worker
	void t_worker( int clientId, void* pContext ) {
		m_bRun = true;
		static bryllite::Lockable lock ;

		while( m_bRun ) 
		{
			bryllite::sleep_for(0);	// yield to other thread

			try {
				AUTO_LOCK( &lock, 1000 ) ;

				std::cout << "clientId[" << clientId << "] says: Hello, Bryllite!" << std::endl;

				// sleep for 1 sec
				bryllite::sleep_for( 250 ) ;
			} catch( bryllite::AutoLock::lock_exception& e ) {
				applogs( "%s", e.what().c_str() ) ;
			}
		};
	};

	// make client stop
	void stop( void ) { m_bRun = false; };

	// wait for thread terminated
	void join( void ) {
		if ( m_pThread ) {
			m_pThread->join() ;
		}
	};

protected:
	bool m_bRun;
	boost::thread* m_pThread;
};

// ClientEmulator 
std::vector<ClientEmulator*> g_clients;



#ifdef WIN32

// console handler CTRL-C Event
BOOL WINAPI console_handler( DWORD signal )
{
	switch( signal )
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		shutdown_service() ;
		break ;
	}

	return TRUE ;
}

#else	// WIN32

// signal handler
void sig_handler( int sig ) 
{
	switch( sig ) 
	{
	case SIGINT:		// CTRL-C
		shutdown_service() ;
		break ;
	}
}

#endif	// WIN32

// close all client and quit
void shutdown_service( void )
{
	// make all client to stop
	for( auto& t : g_clients ) {
		t->stop();
	}
}


// main entry point
int main( int argc, char** argv )
{
	init_log() ;

	const int max_client_worker_count = 20;

#ifdef WIN32
	// console handler for ctrl-c exit
	SetConsoleCtrlHandler( console_handler, TRUE ) ;
#else
	// signal handler for ctrl-c exit
	signal( SIGINT, sig_handler ) ;
#endif

	// program option ¼³Á¤
	namespace opts = boost::program_options ;
	opts::options_description desc("allowed options");
	desc.add_options()
		( "help,h", "= Show help messages" )
		( "version,v", "= Show version informations" )
		( "thread-count", opts::value<int>()->default_value(5), bryllite::string_format("= set client thread count. max=%d", max_client_worker_count ).c_str() )
		( "server-port,p", opts::value<int>()->default_value(3800), "= set server port number" ) 
		( "config-file", opts::value<std::string>()->default_value("config.ini"), "= set config file path" ) ;

	opts::variables_map vm;
	try {
		opts::store( opts::parse_command_line( argc, argv, desc ), vm ) ;
		opts::notify( vm ) ;
	} catch( opts::error& e ) {
		std::cout << e.what() << std::endl ;
		return -1 ;
	}

	// show help message and quit
	if ( vm.count( "help" ) ) {
		std::cout << "Usage" << std::endl << std::endl ;
		std::cout << argv[0] << " [options]" << std::endl << std::endl ;

		std::cout << desc << std::endl ;
		return 1 ;
	}

	// show version information and quit
	if ( vm.count( "version" ) ) {
		applogs( "bryllite-core version: %s", bryllite::version() ) ;
		return 1 ;
	}

	// show options value and start
#ifdef WIN32
	int thread_count = min( max_client_worker_count, vm["thread-count"].as<int>() ) ;
#else
	int thread_count = std::min( max_client_worker_count, vm["thread-count"].as<int>() );
#endif

	std::cout << "Hello, Bryllite Game Client!" << std::endl ;
	std::cout << "Client Thread Count: " << thread_count << std::endl ;
	std::cout << "Server Port: " << vm["server-port"].as<int>() << std::endl ;
	std::cout << "config file: " << vm["config-file"].as<std::string>() << std::endl ;

	// service manager
	if ( ServiceManager::getInstance()->initialize() ) {

		// start service ( will be block here. main program runs in ServiceManager::main() )
		ServiceManager::getInstance()->start_service( argc, argv ) ;

		ServiceManager::releaseInstance() ;
	}

	// create client threads
	for( int i = 0 ; i < thread_count ; ++i ) {
		ClientEmulator* pClient = new ClientEmulator() ;
		if ( pClient ) {
			g_clients.push_back( pClient ) ;
			pClient->run( i, nullptr ) ;
		}
	}

	// wait for all thread join
	for( auto& t : g_clients ) {
		t->join() ;
	}

	std::cout << "Bryllite Game Client Terminated!" << std::endl ;

	return 0 ;
}

