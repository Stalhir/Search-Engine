cmake_minimum_required(VERSION 3.18) 
project(Search_Engine LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ====================================================================
# 1. СИСТЕМНЫЕ ЗАВИСИМОСТИ (Boost, OpenSSL, libpqxx)
# ====================================================================

# Boost
find_package(Boost REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

# OpenSSL
find_package(OpenSSL REQUIRED)

# libpqxx 
find_package(libpqxx CONFIG REQUIRED) 

# ====================================================================
# 2. ВНУТРЕННИЕ/СТОРОННИЕ ЗАВИСИМОСТИ (pugixml через FetchContent)
# ====================================================================
include(FetchContent)

# pugixml
FetchContent_Declare(
    pugixml_external
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.14
)
FetchContent_MakeAvailable(pugixml_external)

# ====================================================================
# 3. КОМПИЛЯЦИЯ ИСПОЛНЯЕМОГО ФАЙЛА 
# ====================================================================

if(WIN32)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

add_executable(Search_Engine
    main.cpp
    src/Initializer/INI_parser/INI_parser.cpp
    src/Crowler/HTTPClient/httpclient.cpp
    include/SafeQueue.h
    src/Crowler/ThreadPool/threadpool.cpp
    src/DataBase_Interfejs/DataBase.cpp
    src/Initializer/initializer.cpp
    src/Crowler/Сrowler.cpp
    src/Crowler/Indexer/Indexer.cpp
    src/Search_Engine/SearchEngine.cpp
    src/DataBase_Interfejs/DataBase.cpp
    include/SYMBOLS.h
    src/Search_Engine/HTTPServer/httpserver.cpp
    include/httpserver.h
)

target_include_directories(Search_Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Search_Engine PRIVATE cxx_std_17)


# ====================================================================
# 4. ЛИНКОВКА 
# ====================================================================

target_link_libraries(Search_Engine PRIVATE
    # Boost 
    Boost::boost # <--- ИСПРАВЛЕНО: Используем общую цель Boost::boost
    
    # libpqxx 
    libpqxx::pqxx 

    # pugixml 
    pugixml

    # OpenSSL 
    OpenSSL::SSL
    OpenSSL::Crypto

    # WIN32 специфичные библиотеки
    ws2_32
    wsock32
)