/// @file
/// @brief LTV и health factor позиции на рынке Morpho Blue (FR-POS-01).

#pragma once

/// @brief Код проекта defi-risk-engine.
namespace defi_risk {

/// @brief Состояние позиции на рынке Morpho, достаточное для LTV и health factor.
///
/// Величины в единицах токенов, с плавающей точкой. Точный целочисленный расчёт
/// долга по долям займа (FR-POS-01, критерий К1) сюда не входит.
struct MorphoPosition {
    double collateral;    ///< Залог: количество PT.
    double oracle_price;  ///< Цена одного PT в токене займа по оракулу рынка.
    double debt;          ///< Долг в токене займа (USDC).
    double lltv;          ///< Порог ликвидации рынка (LLTV), доля из (0, 1).
};

/// @brief LTV позиции: долг / (залог · цена оракула).
///
/// Без долга LTV равен 0. При долге и нулевой стоимости залога LTV равен +∞.
/// @throws std::invalid_argument если величина отрицательна, не конечна
///         или LLTV вне (0, 1).
double ltv(const MorphoPosition& position);

/// @brief Health factor позиции: залог · цена оракула · LLTV / долг.
///
/// Позиция ликвидируема, когда health factor меньше 1: долг превысил
/// залог · цена · LLTV. Без долга health factor равен +∞.
/// @throws std::invalid_argument при тех же условиях, что ltv().
double health_factor(const MorphoPosition& position);

}  // namespace defi_risk
