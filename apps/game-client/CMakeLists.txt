set( source_files main.cpp service_manager.cpp service_manager.hpp )

add_executable( game-client ${source_files} )
target_link_libraries( game-client bryllite ${Boost_LIBRARIES} )
