#pragma once

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <atomic>
#include <chrono>
#include "NPC.hpp"
#include "Factory.hpp"
#include "Visitor.hpp"
#include "Observer.hpp"

// Constants for game map
constexpr int MAP_WIDTH = 100;
constexpr int MAP_HEIGHT = 100;
constexpr int GAME_DURATION_SECONDS = 30;
constexpr int INITIAL_NPC_COUNT = 50;

struct Battle {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class Game {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    Factory factory;
    std::vector<std::shared_ptr<Observer>> observers;
    
    // Synchronization primitives
    mutable std::shared_mutex npc_mutex;
    mutable std::mutex cout_mutex;
    std::mutex battle_queue_mutex;
    
    // Battle queue for thread communication
    std::queue<Battle> battle_queue;
    
    // Thread control
    std::atomic<bool> running{false};
    std::unique_ptr<std::thread> movement_thread;
    std::unique_ptr<std::thread> battle_thread;
    std::unique_ptr<std::thread> display_thread;
    
    // Helper methods for threads
    void movementThreadFunction();
    void battleThreadFunction();
    void displayThreadFunction();
    
    // Battle helper
    void performBattle(const Battle& battle);
    double getDistance(const std::shared_ptr<NPC>& npc1, const std::shared_ptr<NPC>& npc2) const;
    void printMap() const;
    
public:
    Game();
    ~Game();
    
    // Dependency injection - add observers from outside
    void addObserver(std::shared_ptr<Observer> observer);
    
    void addNPC(NPCType type, int x, int y, const std::string& name);
    void printAllNPCs() const;

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    bool isValidPosition(int x, int y) const;
    size_t getNPCCount() const;
    
    // Game control
    void startGame();
    void stopGame();
    void initializeNPCs();
    void startBattle(int distance);
    
    // Print utilities
    void printSurvivors() const;
};