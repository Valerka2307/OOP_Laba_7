# Google Tests для NPC Battle Simulation

## Структура тестов

### 1. test_npc_game.cpp
Основные unit-тесты для базовой функциональности:

#### NPCTest
- Тестирование создания Robber, Elf, Bear
- Проверка имени, позиции, типа
- Проверка методов is_robber(), is_elf(), is_bear()

#### NPCDistancesTest
- Robber: move distance = 10, kill distance = 10
- Elf: move distance = 10, kill distance = 50
- Bear: move distance = 10, kill distance = 10

#### NPCStateTest
- Изменение позиции (setX, setY)
- Изменение состояния жизни (setAlive)
- Изменение имени (setName)

#### FactoryTest
- Создание NPC через Factory
- Проверка типов и позиций
- Тестирование Invalid типов

#### GameMapTest
- Валидация позиций на карте (0-99)
- Проверка граничных значений
- Проверка invalid позиций

#### GameNPCManagementTest
- Добавление NPC на карту
- Инициализация 50 NPC
- Проверка invalid позиций

#### ConstantsTest
- MAP_WIDTH = 100
- MAP_HEIGHT = 100
- GAME_DURATION_SECONDS = 30
- INITIAL_NPC_COUNT = 50

#### BattleResultTest
- Проверка enum значений BattleResult

#### ObserverTest
- Создание ObserverLog
- Создание ObserverOut

#### NPCProximityTest
- Расчет расстояния между NPC
- Проверка метода is_close()

### 2. test_program.cpp
Интеграционные и системные тесты:

#### SimulationIntegrationTest
- Инициализация игры с 50 NPC
- Проверка валидности позиций при инициализации

#### BattleMechanicsTest
- Robber vs Elf → Defeat
- Elf vs Bear → Defeat
- Bear vs Robber → PeaceAndLove
- Robber vs Robber → MutualDestruction
- Elf vs Elf → PeaceAndLove
- Bear vs Bear → PeaceAndLove

#### NPCTypeIdentificationTest
- Проверка методов is_robber(), is_elf(), is_bear()
- Тестирование для каждого типа

#### MapBoundaryTest
- Все четыре угла карты
- Проверка out-of-bounds позиций

#### GameConfigTest
- Проверка констант конфигурации

#### NPCMovementConfigTest
- Проверка move distance для всех типов
- Проверка kill distance для всех типов

#### NPCLifecycleTest
- NPC рождается живым (isAlive = true)
- NPC может умереть (setAlive = false)
- NPC может быть "воскрешен"

#### NPCPositionTest
- Начальная позиция
- Изменение позиции
- Движение за границы (проверка логики)

## Запуск тестов

### Все тесты:
```bash
cd build
cmake --build . --config Debug
ctest
```

### Конкретный тест:
```bash
ctest -R NPCGameTests
ctest -R ProgramTests
ctest -R FightLogicTests
```

### С подробным выводом:
```bash
ctest -V
ctest -R NPCGameTests -V
```

## Структура CMakeLists.txt

В CMakeLists.txt настроены следующие тесты:
1. **npc_game_tests** - unit-тесты NPC и Game
2. **test_program** - интеграционные тесты
3. **input_tests** - тесты ввода
4. **npc_factory_tests** - тесты Factory
5. **output_tests** - тесты вывода
6. **fight_logic_tests** - тесты боевой логики

Все тесты используют Google Test Framework (gtest) версии 1.15.0.
