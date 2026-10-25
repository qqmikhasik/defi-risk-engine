include(FetchContent)

# GoogleTest: архив релиза, CMake сверяет контрольную сумму и отвергает подменённый файл.
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
    URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
    SYSTEM)
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

add_executable(morpho_position_test morpho_position_test.cpp)
target_link_libraries(morpho_position_test PRIVATE defi_risk_core GTest::gtest_main)
target_compile_options(morpho_position_test PRIVATE ${DRE_WARNINGS})

include(GoogleTest)
gtest_discover_tests(morpho_position_test)
