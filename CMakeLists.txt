cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ZephyrApp)

target_sources(app PRIVATE 

src/main.c
src/custom_allocation.c
src/file_converter.c
)

target_include_directories(app PRIVATE src)