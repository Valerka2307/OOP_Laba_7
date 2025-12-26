#include "Game.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <limits>

void displayMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "     NPC Battle Simulation Menu" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Start new simulation (30 seconds)" << std::endl;
    std::cout << "2. View simulation log" << std::endl;
    std::cout << "3. Clear log file" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Select option (1-4): ";
}

void viewLog() {
    std::string log_path = std::filesystem::current_path().parent_path().string() + "/log.txt";
    std::ifstream log_file(log_path);
    
    if (!log_file.is_open()) {
        std::cout << "\nNo log file found at: " << log_path << std::endl;
        return;
    }
    
    std::cout << "\n========== SIMULATION LOG ==========" << std::endl;
    std::string line;
    while (std::getline(log_file, line)) {
        std::cout << line << std::endl;
    }
    std::cout << "==================================\n" << std::endl;
    
    log_file.close();
}

void clearLog() {
    std::string log_path = std::filesystem::current_path().parent_path().string() + "/log.txt";
    std::ofstream log_file(log_path, std::ios_base::trunc);
    if (log_file.is_open()) {
        log_file << "Log cleared." << std::endl;
        log_file.close();
        std::cout << "\nLog file cleared successfully.\n" << std::endl;
    } else {
        std::cout << "\nError: Could not clear log file.\n" << std::endl;
    }
}

int main() {
    int choice = 0;
    bool running = true;
    
    std::cout << "\n" << std::endl;
    std::cout << "  ╔════════════════════════════════════╗" << std::endl;
    std::cout << "  ║   NPC Battle Simulation Engine     ║" << std::endl;
    std::cout << "  ║   Robber vs Elf vs Bear           ║" << std::endl;
    std::cout << "  ╚════════════════════════════════════╝" << std::endl;
    
    while (running) {
        displayMenu();
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                std::cout << "\nStarting new simulation..." << std::endl;
                std::cout << "Please wait (30 seconds)...\n" << std::endl;
                
                Game game;
                game.startGame();
                
                std::cout << "\nSimulation completed!" << std::endl;
                std::cout << "Check log.txt for detailed battle information.\n" << std::endl;
                break;
            }
            
            case 2: {
                viewLog();
                break;
            }
            
            case 3: {
                std::cout << "\nClearing log file..." << std::endl;
                clearLog();
                break;
            }
            
            case 4: {
                std::cout << "\nThank you for using NPC Battle Simulation!" << std::endl;
                std::cout << "Goodbye!\n" << std::endl;
                running = false;
                break;
            }
            
            default: {
                std::cout << "\nInvalid option! Please select 1-4.\n" << std::endl;
                break;
            }
        }
    }
    
    return 0;
}
