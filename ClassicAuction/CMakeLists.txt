cmake_minimum_required(VERSION 3.29)
project(AuctionAlgoritm)

set(CMAKE_CXX_STANDARD 17)

# Собираем две версии алгоритма
add_executable(classic_auct_min classic_auct_min.cpp)
add_executable(classic_auct_max classic_auct_max.cpp)

# Собираем тест-раннер
add_executable(AuctionTest test.cpp)

enable_testing()

# Тест для минимизации
add_test(NAME AuctionTests_min
        COMMAND AuctionTest
        $<TARGET_FILE:classic_auct_min>
)

# Тест для максимизации
add_test(NAME AuctionTests_max
        COMMAND AuctionTest
        $<TARGET_FILE:classic_auct_max>
)

# Устанавливаем рабочую директорию для тестов
set_tests_properties(AuctionTests_min PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(AuctionTests_max PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)