cmake_minimum_required(VERSION 3.31)
project(svr)

find_package(Crow)

add_executable(${PROJECT_NAME}
src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow)