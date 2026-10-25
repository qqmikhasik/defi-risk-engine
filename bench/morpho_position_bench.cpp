/// @file
/// @brief Замер скорости LTV и health factor (FR-POS-01).

#include "morpho_position.hpp"

#include <benchmark/benchmark.h>

namespace {

// DoNotOptimize(position) не даёт компилятору вычислить результат один раз до цикла.
void BM_Ltv(benchmark::State& state) {
    defi_risk::MorphoPosition position{1000.0, 0.95, 760.0, 0.915};
    for (auto _ : state) {
        benchmark::DoNotOptimize(position);
        benchmark::DoNotOptimize(defi_risk::ltv(position));
    }
}
BENCHMARK(BM_Ltv);

void BM_HealthFactor(benchmark::State& state) {
    defi_risk::MorphoPosition position{1000.0, 0.95, 760.0, 0.915};
    for (auto _ : state) {
        benchmark::DoNotOptimize(position);
        benchmark::DoNotOptimize(defi_risk::health_factor(position));
    }
}
BENCHMARK(BM_HealthFactor);

}  // namespace
