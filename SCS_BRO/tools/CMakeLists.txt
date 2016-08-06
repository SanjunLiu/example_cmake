cmake_minimum_required(VERSION 2.8)
project (my_tools)

set(TEST_TOOLS_SRC
    img2bin
    )

#set(EXCUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#message(STATUS "CXXFLAG: " ${CMAKE_CXX_FLAGS})

##include directories
set (TOOLS_INCLUDE_DIR ${my_tools_SOURCE_DIR})

##link_directories
set (TOOLS_LINK_DIR ${PROJECT_SOURCE_DIR}/lib ${LIBRARY_OUTPUT_PATH})

##link_libraries
set (TOOLS_LINK_LIB ${TEST_LIB_NAME})
if (NOT BUILD_WINDOWS)
    set (TOOLS_LINK_LIB ${TOOLS_LINK_LIB} rt -lpthread)
endif()

INCLUDE_DIRECTORIES(${TOOLS_INCLUDE_DIR})
LINK_DIRECTORIES(${TOOLS_LINK_DIR})

##add_executes
foreach(tname ${TEST_TOOLS_SRC})
    ADD_EXECUTABLE(${tname} ${tname}.cpp)
    TARGET_LINK_LIBRARIES(${tname} ${TOOLS_LINK_LIB})
endforeach()

