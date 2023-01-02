cmake_minimum_required(VERSION 3.15)
project(supervolleyball2)

find_package(raylib 3.0 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost 1.55 REQUIRED COMPONENTS system serialization)
find_package(Threads REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})
set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} game.cpp network.cpp input.cpp)
add_executable(server server.cpp network.cpp input.cpp)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
  target_compile_options(server PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
  target_link_libraries(server "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_link_libraries(${PROJECT_NAME} raylib ${BOOST_LIBRARIES} ${GLFW3_LIBRARY} m ${CMAKE_DL_LIBS} Threads::Threads Boost::serialization)

target_link_libraries(server raylib ${BOOST_LIBRARIES} ${GLFW3_LIBRARY} m ${CMAKE_DL_LIBS} Threads::Threads Boost::serialization)
