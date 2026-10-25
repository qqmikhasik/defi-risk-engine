/// @file
/// @brief Реализация LTV и health factor позиции Morpho.

#include "morpho_position.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace defi_risk {

namespace {

constexpr double kInfinity = std::numeric_limits<double>::infinity();

/// Проверяет входные величины; общая часть ltv() и health_factor().
void validate(const MorphoPosition& position) {
    const double values[] = {position.collateral, position.oracle_price, position.debt,
                             position.lltv};
    for (double value : values) {
        if (!std::isfinite(value) || value < 0.0) {
            throw std::invalid_argument("величины позиции должны быть конечными и неотрицательными");
        }
    }
    if (position.lltv <= 0.0 || position.lltv >= 1.0) {
        throw std::invalid_argument("LLTV должен лежать в интервале (0, 1)");
    }
}

}  // namespace

double ltv(const MorphoPosition& position) {
    validate(position);
    if (position.debt == 0.0) {
        return 0.0;
    }
    const double collateral_value = position.collateral * position.oracle_price;
    if (collateral_value == 0.0) {
        return kInfinity;
    }
    return position.debt / collateral_value;
}

double health_factor(const MorphoPosition& position) {
    validate(position);
    if (position.debt == 0.0) {
        return kInfinity;
    }
    return position.collateral * position.oracle_price * position.lltv / position.debt;
}

}  // namespace defi_risk
