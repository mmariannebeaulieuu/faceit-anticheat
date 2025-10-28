cmake_minimum_required(VERSION 3.20)
project(WindowsAntiCheatPrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(anticheat-client
    src/Logger.cpp
    src/ProcessEnumerator.cpp
    src/SignatureScanner.cpp
    src/CertificateVerifier.cpp
    src/KernelInterface.cpp
    src/main.cpp
)

target_include_directories(anticheat-client PRIVATE include shared)

target_compile_definitions(anticheat-client PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

target_compile_options(anticheat-client PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive- /Zc:preprocessor>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror>
)

if (MSVC)
    target_link_options(anticheat-client PRIVATE /SUBSYSTEM:CONSOLE)
endif()

target_link_libraries(anticheat-client PRIVATE
    Wintrust
    Crypt32
)
