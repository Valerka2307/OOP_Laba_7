#include <gtest/gtest.h>
#include "Game.hpp"
#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Factory.hpp"

// Test NPC creation and properties
class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        robber = std::make_shared<Robber>(10, 20, "TestRobber");
        elf = std::make_shared<Elf>(30, 40, "TestElf");
        bear = std::make_shared<Bear>(50, 60, "TestBear");
    }

    std::shared_ptr<Robber> robber;
    std::shared_ptr<Elf> elf;
    std::shared_ptr<Bear> bear;
};

TEST_F(NPCTest, RobberCreation) {
    EXPECT_EQ(robber->getName(), "TestRobber");
    EXPECT_EQ(robber->getX(), 10);
    EXPECT_EQ(robber->getY(), 20);
    EXPECT_EQ(robber->getTypeName(), "Robber");
    EXPECT_TRUE(robber->isAlive());
    EXPECT_TRUE(robber->is_robber());
    EXPECT_FALSE(robber->is_elf());
    EXPECT_FALSE(robber->is_bear());
}

TEST_F(NPCTest, ElfCreation) {
    EXPECT_EQ(elf->getName(), "TestElf");
    EXPECT_EQ(elf->getX(), 30);
    EXPECT_EQ(elf->getY(), 40);
    EXPECT_EQ(elf->getTypeName(), "Elf");
    EXPECT_TRUE(elf->isAlive());
    EXPECT_FALSE(elf->is_robber());
    EXPECT_TRUE(elf->is_elf());
    EXPECT_FALSE(elf->is_bear());
}

TEST_F(NPCTest, BearCreation) {
    EXPECT_EQ(bear->getName(), "TestBear");
    EXPECT_EQ(bear->getX(), 50);
    EXPECT_EQ(bear->getY(), 60);
    EXPECT_EQ(bear->getTypeName(), "Bear");
    EXPECT_TRUE(bear->isAlive());
    EXPECT_FALSE(bear->is_robber());
    EXPECT_FALSE(bear->is_elf());
    EXPECT_TRUE(bear->is_bear());
}

// Test NPC movement distances
class NPCDistancesTest : public ::testing::Test {
protected:
    std::shared_ptr<Robber> robber = std::make_shared<Robber>(0, 0, "Robber");
    std::shared_ptr<Elf> elf = std::make_shared<Elf>(0, 0, "Elf");
    std::shared_ptr<Bear> bear = std::make_shared<Bear>(0, 0, "Bear");
};

TEST_F(NPCDistancesTest, RobberDistances) {
    EXPECT_EQ(robber->getMoveDistance(), 10);
    EXPECT_EQ(robber->getKillDistance(), 10);
}

TEST_F(NPCDistancesTest, ElfDistances) {
    EXPECT_EQ(elf->getMoveDistance(), 10);
    EXPECT_EQ(elf->getKillDistance(), 50);
}

TEST_F(NPCDistancesTest, BearDistances) {
    EXPECT_EQ(bear->getMoveDistance(), 10);
    EXPECT_EQ(bear->getKillDistance(), 10);
}

// Test NPC setters and state changes
class NPCStateTest : public ::testing::Test {
protected:
    std::shared_ptr<Robber> npc = std::make_shared<Robber>(0, 0, "TestNPC");
};

TEST_F(NPCStateTest, PositionSetters) {
    npc->setX(100);
    npc->setY(200);
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

TEST_F(NPCStateTest, AliveState) {
    EXPECT_TRUE(npc->isAlive());
    npc->setAlive(false);
    EXPECT_FALSE(npc->isAlive());
    npc->setAlive(true);
    EXPECT_TRUE(npc->isAlive());
}

TEST_F(NPCStateTest, NameSetter) {
    std::string new_name = "NewName";
    npc->setName(new_name);
    EXPECT_EQ(npc->getName(), new_name);
}

// Test Factory
class FactoryTest : public ::testing::Test {
protected:
    Factory factory;
};

TEST_F(FactoryTest, CreateRobber) {
    auto robber = factory.createNPC(RobberType, 10, 20);
    EXPECT_NE(robber, nullptr);
    EXPECT_EQ(robber->getTypeName(), "Robber");
    EXPECT_EQ(robber->getX(), 10);
    EXPECT_EQ(robber->getY(), 20);
}

TEST_F(FactoryTest, CreateElf) {
    auto elf = factory.createNPC(ElfType, 30, 40);
    EXPECT_NE(elf, nullptr);
    EXPECT_EQ(elf->getTypeName(), "Elf");
    EXPECT_EQ(elf->getX(), 30);
    EXPECT_EQ(elf->getY(), 40);
}

TEST_F(FactoryTest, CreateBear) {
    auto bear = factory.createNPC(BearType, 50, 60);
    EXPECT_NE(bear, nullptr);
    EXPECT_EQ(bear->getTypeName(), "Bear");
    EXPECT_EQ(bear->getX(), 50);
    EXPECT_EQ(bear->getY(), 60);
}

TEST_F(FactoryTest, CreateInvalidType) {
    auto npc = factory.createNPC(Unknown, 0, 0);
    EXPECT_EQ(npc, nullptr);
}

// Test Game map validation
class GameMapTest : public ::testing::Test {
protected:
    Game game;
};

TEST_F(GameMapTest, ValidPositions) {
    EXPECT_TRUE(game.isValidPosition(0, 0));
    EXPECT_TRUE(game.isValidPosition(50, 50));
    EXPECT_TRUE(game.isValidPosition(99, 99));
    EXPECT_TRUE(game.isValidPosition(MAP_WIDTH - 1, MAP_HEIGHT - 1));
}

TEST_F(GameMapTest, InvalidPositions) {
    EXPECT_FALSE(game.isValidPosition(-1, 0));
    EXPECT_FALSE(game.isValidPosition(0, -1));
    EXPECT_FALSE(game.isValidPosition(100, 0));
    EXPECT_FALSE(game.isValidPosition(0, 100));
    EXPECT_FALSE(game.isValidPosition(MAP_WIDTH, MAP_HEIGHT));
}

TEST_F(GameMapTest, BoundaryPositions) {
    EXPECT_TRUE(game.isValidPosition(0, MAP_HEIGHT - 1));
    EXPECT_TRUE(game.isValidPosition(MAP_WIDTH - 1, 0));
    EXPECT_FALSE(game.isValidPosition(MAP_WIDTH, 0));
    EXPECT_FALSE(game.isValidPosition(0, MAP_HEIGHT));
}

// Test Game NPC management
class GameNPCManagementTest : public ::testing::Test {
protected:
    Game game;
};

TEST_F(GameNPCManagementTest, AddNPC) {
    game.addNPC(RobberType, 10, 20, "Robber1");
    EXPECT_EQ(game.getNPCCount(), 1);
}

TEST_F(GameNPCManagementTest, AddMultipleNPCs) {
    game.addNPC(RobberType, 10, 20, "Robber1");
    game.addNPC(ElfType, 30, 40, "Elf1");
    game.addNPC(BearType, 50, 60, "Bear1");
    EXPECT_EQ(game.getNPCCount(), 3);
}

TEST_F(GameNPCManagementTest, AddNPCInvalidPosition) {
    game.addNPC(RobberType, -1, 20, "InvalidRobber");
    EXPECT_EQ(game.getNPCCount(), 0);
    
    game.addNPC(RobberType, 100, 100, "InvalidRobber2");
    EXPECT_EQ(game.getNPCCount(), 0);
}

TEST_F(GameNPCManagementTest, InitializeNPCs) {
    game.initializeNPCs();
    EXPECT_EQ(game.getNPCCount(), INITIAL_NPC_COUNT);
}

// Test constants
class ConstantsTest : public ::testing::Test {
};

TEST_F(ConstantsTest, MapDimensions) {
    EXPECT_EQ(MAP_WIDTH, 100);
    EXPECT_EQ(MAP_HEIGHT, 100);
}

TEST_F(ConstantsTest, GameDuration) {
    EXPECT_EQ(GAME_DURATION_SECONDS, 30);
}

TEST_F(ConstantsTest, InitialNPCCount) {
    EXPECT_EQ(INITIAL_NPC_COUNT, 50);
}

// Test Battle Results enum
class BattleResultTest : public ::testing::Test {
};

TEST_F(BattleResultTest, BattleResultValues) {
    EXPECT_EQ(Victory, 0);
    EXPECT_EQ(Defeat, 1);
    EXPECT_EQ(PeaceAndLove, 2);
    EXPECT_EQ(MutualDestruction, 3);
}

// Test Observer interface
class ObserverTest : public ::testing::Test {
};

TEST_F(ObserverTest, ObserverLogCreation) {
    auto observer = ObserverLog::get();
    EXPECT_NE(observer, nullptr);
}

TEST_F(ObserverTest, ObserverOutCreation) {
    auto observer = ObserverOut::get();
    EXPECT_NE(observer, nullptr);
}

// Test NPC proximity
class NPCProximityTest : public ::testing::Test {
protected:
    std::shared_ptr<Robber> npc1 = std::make_shared<Robber>(0, 0, "NPC1");
    std::shared_ptr<Elf> npc2 = std::make_shared<Elf>(3, 4, "NPC2");
};

TEST_F(NPCProximityTest, DistanceCalculation) {
    // Distance between (0,0) and (3,4) is 5
    EXPECT_TRUE(npc1->is_close(npc2, 5));
    EXPECT_TRUE(npc1->is_close(npc2, 6));
    EXPECT_FALSE(npc1->is_close(npc2, 4));
}

TEST_F(NPCProximityTest, SamePosition) {
    auto npc3 = std::make_shared<Bear>(0, 0, "NPC3");
    EXPECT_TRUE(npc1->is_close(npc3, 0));
}
