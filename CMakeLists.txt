cmake_minimum_required(VERSION 3.25)

project(defi-risk-engine
    VERSION 0.1.0
    DESCRIPTION "Риск ликвидации кредитной позиции с плечом в DeFi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Предупреждения для собственного кода; на зависимости не распространяются.
set(DRE_WARNINGS -Wall -Wextra -Wpedantic)

add_library(defi_risk_core src/morpho_position.cpp)
target_include_directories(defi_risk_core PUBLIC src)
target_compile_options(defi_risk_core PRIVATE ${DRE_WARNINGS})

add_executable(defi-risk-engine src/main.cpp)
target_link_libraries(defi-risk-engine PRIVATE defi_risk_core)
target_compile_options(defi-risk-engine PRIVATE ${DRE_WARNINGS})

enable_testing()
add_subdirectory(tests)
add_subdirectory(bench)
