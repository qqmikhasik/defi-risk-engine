# defi-risk-engine

[![CI](https://github.com/qqmikhasik/defi-risk-engine/actions/workflows/ci.yml/badge.svg)](https://github.com/qqmikhasik/defi-risk-engine/actions/workflows/ci.yml)

Проект курса «Технологии в финансах», мехмат МГУ, осень 2026.

## Что делает программа

Консольная программа на C++ для Linux. Она следит за кредитной позицией с плечом в сети
Ethereum: PT-токены Pendle лежат залогом в кредитном протоколе Morpho или Aave v3, под
этот залог взят заём в USDC. Программа считает, насколько позиция близка к ликвидации,
и на каждом событии сети решает: держать позицию, уменьшить заём, увеличить заём или
закрыть позицию. Работает на исторических данных (backtest), на новых данных сети с
виртуальной позицией (paper-trading) и с реальной позицией пользователя (live).
Транзакций не подписывает и не отправляет. Подробно:
[требования](https://github.com/qqmikhasik/defi-risk-engine/tree/main/docs/requirements).

Сейчас реализован первый расчёт: LTV и health factor позиции на Morpho по цене оракула
рынка (требование FR-POS-01).

```
$ ./build/defi-risk-engine 1000 0.95 760 0.915
LTV: 0.8
Health factor: 1.14375
```

Аргументы: залог (количество PT), цена PT по оракулу, долг в USDC, порог ликвидации
рынка LLTV. Позиция ликвидируема, когда health factor меньше 1.

## Сборка

Нужны Linux (подойдёт WSL), CMake 3.25 или новее и компилятор с поддержкой C++20
(g++ 13 или новее). При первой конфигурации нужен интернет: CMake скачивает GoogleTest
и Google Benchmark закреплённых версий и сверяет их контрольные суммы.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Тесты и бенчмарк

```sh
ctest --test-dir build --output-on-failure
./build/bench/morpho_position_bench
```

CI запускает сборку, тесты и бенчмарк при каждом push; результаты видны в журнале задания.

## Документация

Документация по коду собирается Doxygen и публикуется CI при каждом push в `main`:
https://qqmikhasik.github.io/defi-risk-engine/

## Структура

| Путь | Что лежит |
|---|---|
| `src/` | код программы |
| `tests/` | модульные тесты (GoogleTest) |
| `bench/` | бенчмарк (Google Benchmark) |
| `docs/requirements/` | требования, реестр рисков |
| `docs/adr/` | архитектурные решения |

Автор: Михаил Швайков.
