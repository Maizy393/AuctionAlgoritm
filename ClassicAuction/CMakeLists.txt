cmake_minimum_required(VERSION 3.29)
project(AuctionAlgoritm)

set(CMAKE_CXX_STANDARD 17)

# 1) Собираем общий модуль auction_core
add_library(auction_core STATIC
        AuctionCore/auction_core.cpp
)
target_include_directories(auction_core
        PUBLIC ${CMAKE_SOURCE_DIR}    # чтобы найти auction_core.hpp
)

# 2) Собираем версию «min», линкуем в неё auction_core
add_executable(classic_auct_min
        classic_auct_min.cpp
)
target_link_libraries(classic_auct_min
        PRIVATE auction_core
)

# 3) Собираем версию «max», линкуем в неё auction_core
add_executable(classic_auct_max
        classic_auct_max.cpp
)
target_link_libraries(classic_auct_max
        PRIVATE auction_core
)

# 4) Собираем тест-раннер
add_executable(AuctionTest
        test.cpp
)

enable_testing()

# 5) Тест для минимизации
add_test(NAME AuctionTests_min
        COMMAND AuctionTest
        $<TARGET_FILE:classic_auct_min>
)

# 6) Тест для максимизации
add_test(NAME AuctionTests_max
        COMMAND AuctionTest
        $<TARGET_FILE:classic_auct_max>
)

# 7) Устанавливаем рабочую директорию, чтобы тесты читали test.txt из ${CMAKE_SOURCE_DIR}
set_tests_properties(AuctionTests_min PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_tests_properties(AuctionTests_max PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)