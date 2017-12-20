cmake_minimum_required(VERSION 2.8)


project(test_server)

set(SOURCES src/server.cpp)

# Поиск библиотек
find_library(WILTON_CORE_LIBRARY wilton_core)
find_library(WILTON_SERVER_LIBRARY wilton_server)
find_library(WILTON_LOGGING_LIBRARY wilton_logging)

# Задание флагов
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set ( CMAKE_C_FLAGS_DEBUG "-g -Og" CACHE INTERNAL "" )

# Задание директорий заголовочных файлов
set(WILTON_CORE_INCLUDE "../wilton/core/include")
set(WILTON_SERVER_INCLUDE "../wilton/modules/wilton_server/include")
set(WILTON_LOGGER_INCLUDE "../wilton/modules/wilton_logging/include")

# подключение директорий
include_directories("./" ${WILTON_CORE_INCLUDE} ${WILTON_SERVER_INCLUDE} ${WILTON_LOGGER_INCLUDE})

# сборка бинарника
add_executable(test_server ${SOURCES})

# Линковка
target_link_libraries(test_server ${WILTON_CORE_LIBRARY} ${WILTON_SERVER_LIBRARY} ${WILTON_LOGGING_LIBRARY})
