# include sub directories
include_directories(${PROJECT_SOURCE_DIR}/include/base)
include_directories(${PROJECT_SOURCE_DIR}/include/config)
include_directories(${PROJECT_SOURCE_DIR}/include/log)
include_directories(${PROJECT_SOURCE_DIR}/include/network)
include_directories(${PROJECT_SOURCE_DIR}/include/utility)
include_directories(${PROJECT_SOURCE_DIR}/include/data)
include_directories(${PROJECT_SOURCE_DIR}/include/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/include/thirdparty/rapidjson)

# add compile tree
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(network)
add_subdirectory(utility)
add_subdirectory(data)

# all source files
file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

# find_library(CURL
#   NAMES curl
#   PATHS ${PROJECT_SOURCE_DIR}/lib
# )
# set(ARM OFF) 

# if(${ARM})
#   link_directories(${PROJECT_SOURCE_DIR}/lib/arm/libcurl)
#   link_directories(${PROJECT_SOURCE_DIR}/lib/arm/openssl)
# else()
#   link_directories(${PROJECT_SOURCE_DIR}/lib/x86/libcurl)
#   link_directories(${PROJECT_SOURCE_DIR}/lib/x86/openssl)
# endif()


# create the executable
set(EXECUTABLE_NAME KewellDaemon)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/output)

# execute file 
add_executable(${EXECUTABLE_NAME} ${ALL_SOURCE})

target_link_libraries(${EXECUTABLE_NAME} pthread)
target_link_libraries(${EXECUTABLE_NAME} rt)
# target_link_libraries(${EXECUTABLE_NAME} crypto)