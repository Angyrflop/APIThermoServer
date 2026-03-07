cmake_minimum_required(VERSION 3.31)
#project(svr)
project(ipUtil)

#find_package(Crow)

add_executable(${PROJECT_NAME}
src/ip_utils.c
)

target_link_libraries(${PROJECT_NAME}) #PUBLIC Crow::Crow)