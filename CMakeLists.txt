cmake_minimum_required(VERSION 3.27)
project(yandex_music_plugin C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(CJSON REQUIRED IMPORTED_TARGET libcjson)
pkg_check_modules(VLCPLUGIN REQUIRED IMPORTED_TARGET vlc-plugin)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(XML REQUIRED IMPORTED_TARGET libxml-2.0)

add_library(yandex_music_plugin SHARED src/library.c
        src/api.c
        src/api.h
        src/interceptor.c
        src/interceptor.h
        src/handler.c
        src/handler.h
        src/config.h
)

target_include_directories(yandex_music_plugin PUBLIC
        ${CJSON_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(yandex_music_plugin PkgConfig::CJSON)

target_include_directories(yandex_music_plugin PUBLIC
        ${CURL_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(yandex_music_plugin PkgConfig::CURL)

target_include_directories(yandex_music_plugin PUBLIC
        ${VLCPLUGIN_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(yandex_music_plugin PUBLIC ${VLCPLUGIN_CFLAGS_OTHER})
target_compile_options(yandex_music_plugin PUBLIC "-DMODULE_STRING=\"YandexMusic\"")
target_link_libraries(yandex_music_plugin PkgConfig::VLCPLUGIN)

target_include_directories(yandex_music_plugin PUBLIC
        ${XML_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(yandex_music_plugin PkgConfig::XML)

install(TARGETS yandex_music_plugin
        DESTINATION /usr/lib/x86_64-linux-gnu/vlc/plugins/misc  # Место установки библиотеки
)
