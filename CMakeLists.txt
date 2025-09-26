cmake_minimum_required(VERSION 3.10)
project(GamblingTaxCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(gambling-calc
    src/main.cpp
    src/ConsoleInterface.cpp
    src/GamblingSession.cpp
    src/SessionDatabase.cpp
    src/TaxCalculator.cpp
    src/TaxRulesConfig.cpp
    src/UserProfile.cpp
)

if(WIN32)
    target_link_libraries(gambling-calc)
else()
    target_link_libraries(gambling-calc stdc++fs)
endif()

# Copy config directory to build directory
add_custom_command(TARGET gambling-calc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:gambling-calc>/config
    COMMENT "Copying config files to build directory"
)