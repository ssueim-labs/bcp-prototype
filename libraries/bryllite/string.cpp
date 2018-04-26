/*
 @File: string.cpp
 @Desc: string utils
 @Author:okseok.kim@gmail.com
 */

#include <iostream>
#include <string.h>
#include <stdarg.h>
#include "string.hpp"

// namespace bryllite
namespace bryllite { 


// string format
std::string string_format( std::string format, ... )
{
	int size = ( format.size() * 2 ) + 1;
	std::string str;
	str.resize( size );

	va_list ap;

	va_start( ap, format );
	int len = vsnprintf( ( char* )str.data(), size, format.c_str(), ap );
	if( len >= size )
	{
		size = len + 1;
		str.resize( size );
		len = vsnprintf( ( char* )str.data(), size, format.c_str(), ap );
	}
	va_end( ap );

	return str;
}

// string parser
std::vector< std::string > string_parser( const std::string str, const std::string delimiter, bool remove_empty )
{
	std::vector< std::string > tokens ;
	size_t prev = 0, pos = 0 ;

	do {
		pos = str.find( delimiter, prev ) ;
		if ( pos == std::string::npos ) 
			pos = str.length() ;
		
		std::string token = str.substr( prev, pos - prev ) ;

		if ( !remove_empty || !token.empty() )
			tokens.push_back( token.empty()?"":token ) ;

		prev = pos + delimiter.length() ;
	} while( pos < str.length() && prev < str.length() ) ;

	return tokens ;
}

// string replace
std::string string_replace( std::string& str, const char* before, const char* after )
{
	size_t pos = 0, len = strlen( before ) ;
	while( ( pos = str.find( before, pos ) ) != std::string::npos ) {
		str.replace( pos, len, after ) ;
	};

	return str ;
}

std::string string_replace_copy( const std::string str, const char* before, const char* after )
{
	std::string copy = str ;
	return string_replace( copy, before, after ) ;
}




} // namespace bryllite
