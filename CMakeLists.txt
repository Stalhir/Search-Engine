cmake_minimum_required(VERSION 3.20)
project(Search_Engine)
set(PostgreSQL_ADDITIONAL_VERSIONS "17")

set(BOOST_ROOT "C:/Users/askoy/Desktop/search engine/lib/boost")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

find_package(Boost REQUIRED)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

if(WIN32)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

add_subdirectory(lib/libpqxx libpqxx-build)
add_executable(Search_Engine main.cpp src/Crowler/INI_parser/INI_parser.cpp)

target_link_libraries(Search_Engine PRIVATE Boost::headers)
target_compile_features(Search_Engine PRIVATE cxx_std_17)
target_link_libraries(Search_Engine PRIVATE pqxx)

if(WIN32)
    target_link_libraries(Search_Engine PRIVATE ws2_32 wsock32)
endif()