#pragma once

#include <string>

// namespace bryllite
namespace bryllite {

// bryllite error code
enum bryllite_err_code {
	E_SUCCESS = 0,
	E_FAILED,

	E_END
};

// bryllite exception class
class exception 
{
public:
	exception() {
		m_errCode = E_SUCCESS ;
	} ;
	exception( int errCode ) {
		m_errCode = errCode ;
	} ;
	exception( const char* errMessage ) {
		m_errMessage = errMessage ;
	};
	exception( int errCode, const char* errMessage ) {
		m_errCode = errCode ;
		m_errMessage = errMessage ;
	};

	virtual ~exception() {} ;

	virtual int get_error_code( void ) { return m_errCode ; } ;
	virtual std::string get_error_message( void ) { return m_errMessage ; } ;

protected:
	int m_errCode ;				// error code
	std::string m_errMessage ;	// error message
};

} // namespace bryllite