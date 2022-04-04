include_directories(${rtdb_includes})

add_library(rtdb STATIC rtdb_api.cpp)

add_library(multicast STATIC multicast.cpp)

add_executable(comm comm.cpp)
target_link_libraries(comm ${CMAKE_THREAD_LIBS_INIT} pthread rtdb multicast ${catkin_LIBRARIES})
