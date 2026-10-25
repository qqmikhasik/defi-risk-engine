include(FetchContent)

# Google Benchmark: архив релиза с проверкой контрольной суммы, как GoogleTest в tests/.
FetchContent_Declare(benchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.tar.gz
    URL_HASH SHA256=9631341c82bac4a288bef951f8b26b41f69021794184ece969f8473977eaa340
    SYSTEM)
# Собственные тесты библиотеки не нужны: с ними она скачивала бы GoogleTest ещё раз.
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
# Её -Werror превращал бы новые предупреждения компилятора в чужом коде в падение сборки.
set(BENCHMARK_ENABLE_WERROR OFF)
FetchContent_MakeAvailable(benchmark)

add_executable(morpho_position_bench morpho_position_bench.cpp)
target_link_libraries(morpho_position_bench PRIVATE defi_risk_core benchmark::benchmark_main)
target_compile_options(morpho_position_bench PRIVATE ${DRE_WARNINGS})
