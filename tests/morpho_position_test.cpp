/// @file
/// @brief Тесты LTV и health factor (FR-POS-01, критерий К2).

#include "morpho_position.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

namespace {

using defi_risk::MorphoPosition;

constexpr double kInfinity = std::numeric_limits<double>::infinity();

// Посчитано вручную: стоимость залога 1000 · 0,95 = 950;
// LTV = 760 / 950 = 0,8; health factor = 950 · 0,915 / 760 = 1,14375.
TEST(MorphoPosition, MatchesHandCalculation) {
    const MorphoPosition position{1000.0, 0.95, 760.0, 0.915};
    EXPECT_DOUBLE_EQ(defi_risk::ltv(position), 0.8);
    EXPECT_DOUBLE_EQ(defi_risk::health_factor(position), 1.14375);
}

// Долг 950 · 0,915 = 869,25 ровно на границе ликвидации: LTV = LLTV, health factor = 1.
TEST(MorphoPosition, HealthFactorIsOneAtLiquidationThreshold) {
    const MorphoPosition position{1000.0, 0.95, 869.25, 0.915};
    EXPECT_DOUBLE_EQ(defi_risk::ltv(position), 0.915);
    EXPECT_DOUBLE_EQ(defi_risk::health_factor(position), 1.0);
}

TEST(MorphoPosition, NoDebt) {
    const MorphoPosition position{1000.0, 0.95, 0.0, 0.915};
    EXPECT_EQ(defi_risk::ltv(position), 0.0);
    EXPECT_EQ(defi_risk::health_factor(position), kInfinity);
}

TEST(MorphoPosition, DebtWithoutCollateral) {
    const MorphoPosition position{0.0, 0.95, 760.0, 0.915};
    EXPECT_EQ(defi_risk::ltv(position), kInfinity);
    EXPECT_EQ(defi_risk::health_factor(position), 0.0);
}

TEST(MorphoPosition, RejectsInvalidInput) {
    EXPECT_THROW(defi_risk::ltv({1000.0, 0.95, -1.0, 0.915}), std::invalid_argument);
    EXPECT_THROW(defi_risk::ltv({kInfinity, 0.95, 760.0, 0.915}), std::invalid_argument);
    EXPECT_THROW(defi_risk::health_factor({1000.0, 0.95, 760.0, 0.0}), std::invalid_argument);
    EXPECT_THROW(defi_risk::health_factor({1000.0, 0.95, 760.0, 1.0}), std::invalid_argument);
}

}  // namespace
