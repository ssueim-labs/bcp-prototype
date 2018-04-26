#pragma once

#include <string>
#include <vector>

// namespace bryllite
namespace bryllite { 

// string format
std::string string_format( std::string format, ... );

// string parser
std::vector< std::string > string_parser( const std::string s, const std::string delimiter, bool remove_empty = false ) ;

// string replace
std::string string_replace( std::string& org, const char* before, const char* after );
std::string string_replace_copy( const std::string org, const char* before, const char* after ) ;


} // namespace bryllite