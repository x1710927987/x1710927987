cmake_minimum_required(VERSION 3.10)
project(cpp_exam)  # 项目名与二进制文件名一致，符合要求

# 设置 C++ 标准为 C++14（原文件已有，保留）
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求符合 C++14 标准，避免兼容性问题

# ---------------------- 新增：编译目标配置 ----------------------
# 指定源文件（main.cpp 为主程序，Song.cpp 为 Song 类实现）
add_executable(${PROJECT_NAME}  # 生成的二进制文件名 = 项目名（cpp_exam）
    main.cpp
    Song.cpp
)

# 为目标添加头文件搜索路径（确保编译器能找到 Song.h）
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}  # 搜索当前项目根目录
)
# ----------------------------------------------------------------

# 启用测试（原文件已有，保留）
enable_testing()

# 添加测试用例（原文件已有，保留）
set(TEST_CASES 1 2 3 4 5 6 7 8)
foreach(TEST_NUM ${TEST_CASES})
    add_test(
        NAME test_${TEST_NUM}
        COMMAND ${CMAKE_COMMAND} -E compare_files
            ${CMAKE_CURRENT_SOURCE_DIR}/testcases/expected_${TEST_NUM}.txt
            ${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NUM}.txt
    )
    
    # 在测试前运行程序生成输出
    add_test(
        NAME run_test_${TEST_NUM}
        COMMAND sh -c "$<TARGET_FILE:${PROJECT_NAME}> < \"${CMAKE_CURRENT_SOURCE_DIR}/testcases/input_${TEST_NUM}.txt\" > \"${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NUM}.txt\" 2>&1"
    )
    
    # 设置测试依赖
    set_tests_properties(test_${TEST_NUM} PROPERTIES DEPENDS run_test_${TEST_NUM})
endforeach()

# 设置测试属性（原文件已有，保留）
foreach(TEST_NUM ${TEST_CASES})
    set_tests_properties(run_test_${TEST_NUM} PROPERTIES TIMEOUT 10)
    set_tests_properties(test_${TEST_NUM} PROPERTIES TIMEOUT 5)
endforeach()
