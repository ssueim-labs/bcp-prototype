# source files
set( source_files 
	version.hpp 
	common.cpp 
	common.hpp 
	singleton.hpp 
	lockable.hpp 
	string.cpp 
	string.hpp 
	log.cpp 
	log.hpp 
#	thread.hpp
#	thread.cpp
	compat/time.cpp
	compat/time.hpp
)

add_library( bryllite STATIC ${source_files} )
