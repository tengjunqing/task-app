# 1. 指定CMake的最低版本要求
# 3.10是一个现代且兼容性好的版本
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称
project(SimpleCPPApp LANGUAGES CXX)

# 3. 【新增关键配置】设置 C++ 标准为 C++11
# CMAKE_CXX_STANDARD_REQUIRED 设置为 ON 意味着如果编译器不支持 C++11 就会报错
# CMAKE_CXX_EXTENSIONS 设置为 OFF 意味着严格遵循 C++11 标准，不启用 GNU 扩展
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 对应于 -std=c++11 而非 -std=gnu++11

# 4. 添加可执行文件
add_executable(main_program main.cpp)