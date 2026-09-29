# Схемы требований

**Проект:** Программа для управления риском ликвидации и хеджированием кредитной позиции
с плечом в DeFi.
**Версия:** 0.1, черновик от 28.09.2026. **Автор:** Михаил Швайков.

Схемы показывают требования `functional.md` с разных сторон: кто пользуется программой,
из каких функций она состоит, откуда идут данные, как принимается одно решение и как живёт
позиция. Схемы производны от текста: при расхождении прав `functional.md`. Номера
требований на схемах ведут в него.

Схемы записаны на языке Mermaid. Их рисуют GitHub и встроенный просмотр Markdown в VS Code
(Ctrl+Shift+V).

## 1. Кто и что делает с программой (use case)

Прямоугольники слева и справа: участники вне программы. Овалы: действия. Пунктир:
действие вне программы.

```mermaid
flowchart LR
    owner["Владелец позиции"]
    researcher["Исследователь<br/>(тот же человек, подбор параметров)"]

    subgraph program["Программа"]
        direction TB
        uc_cfg(["Задать настройки<br/>FR-MODE-05"])
        uc_bt(["Запустить бэктест<br/>FR-MODE-01"])
        uc_pt(["Запустить paper-trading<br/>FR-MODE-02"])
        uc_obs(["Наблюдать реальную позицию<br/>FR-MODE-03, FR-EXEC-04"])
        uc_stop(["Остановить по Ctrl+C<br/>FR-MODE-06"])
        uc_read(["Читать консоль, журнал, отчёт<br/>FR-OUT-02 … FR-OUT-05"])
        uc_data(["Заморозить набор исторических данных<br/>FR-DATA-04 … FR-DATA-06"])
    end

    uc_research(["Подобрать параметры хеджа<br/>research/, раздел 1.4"])
    uc_manual(["Исполнить рекомендацию вручную"])

    eth["Сеть Ethereum<br/>Morpho, Pendle, пулы"]
    arb["Сеть Arbitrum<br/>Pendle Boros"]
    dumps["Открытые выгрузки Boros"]

    owner --> uc_cfg
    owner --> uc_bt
    owner --> uc_pt
    owner --> uc_obs
    owner --> uc_stop
    owner --> uc_read
    owner -.-> uc_manual
    researcher --> uc_research
    uc_research -.->|"запускает много раз"| uc_bt

    uc_data --> eth
    uc_data --> arb
    uc_data --> dumps
    uc_pt --> eth
    uc_pt --> arb
    uc_obs --> eth
    uc_obs --> arb
    uc_manual -.-> eth
    uc_manual -.-> arb
```

## 2. Дерево функций

Все 68 требований по группам. Серые с пунктирной рамкой: объём «продукт», в учебный
проект не входят.

```mermaid
flowchart TB
    root(["Программа управления риском<br/>позиции с плечом"])

    subgraph MODE["MODE: режимы и запуск"]
        direction TB
        m01["FR-MODE-01 бэктест"] ~~~ m02["FR-MODE-02 paper-trading"] ~~~ m03["FR-MODE-03 наблюдение"] ~~~ m04["FR-MODE-04 одна логика во всех режимах"] ~~~ m05["FR-MODE-05 настройки"] ~~~ m06["FR-MODE-06 остановка по Ctrl+C"] ~~~ m07["FR-MODE-07 обрыв связи"] ~~~ m08["FR-MODE-08 восстановление после перезапуска"] ~~~ m09["FR-MODE-09 работа как сервис"]
    end

    subgraph POS["POS: позиция"]
        direction TB
        p01["FR-POS-01 модель петли"] ~~~ p02["FR-POS-02 хедж YT"] ~~~ p03["FR-POS-03 хедж Boros"] ~~~ p04["FR-POS-04 состав позиции"] ~~~ p05["FR-POS-05 открытие"] ~~~ p06["FR-POS-06 конец позиции"] ~~~ p09["FR-POS-09 перекат хеджа Boros"] ~~~ p11["FR-POS-11 выход в погашение"] ~~~ p07["FR-POS-07 несколько позиций"] ~~~ p08["FR-POS-08 выбор рынка и перекат PT"] ~~~ p10["FR-POS-10 объём ликвидации по ликвидаторам"]
    end

    subgraph DATA["DATA: данные"]
        direction TB
        d01["FR-DATA-01 источник истины: сеть"] ~~~ d02["FR-DATA-02 частота данных"] ~~~ d03["FR-DATA-03 порядок событий двух сетей"] ~~~ d04["FR-DATA-04 история Ethereum"] ~~~ d05["FR-DATA-05 история Boros"] ~~~ d06["FR-DATA-06 замороженные наборы"] ~~~ d07["FR-DATA-07 качество данных"] ~~~ d08["FR-DATA-08 несколько узлов"] ~~~ d09["FR-DATA-09 другие сети и площадки"]
    end

    subgraph RISK["RISK: расчёт риска"]
        direction TB
        r01["FR-RISK-01 цена ликвидации у оракула"] ~~~ r02["FR-RISK-02 виды оракула"] ~~~ r03["FR-RISK-03 отказ на незнакомом оракуле"] ~~~ r04["FR-RISK-04 сверка формулы с сетью"] ~~~ r05["FR-RISK-05 LTV, health, запас"] ~~~ r06["FR-RISK-06 прогноз цены оракула"] ~~~ r07["FR-RISK-07 вероятность ликвидации, L_U на TWAP"] ~~~ r08["FR-RISK-08 нижняя граница L_L"] ~~~ r09["FR-RISK-09 риск ноги Boros"] ~~~ r10["FR-RISK-10 базисный риск"] ~~~ r11["FR-RISK-11 стоимость выхода"] ~~~ r12["FR-RISK-12 залог хеджа в долларах"] ~~~ r15["FR-RISK-15 L_U на линейном дисконте"] ~~~ r16["FR-RISK-16 таймер мета-оракула"] ~~~ r13["FR-RISK-13 гибридные оракулы"] ~~~ r14["FR-RISK-14 качество модели"] ~~~ r17["FR-RISK-17 кластеры сделок"]
    end

    subgraph DEC["DEC: решения"]
        direction TB
        c01["FR-DEC-01 решения по петле"] ~~~ c02["FR-DEC-02 решения по хеджу"] ~~~ c03["FR-DEC-03 переброска капитала"] ~~~ c04["FR-DEC-04 порядок приоритетов"] ~~~ c05["FR-DEC-05 объяснение решения"] ~~~ c06["FR-DEC-06 быстрые мосты"] ~~~ c07["FR-DEC-07 защитные решения"]
    end

    subgraph EXEC["EXEC: исполнение"]
        direction TB
        e01["FR-EXEC-01 интерфейс исполнения"] ~~~ e02["FR-EXEC-02 имитация исполнения"] ~~~ e03["FR-EXEC-03 цена выхода"] ~~~ e04["FR-EXEC-04 рекомендации"] ~~~ e05["FR-EXEC-05 реальное исполнение"] ~~~ e06["FR-EXEC-06 секреты"] ~~~ e07["FR-EXEC-07 ограничители"] ~~~ e08["FR-EXEC-08 сверка после исполнения"]
    end

    subgraph OUT["OUT: вывод"]
        direction TB
        out01["FR-OUT-01 легенда метрик"] ~~~ out02["FR-OUT-02 консоль"] ~~~ out03["FR-OUT-03 журнал решений"] ~~~ out04["FR-OUT-04 отчёт бэктеста"] ~~~ out05["FR-OUT-05 показатели хеджа"] ~~~ out06["FR-OUT-06 оповещения вне консоли"] ~~~ out07["FR-OUT-07 метрики и наблюдаемость"]
    end

    root --> MODE
    root --> POS
    root --> DATA
    root --> RISK
    root --> DEC
    root --> EXEC
    root --> OUT

    classDef product fill:#eeeeee,stroke:#888888,stroke-dasharray:4 3,color:#555555
    class m08,m09,p07,p08,p10,d09,r13,r14,r17,c06,c07,e05,e06,e07,e08,out06,out07 product
```

## 3. Потоки данных

Прямоугольники: внешние источники. Скруглённые: шаги обработки. Цилиндры: хранилища.

```mermaid
flowchart LR
    eth["Узлы Ethereum"]
    arb["Узел Arbitrum"]
    dumps["Выгрузки Boros"]
    sdk["Pendle Hosted SDK"]
    cfg["Файл настроек<br/>FR-MODE-05"]

    load("Загрузка и сверка с сетью<br/>FR-DATA-01, FR-DATA-04, FR-DATA-05")
    frozen[("Замороженные наборы<br/>FR-DATA-06")]
    feed("Лента событий двух сетей<br/>FR-DATA-02, FR-DATA-03, FR-DATA-07")
    risk("Расчёт риска<br/>FR-RISK-01 … FR-RISK-16")
    dec("Решения<br/>FR-DEC-01 … FR-DEC-05")
    exec("Исполнение<br/>FR-EXEC-01 … FR-EXEC-04")
    out("Вывод<br/>FR-OUT-01 … FR-OUT-05")
    journal[("Журнал решений<br/>FR-OUT-03")]
    console["Консоль и отчёт"]
    research["Исследование<br/>research/"]

    eth -->|"события, price(), состояние Morpho"| load
    arb -->|"события Boros, счёт и маржа"| load
    dumps -->|"сделки, стакан, расчёты ставки"| load
    load -->|"бэктест: сохранить"| frozen
    frozen -->|"бэктест: проиграть"| feed
    load -->|"живые режимы"| feed
    feed -->|"состояние позиции и рынков"| risk
    cfg --> risk
    cfg --> dec
    risk -->|"L_U, L_L, запас, риск Boros"| dec
    dec -->|"приказы"| exec
    exec -.->|"котировки пулов, eth_call"| eth
    sdk -.->|"котировка для сравнения, paper-trading"| exec
    exec -->|"новое состояние виртуальной позиции"| feed
    dec -->|"решение с причиной"| out
    exec -->|"затраты, рекомендация"| out
    out --> journal
    out --> console
    journal --> research
```

## 4. Один цикл решения (swimlane)

Дорожки: части программы. Одна и та же последовательность работает во всех режимах
(FR-MODE-04), различается только последний шаг.

```mermaid
sequenceDiagram
    autonumber
    participant N as Сеть или набор данных
    participant D as Источник данных
    participant R as Расчёт риска
    participant C as Решения
    participant E as Интерфейс исполнения
    participant O as Журнал и консоль

    N->>D: блок Ethereum, сделка или расчёт Boros (FR-DATA-02)
    D->>D: событие встаёт в ленту (FR-DATA-03)
    alt данные ненадёжны (FR-DATA-07, FR-MODE-07)
        D->>O: предупреждение с причиной, решений нет
    else данные в порядке
        D->>R: состояние позиции и рынков
        R->>R: LTV, health, запас, границы L_U и L_L, риск Boros
        R->>R: сверка цены оракула с price() (FR-RISK-04)
        R->>C: оценка риска
        C->>C: приоритет: защита, выход, подстройка, держать (FR-DEC-04)
        C->>O: решение с причиной и числами (FR-DEC-05, FR-OUT-03)
        opt решение не «держать»
            C->>E: приказы
            alt бэктест или paper-trading
                E->>E: имитация: комиссии, проскальзывание, газ (FR-EXEC-02)
                E->>D: новое состояние виртуальной позиции
            else наблюдение
                E->>O: рекомендация человеку (FR-EXEC-04)
            end
        end
    end
```

## 5. Состояния позиции

Позиция, пока открыта, стоит в состоянии «Держать». Каждое действие после исполнения
возвращает её туда. Если поводов для действий несколько, выбор идёт по FR-DEC-04.

```mermaid
stateDiagram-v2
    direction TB

    state "Открытие (FR-POS-05)" as opening
    state "Держать" as hold
    state "Сократить плечо (FR-DEC-01)" as reduce
    state "Нарастить плечо (FR-DEC-01)" as increase
    state "Подогнать хедж (FR-DEC-02)" as rehedge
    state "Довнести или вывести маржу Boros (FR-DEC-02)" as margin
    state "Перекатить хедж Boros (FR-POS-09)" as roll
    state "Перебросить капитал (FR-DEC-03)" as bridge
    state "Нет решений, данные ненадёжны (FR-DATA-07)" as nodata
    state "Ликвидация хеджа Boros (FR-POS-06)" as hedgeliq
    state "Досрочное закрытие (FR-DEC-01)" as early
    state "Выход в погашение выкупом (FR-POS-11)" as maturity
    state "Ликвидация петли (FR-POS-06)" as loopliq
    state "Закрыта" as closed

    [*] --> opening : бэктест или paper-trading
    [*] --> hold : наблюдение, позиция прочитана из сети
    opening --> hold : план открытия исполнен

    hold --> reduce : LTV выше L_U
    reduce --> hold
    hold --> increase : LTV ниже L_L и окупается
    increase --> hold
    hold --> rehedge : подгонка выгодна
    rehedge --> hold
    hold --> margin : health хеджа вне границ
    margin --> hold
    hold --> roll : τ_B/τ ниже порога
    roll --> hold
    hold --> bridge : одной ноге грозит ликвидация
    bridge --> hold

    hold --> nodata : отставание, реорганизация, расхождение
    nodata --> hold : данные восстановлены
    hold --> hedgeliq : Net Balance ниже MM
    hedgeliq --> hold : петля живёт без хеджа

    hold --> early : заём дороже доходности PT
    hold --> maturity : дата погашения PT
    hold --> loopliq : LTV по оракулу выше LLTV
    early --> closed
    maturity --> closed
    loopliq --> closed
    closed --> [*]

    note right of loopliq
        Что делать с хеджем после
        ликвидации петли, требования
        пока не говорят.
    end note
```

## 6. Требования по шагам процесса

Таблица связывает требования с шагами работы программы. Шаги S1–S3 выполняются при
запуске, S4–S9 повторяются на каждом событии; после исполнения (S8) меняется состояние
позиции (S5). Каждое требование стоит ровно в одной строке столбца «Требования»; если оно
работает ещё на каком-то шаге, оно указано у того шага в последнем столбце.

По этой таблице, `functional.md` и `risks.md` ноутбук `risk-map.ipynb` собирает объёмную
схему `requirements-graph.html`: внизу шаги, над ними требования, выше риски, между ними
связи. Файл открывается в браузере, нужен интернет (библиотека three.js).

| Шаг | Название | Что происходит | Требования | Также работают на шаге |
|---|---|---|---|---|
| S1 | Запуск и настройки | Выбор режима, проверка настроек, подготовка журнала | FR-MODE-01, FR-MODE-02, FR-MODE-03, FR-MODE-05, FR-MODE-08, FR-MODE-09 | |
| S2 | Загрузка данных сетей | События и состояние контрактов Ethereum и Arbitrum, выгрузки Boros, сверка с сетью | FR-DATA-01, FR-DATA-04, FR-DATA-05, FR-DATA-08, FR-DATA-09 | |
| S3 | Замороженные наборы | Скачанное сохраняется набором с версией, бэктест проигрывает только его | FR-DATA-06 | |
| S4 | Лента событий | События двух сетей в одной ленте, проверка свежести и согласованности данных | FR-DATA-02, FR-DATA-03, FR-DATA-07, FR-MODE-07 | |
| S5 | Состояние позиции | Петля и хедж после события: PT в залоге, долг, маржа Boros | FR-POS-01, FR-POS-02, FR-POS-03, FR-POS-04, FR-POS-05, FR-POS-06, FR-POS-07, FR-POS-08, FR-POS-10, FR-MODE-04 | FR-POS-09 |
| S6 | Расчёт риска | LTV и запас по цене оракула, границы коридора, риск ноги Boros, стоимость выхода | FR-RISK-01, FR-RISK-02, FR-RISK-03, FR-RISK-04, FR-RISK-05, FR-RISK-06, FR-RISK-07, FR-RISK-08, FR-RISK-09, FR-RISK-10, FR-RISK-11, FR-RISK-12, FR-RISK-13, FR-RISK-14, FR-RISK-15, FR-RISK-16, FR-RISK-17 | FR-EXEC-03, FR-MODE-04 |
| S7 | Решения | Одно решение по приоритету: защита, выход, подстройка, держать | FR-DEC-01, FR-DEC-02, FR-DEC-03, FR-DEC-04, FR-DEC-05, FR-DEC-06, FR-DEC-07, FR-POS-09, FR-POS-11 | FR-MODE-04 |
| S8 | Исполнение | Приказы через интерфейс исполнения: имитация или рекомендация человеку | FR-EXEC-01, FR-EXEC-02, FR-EXEC-03, FR-EXEC-04, FR-EXEC-05, FR-EXEC-06, FR-EXEC-07, FR-EXEC-08 | FR-POS-05, FR-POS-11 |
| S9 | Вывод | Строка в консоль, запись в журнал, отчёт в конце бэктеста | FR-OUT-01, FR-OUT-02, FR-OUT-03, FR-OUT-04, FR-OUT-05, FR-OUT-06, FR-OUT-07, FR-MODE-06 | |

## 7. Риски и трассировка

- Карта рисков и реестр: `risks.md`.
- Матрица трассировки «риск → требования» и «требование → риски»: `traceability.md`.
  Её собирает ноутбук `risk-map.ipynb`.

## 8. Журнал изменений

| Дата | Версия | Изменение |
|---|---|---|
| 28.09.2026 | 0.1 | Первый черновик. |
