set( source_files main.cpp service_manager.cpp service_manager.hpp )

add_executable( game-server ${source_files} )
target_link_libraries( game-server bryllite ${Boost_LIBRARIES} )
