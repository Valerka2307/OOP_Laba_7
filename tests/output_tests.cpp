#include <gtest/gtest.h>
#include <sstream>
#include "Game.hpp"
#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Observer.hpp"

// Output and observer tests
class OutputTest : public ::testing::Test {
};

TEST_F(OutputTest, NPCTypeName) {
    auto robber = std::make_shared<Robber>(0, 0, "TestRobber");
    auto elf = std::make_shared<Elf>(0, 0, "TestElf");
    auto bear = std::make_shared<Bear>(0, 0, "TestBear");
    
    EXPECT_EQ(robber->getTypeName(), "Robber");
    EXPECT_EQ(elf->getTypeName(), "Elf");
    EXPECT_EQ(bear->getTypeName(), "Bear");
}

TEST_F(OutputTest, NPCName) {
    auto npc = std::make_shared<Robber>(0, 0, "MyNPC");
    EXPECT_EQ(npc->getName(), "MyNPC");
    
    npc->setName("NewName");
    EXPECT_EQ(npc->getName(), "NewName");
}

TEST_F(OutputTest, NPCPosition) {
    auto npc = std::make_shared<Elf>(25, 75, "TestElf");
    EXPECT_EQ(npc->getX(), 25);
    EXPECT_EQ(npc->getY(), 75);
}

TEST_F(OutputTest, ObserverLogCreated) {
    auto observer = ObserverLog::get();
    EXPECT_NE(observer, nullptr);
}

TEST_F(OutputTest, ObserverOutCreated) {
    auto observer = ObserverOut::get();
    EXPECT_NE(observer, nullptr);
}

TEST_F(OutputTest, GameInitializesProperly) {
    Game game;
    game.initializeNPCs();
    EXPECT_EQ(game.getNPCCount(), INITIAL_NPC_COUNT);
}

TEST_F(OutputTest, MultipleObserversCanBeCreated) {
    auto observer1 = ObserverLog::get();
    auto observer2 = ObserverLog::get();
    auto observer3 = ObserverOut::get();
    
    EXPECT_NE(observer1, nullptr);
    EXPECT_NE(observer2, nullptr);
    EXPECT_NE(observer3, nullptr);
}
