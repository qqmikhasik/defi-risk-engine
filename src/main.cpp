/// @file
/// @brief Консольная программа: LTV и health factor позиции Morpho по аргументам.

#include <charconv>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <system_error>

#include "morpho_position.hpp"

namespace {

constexpr const char* kUsage =
    "Использование: defi-risk-engine <залог PT> <цена PT> <долг USDC> <LLTV>\n"
    "Пример: defi-risk-engine 1000 0.95 760 0.915\n";

/// Разбирает число целиком; строку с лишними символами не принимает.
std::optional<double> parse_number(std::string_view text) {
    double value = 0.0;
    const char* const end = text.data() + text.size();
    const auto [stop, error] = std::from_chars(text.data(), end, value);
    if (error != std::errc{} || stop != end) {
        return std::nullopt;
    }
    return value;
}

}  // namespace

/// @brief Точка входа: печатает LTV и health factor позиции, заданной аргументами.
/// @return 0 при успехе, 1 при недопустимых величинах, 2 при неверных аргументах.
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << kUsage;
        return 2;
    }
    double values[4];
    for (int i = 0; i < 4; ++i) {
        const std::optional<double> value = parse_number(argv[i + 1]);
        if (!value) {
            std::cerr << "Не число: " << argv[i + 1] << '\n' << kUsage;
            return 2;
        }
        values[i] = *value;
    }
    const defi_risk::MorphoPosition position{values[0], values[1], values[2], values[3]};
    try {
        const double ltv = defi_risk::ltv(position);
        const double health_factor = defi_risk::health_factor(position);
        std::cout << "LTV: " << ltv << '\n' << "Health factor: " << health_factor << '\n';
    } catch (const std::invalid_argument& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
