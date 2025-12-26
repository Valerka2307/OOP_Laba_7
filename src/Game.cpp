#include "Game.hpp"
#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Visitor.hpp"
#include "Observer.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <random>
#include <iomanip>
#include <cmath>

Game::Game() : running(false) {
}

Game::~Game() {
    stopGame();
}

void Game::addObserver(std::shared_ptr<Observer> observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

bool Game::isValidPosition(int x, int y) const {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}

void Game::addNPC(NPCType type, int x, int y, const std::string& name) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Error: Invalid position (" << x << ", " << y << 
                     "). Allowed range: 0-" << MAP_WIDTH-1 << ", 0-" << MAP_HEIGHT-1 << std::endl;
        return;
    }
    
    auto npc = factory.createNPC(type, x, y);
    if (npc) {
        npc->setName(name);
        // Subscribe all observers
        for (const auto& observer : observers) {
            npc->subscribe(observer);
        }
        
        std::lock_guard<std::shared_mutex> lock(npc_mutex);
        npcs.push_back(npc);
        
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << "NPC \"" << name << "\" added successfully at (" << x << ", " << y << ")" << std::endl;
        }
    }
}

void Game::printAllNPCs() const {
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    
    if (npcs.empty()) {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "No NPCs on the map." << std::endl;
        return;
    }
    
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "\n=== Current NPCs ===" << std::endl;
    for (const auto& npc : npcs) {
        std::cout << npc->getTypeName() << " \"" << npc->getName() << "\": (" 
                  << npc->getX() << ", " << npc->getY() << ")" << std::endl;
    }
    std::cout << "==================\n" << std::endl;
}

void Game::saveToFile(const std::string& filename) {
    std::string filepath = filename;
    if (!std::filesystem::path(filename).is_absolute()) {
        filepath = std::filesystem::current_path().parent_path().string() + "/" + filename;
    }
    
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filepath << " for writing." << std::endl;
        return;
    }
    
    for (const auto& npc : npcs) {
        npc->save(file);
    }
    
    file.close();
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "Saved " << npcs.size() << " NPCs to " << filepath << std::endl;
}

void Game::loadFromFile(const std::string& filename) {
    std::string filepath = filename;
    if (!std::filesystem::path(filename).is_absolute()) {
        filepath = std::filesystem::current_path().parent_path().string() + "/" + filename;
    }

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filepath << std::endl;
        return;
    }

    std::lock_guard<std::shared_mutex> write_lock(npc_mutex);
    npcs.clear();

    while (file.peek() != EOF) {
        auto npc = factory.loadNPC(file);
        
        if (npc) {
            // Subscribe all observers
            for (const auto& observer : observers) {
                npc->subscribe(observer);
            }
            npcs.push_back(npc);
        } else {
            break; // Stop on error
        }
    }

    file.close();
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "Loaded " << npcs.size() << " NPCs from " << filepath << std::endl;
}

size_t Game::getNPCCount() const {
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    return npcs.size();
}

void Game::initializeNPCs() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_x(0, MAP_WIDTH - 1);
    std::uniform_int_distribution<> dis_y(0, MAP_HEIGHT - 1);
    std::uniform_int_distribution<> dis_type(1, 3); // 1=Robber, 2=Elf, 3=Bear
    
    std::lock_guard<std::shared_mutex> lock(npc_mutex);
    npcs.clear();
    
    for (int i = 0; i < INITIAL_NPC_COUNT; ++i) {
        int x = dis_x(gen);
        int y = dis_y(gen);
        NPCType type = static_cast<NPCType>(dis_type(gen));
        
        auto npc = factory.createNPC(type, x, y);
        if (npc) {
            npc->setName("NPC_" + std::to_string(i));
            for (const auto& observer : observers) {
                npc->subscribe(observer);
            }
            npcs.push_back(npc);
        }
    }
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "Initialized " << npcs.size() << " NPCs on " << MAP_WIDTH 
                  << "x" << MAP_HEIGHT << " map." << std::endl;
    }
}

double Game::getDistance(const std::shared_ptr<NPC>& npc1, const std::shared_ptr<NPC>& npc2) const {
    double dx = npc1->getX() - npc2->getX();
    double dy = npc1->getY() - npc2->getY();
    return std::sqrt(dx * dx + dy * dy);
}

void Game::movementThreadFunction() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_direction(0, 7); // 8 направлений
    
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // Копируем список живых NPC
        std::vector<std::shared_ptr<NPC>> npcs_snapshot;
        {
            std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
            for (const auto& npc : npcs) {
                if (npc->isAlive()) {
                    npcs_snapshot.push_back(npc);
                }
            }
        }
        
        // Двигаем каждого живого NPC
        for (auto& npc : npcs_snapshot) {
            int dir = dis_direction(gen);
            int dx = 0, dy = 0;
            
            // Определяем направление движения (8 направлений)
            switch(dir) {
                case 0: dx = 1; dy = 0; break;  // Right
                case 1: dx = -1; dy = 0; break; // Left
                case 2: dx = 0; dy = 1; break;  // Down
                case 3: dx = 0; dy = -1; break; // Up
                case 4: dx = 1; dy = 1; break;  // Down-Right
                case 5: dx = 1; dy = -1; break; // Up-Right
                case 6: dx = -1; dy = 1; break; // Down-Left
                case 7: dx = -1; dy = -1; break; // Up-Left
            }
            
            // Получаем расстояние хода для этого NPC
            int moveDistance = npc->getMoveDistance();
            
            // Применяем движение со случайным множителем (1 до moveDistance)
            std::uniform_int_distribution<> dis_steps(1, moveDistance);
            int steps = dis_steps(gen);
            
            int new_x = npc->getX() + dx * steps;
            int new_y = npc->getY() + dy * steps;
            
            // Граница карты
            if (new_x >= 0 && new_x < MAP_WIDTH && new_y >= 0 && new_y < MAP_HEIGHT) {
                npc->setX(new_x);
                npc->setY(new_y);
            }
        }
        
        // Проверяем расстояния и создаём задачи на боевой поток
        std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
        for (size_t i = 0; i < npcs.size(); ++i) {
            if (!npcs[i]->isAlive()) continue;
            
            for (size_t j = i + 1; j < npcs.size(); ++j) {
                if (!npcs[j]->isAlive()) continue;
                
                // Получаем дистанцию убийства для обоих NPC (используем максимум)
                int killDist1 = npcs[i]->getKillDistance();
                int killDist2 = npcs[j]->getKillDistance();
                int maxKillDist = std::max(killDist1, killDist2);
                
                if (getDistance(npcs[i], npcs[j]) <= maxKillDist) {
                    Battle battle;
                    battle.attacker = npcs[i];
                    battle.defender = npcs[j];
                    
                    {
                        std::lock_guard<std::mutex> battle_lock(battle_queue_mutex);
                        battle_queue.push(battle);
                    }
                }
            }
        }
    }
}

void Game::performBattle(const Battle& battle) {
    // Проверяем, что оба NPC еще существуют в списке и живы
    {
        std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
        bool attacker_exists = std::find(npcs.begin(), npcs.end(), battle.attacker) != npcs.end();
        bool defender_exists = std::find(npcs.begin(), npcs.end(), battle.defender) != npcs.end();
        
        if (!attacker_exists || !defender_exists || !battle.attacker->isAlive() || !battle.defender->isAlive()) {
            return;
        }
    }
    
    // Используем боевую систему NPC (visitor pattern)
    BattleResult result = battle.attacker->accept_fight(battle.defender);
    
    std::lock_guard<std::shared_mutex> write_lock(npc_mutex);
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "[BATTLE] " << battle.attacker->getTypeName() << " " << battle.attacker->getName() 
                  << " vs " << battle.defender->getTypeName() << " " << battle.defender->getName();
    }
    
    // Обработка результата боя
    if (result == Victory) {
        // Атакующий победил
        battle.defender->setAlive(false);
        battle.attacker->fight_notify(battle.defender, static_cast<int>(result));
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << " - " << battle.attacker->getName() << " WINS!" << std::endl;
        }
    } else if (result == MutualDestruction) {
        // Оба погибают
        battle.attacker->setAlive(false);
        battle.defender->setAlive(false);
        battle.attacker->fight_notify(battle.defender, static_cast<int>(result));
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << " - MUTUAL DESTRUCTION!" << std::endl;
        }
    } else if (result == PeaceAndLove) {
        // Мирный исход
        battle.attacker->fight_notify(battle.defender, static_cast<int>(result));
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << " - PEACE!" << std::endl;
        }
    } else if (result == Defeat) {
        // Атакующий проигрывает
        battle.attacker->setAlive(false);
        battle.attacker->fight_notify(battle.defender, static_cast<int>(result));
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << " - " << battle.defender->getName() << " WINS!" << std::endl;
        }
    }
}

void Game::battleThreadFunction() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        Battle battle;
        bool has_battle = false;
        
        {
            std::lock_guard<std::mutex> battle_lock(battle_queue_mutex);
            if (!battle_queue.empty()) {
                battle = battle_queue.front();
                battle_queue.pop();
                has_battle = true;
            }
        }
        
        if (has_battle) {
            performBattle(battle);
        }
    }
}

void Game::printMap() const {
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    
    // Count alive NPCs
    int alive_count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) alive_count++;
    }
    
    if (alive_count == 0) {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "\n=== MAP (No NPCs alive) ===" << std::endl;
        return;
    }
    
    // Use a smaller display map (50x50) to avoid too much output
    constexpr int DISPLAY_WIDTH = 50;
    constexpr int DISPLAY_HEIGHT = 50;
    
    // Create display matrix
    std::vector<std::vector<char>> map(DISPLAY_HEIGHT, std::vector<char>(DISPLAY_WIDTH, '.'));
    
    // Mark alive NPCs only
    for (const auto& npc : npcs) {
        if (!npc->isAlive()) continue;
        
        int x = npc->getX();
        int y = npc->getY();
        
        // Scale down to display size
        int display_x = (x * DISPLAY_WIDTH) / MAP_WIDTH;
        int display_y = (y * DISPLAY_HEIGHT) / MAP_HEIGHT;
        
        if (display_x >= 0 && display_x < DISPLAY_WIDTH && display_y >= 0 && display_y < DISPLAY_HEIGHT) {
            char symbol = 'N'; // Default
            std::string type_name = npc->getTypeName();
            if (type_name == "Elf") symbol = 'E';
            else if (type_name == "Bear") symbol = 'B';
            else if (type_name == "Robber") symbol = 'R';
            
            if (map[display_y][display_x] == '.') {
                map[display_y][display_x] = symbol;
            } else {
                map[display_y][display_x] = '*'; // Multiple NPCs at same location
            }
        }
    }
    
    // Print the map
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "\n=== MAP (Alive: " << alive_count << "/R:Robber E:Elf B:Bear *:Multiple) ===" << std::endl;
    
    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
        for (int x = 0; x < DISPLAY_WIDTH; ++x) {
            std::cout << map[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << "=== 100x100 map scaled to 50x50 ===" << std::endl;
}

void Game::displayThreadFunction() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printMap();
    }
}

void Game::startGame() {
    if (running) {
        std::cout << "Game is already running!" << std::endl;
        return;
    }
    
    running = true;
    
    // Clear log file at the start
    std::string log_path = std::filesystem::current_path().parent_path().string() + "/log.txt";
    std::ofstream log_file(log_path, std::ios_base::trunc);
    if (log_file.is_open()) {
        log_file << "=== Game Log Started ===" << std::endl;
        log_file << "Map size: " << MAP_WIDTH << "x" << MAP_HEIGHT << std::endl;
        log_file << "Initial NPCs: " << INITIAL_NPC_COUNT << std::endl;
        log_file << "Game duration: " << GAME_DURATION_SECONDS << " seconds" << std::endl;
        log_file << "Robber: move distance = 10, kill distance = 10" << std::endl;
        log_file << "Elf:    move distance = 10, kill distance = 50" << std::endl;
        log_file << "Bear:   move distance = 10, kill distance = 10" << std::endl;
        log_file << "================================\n" << std::endl;
        log_file.close();
    }
    
    // Add observer for logging
    observers.push_back(ObserverLog::get());
    
    // Инициализируем NPC
    initializeNPCs();
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "\n========== GAME START ==========" << std::endl;
        std::cout << "Duration: " << GAME_DURATION_SECONDS << " seconds" << std::endl;
        std::cout << "Map size: " << MAP_WIDTH << "x" << MAP_HEIGHT << std::endl;
        std::cout << "Initial NPCs: " << INITIAL_NPC_COUNT << std::endl;
        std::cout << "Robber: move distance = 10, kill distance = 10" << std::endl;
        std::cout << "Elf:    move distance = 10, kill distance = 50" << std::endl;
        std::cout << "Bear:   move distance = 10, kill distance = 10" << std::endl;
        std::cout << "================================\n" << std::endl;
    }
    
    // Запускаем потоки
    movement_thread = std::make_unique<std::thread>(&Game::movementThreadFunction, this);
    battle_thread = std::make_unique<std::thread>(&Game::battleThreadFunction, this);
    display_thread = std::make_unique<std::thread>(&Game::displayThreadFunction, this);
    
    // Ждем нужное количество времени
    std::this_thread::sleep_for(std::chrono::seconds(GAME_DURATION_SECONDS));
    
    // Останавливаем игру
    stopGame();
}

void Game::stopGame() {
    if (!running) {
        return;
    }
    
    running = false;
    
    // Ждем завершения всех потоков
    if (movement_thread && movement_thread->joinable()) {
        movement_thread->join();
    }
    if (battle_thread && battle_thread->joinable()) {
        battle_thread->join();
    }
    if (display_thread && display_thread->joinable()) {
        display_thread->join();
    }
    
    // Выводим статистику
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "\n========== GAME OVER ==========" << std::endl;
    }
    printSurvivors();
}

void Game::printSurvivors() const {
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    
    // Count survivors by type
    int robber_count = 0, elf_count = 0, bear_count = 0;
    std::vector<std::shared_ptr<NPC>> survivors;
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            survivors.push_back(npc);
            if (npc->getType() == RobberType) robber_count++;
            else if (npc->getType() == ElfType) elf_count++;
            else if (npc->getType() == BearType) bear_count++;
        }
    }
    
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "\n=== Survivors ===" << std::endl;
    
    // Also write to log file
    std::string log_path = std::filesystem::current_path().parent_path().string() + "/log.txt";
    std::ofstream log_file(log_path, std::ios_base::app);
    if (log_file.is_open()) {
        log_file << "\n================================" << std::endl;
        log_file << "=== GAME OVER - SURVIVORS ===" << std::endl;
    }
    
    if (survivors.empty()) {
        std::cout << "No NPCs survived! All were killed in battle." << std::endl;
        if (log_file.is_open()) {
            log_file << "No NPCs survived! All were killed in battle." << std::endl;
        }
    } else {
        std::cout << "Total survivors: " << survivors.size() << " out of " << INITIAL_NPC_COUNT << std::endl;
        std::cout << "  - Robbers: " << robber_count << std::endl;
        std::cout << "  - Elves:   " << elf_count << std::endl;
        std::cout << "  - Bears:   " << bear_count << std::endl;
        
        if (log_file.is_open()) {
            log_file << "Total survivors: " << survivors.size() << " out of " << INITIAL_NPC_COUNT << std::endl;
            log_file << "  - Robbers: " << robber_count << std::endl;
            log_file << "  - Elves:   " << elf_count << std::endl;
            log_file << "  - Bears:   " << bear_count << std::endl;
        }
        
        std::cout << "\nDetailed list:" << std::endl;
        if (log_file.is_open()) {
            log_file << "\nDetailed list:" << std::endl;
        }
        
        for (const auto& npc : survivors) {
            std::cout << "  - " << npc->getTypeName() << " \"" << npc->getName() 
                      << "\" at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
            if (log_file.is_open()) {
                log_file << "  - " << npc->getTypeName() << " \"" << npc->getName() 
                         << "\" at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
            }
        }
    }
    
    if (log_file.is_open()) {
        log_file << "================================" << std::endl;
        log_file.close();
    }
    
    std::cout << "==================\n" << std::endl;
}

void Game::startBattle(int distance) {
    std::shared_lock<std::shared_mutex> read_lock(npc_mutex);
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "Starting battle with distance threshold: " << distance << " meters" << std::endl;
    }
    
    // Find all NPCs within the specified distance and trigger battles
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (getDistance(npcs[i], npcs[j]) <= distance) {
                Battle battle;
                battle.attacker = npcs[i];
                battle.defender = npcs[j];
                
                {
                    std::lock_guard<std::mutex> battle_lock(battle_queue_mutex);
                    battle_queue.push(battle);
                }
            }
        }
    }
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "Battle queue updated." << std::endl;
    }
}