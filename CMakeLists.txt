# 指定CMake的最低版本要求 (3.10是一个现代且兼容性好的版本)
cmake_minimum_required(VERSION 2.8)

# 定义项目名称
# LANGUAGES CXX 表示这是一个C++项目
project(SimpleCPPApp LANGUAGES CXX)

# 添加可执行文件
# 语法：add_executable(最终程序的名称 源文件列表)
add_executable(system main.cpp)
