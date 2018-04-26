#include <string>

#include "time.hpp"
#include "bryllite/string.hpp"


// namespace bryllite
namespace bryllite {

// date code
// e.g, "2018-04-26"
std::string get_date_code( const char* date_format, time_t shift_in_sec ) 
{
	time_t today ;
	time( &today ) ;
	today += shift_in_sec ;
	struct tm* now = localtime( &today ) ;

	int year = 1900 + now->tm_year ;
	int month = 1 + now->tm_mon ;
	int day = now->tm_mday ;

	std::string dateCode = date_format ;
	string_replace( dateCode, "%y", string_format( "%04d", year ).c_str() ) ;
	string_replace( dateCode, "%m", string_format( "%02d", month ).c_str() );
	string_replace( dateCode, "%d", string_format( "%02d", day ).c_str() );

	return dateCode ;
};

// time code
// e.g, "05:28:30"
std::string get_time_code( const char* time_format, time_t shift_in_sec ) 
{
	time_t today;
	time( &today );
	today += shift_in_sec ;
	struct tm* now = localtime( &today );

	int hour = now->tm_hour ;
	int min = now->tm_min ;
	int sec = now->tm_sec ;

	std::string timeCode = time_format;
	string_replace( timeCode, "%h", string_format( "%02d", hour ).c_str() );
	string_replace( timeCode, "%i", string_format( "%02d", min ).c_str() );
	string_replace( timeCode, "%s", string_format( "%02d", sec ).c_str() );

	return timeCode;
};


// date time code
// e.g, "2018-04-26 05:24:30"
std::string get_date_time_code( const char* date_format, const char* time_format, time_t shift_in_sec )
{
	return string_format( "%s %s", 
		get_date_code( date_format, shift_in_sec ).c_str(), 
		get_time_code( time_format, shift_in_sec ).c_str() ) ;
}


} // namespace bryllite 