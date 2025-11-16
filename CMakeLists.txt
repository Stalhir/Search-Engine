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

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_LIBRARIES
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib"
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib"
)


add_subdirectory(lib/libpqxx libpqxx-build)
add_executable(Search_Engine main.cpp src/Initializer/INI_parser/INI_parser.cpp
        src/Crowler/HTTPClient/httpclient.cpp
        include/SafeQueue.h
        src/Crowler/ThreadPool/threadpool.cpp
        src/DataBase_Interfejs/DBInterfejs.cpp
        src/Initializer/initializer.cpp
        src/Crowler/Сrowler.cpp
)

target_include_directories(Search_Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(Search_Engine PRIVATE Boost::headers)
target_compile_features(Search_Engine PRIVATE cxx_std_17)
target_link_libraries(Search_Engine PRIVATE pqxx)
target_link_libraries(Search_Engine PRIVATE ${OPENSSL_LIBRARIES})
        if(WIN32)
    target_link_libraries(Search_Engine PRIVATE ws2_32 wsock32)
endif()