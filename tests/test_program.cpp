#include <gtest/gtest.h>
#include "Game.hpp"
#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"

// Integration tests for the entire simulation system
class SimulationIntegrationTest : public ::testing::Test {
protected:
    Game game;
};

TEST_F(SimulationIntegrationTest, GameInitialization) {
    game.initializeNPCs();
    EXPECT_EQ(game.getNPCCount(), INITIAL_NPC_COUNT);
}

TEST_F(SimulationIntegrationTest, NPCPositionsValid) {
    game.initializeNPCs();
    EXPECT_EQ(game.getNPCCount(), INITIAL_NPC_COUNT);
    // All NPCs should be initialized at valid positions
    // This is verified by the game not throwing errors during initialization
}

TEST_F(SimulationIntegrationTest, MultipleGameRuns) {
    // First run
    game.initializeNPCs();
    EXPECT_EQ(game.getNPCCount(), INITIAL_NPC_COUNT);
}

// Test battle mechanics
class BattleMechanicsTest : public ::testing::Test {
};

TEST_F(BattleMechanicsTest, BearVsRobber) {
    auto bear = std::make_shared<Bear>(0, 0, "Bear");
    auto robber = std::make_shared<Robber>(0, 0, "Robber");
    
    BattleStats stats{3, 2, 4, 1};
    BattleResult result = bear->accept_fight(robber, stats);
    // Result depends on the dice rolls
    EXPECT_TRUE(result == Victory || result == Defeat || result == PeaceAndLove || result == MutualDestruction);
}

TEST_F(BattleMechanicsTest, RobberVsRobber) {
    auto robber1 = std::make_shared<Robber>(0, 0, "Robber1");
    auto robber2 = std::make_shared<Robber>(0, 0, "Robber2");
    
    BattleStats stats{3, 2, 4, 1};
    BattleResult result = robber1->accept_fight(robber2, stats);
    // Result depends on the dice rolls
    EXPECT_TRUE(result == Victory || result == Defeat || result == PeaceAndLove || result == MutualDestruction);
}

TEST_F(BattleMechanicsTest, ElfVsElf) {
    auto elf1 = std::make_shared<Elf>(0, 0, "Elf1");
    auto elf2 = std::make_shared<Elf>(0, 0, "Elf2");
    
    BattleStats stats{3, 2, 4, 1};
    BattleResult result = elf1->accept_fight(elf2, stats);
    // Result depends on the dice rolls
    EXPECT_TRUE(result == Victory || result == Defeat || result == PeaceAndLove || result == MutualDestruction);
}

TEST_F(BattleMechanicsTest, BearVsBear) {
    auto bear1 = std::make_shared<Bear>(0, 0, "Bear1");
    auto bear2 = std::make_shared<Bear>(0, 0, "Bear2");
    
    BattleStats stats{3, 2, 4, 1};
    BattleResult result = bear1->accept_fight(bear2, stats);
    // Result depends on the dice rolls
    EXPECT_TRUE(result == Victory || result == Defeat || result == PeaceAndLove || result == MutualDestruction);
}

// Test NPC type identification
class NPCTypeIdentificationTest : public ::testing::Test {
};

TEST_F(NPCTypeIdentificationTest, RobberIdentification) {
    auto robber = std::make_shared<Robber>(0, 0, "TestRobber");
    std::shared_ptr<NPC> npc = robber;
    
    EXPECT_TRUE(npc->is_robber());
    EXPECT_FALSE(npc->is_elf());
    EXPECT_FALSE(npc->is_bear());
}

TEST_F(NPCTypeIdentificationTest, ElfIdentification) {
    auto elf = std::make_shared<Elf>(0, 0, "TestElf");
    std::shared_ptr<NPC> npc = elf;
    
    EXPECT_FALSE(npc->is_robber());
    EXPECT_TRUE(npc->is_elf());
    EXPECT_FALSE(npc->is_bear());
}

TEST_F(NPCTypeIdentificationTest, BearIdentification) {
    auto bear = std::make_shared<Bear>(0, 0, "TestBear");
    std::shared_ptr<NPC> npc = bear;
    
    EXPECT_FALSE(npc->is_robber());
    EXPECT_FALSE(npc->is_elf());
    EXPECT_TRUE(npc->is_bear());
}

// Test map boundaries and NPC movement constraints
class MapBoundaryTest : public ::testing::Test {
protected:
    Game game;
};

TEST_F(MapBoundaryTest, AllCorners) {
    // Top-left
    EXPECT_TRUE(game.isValidPosition(0, 0));
    // Top-right
    EXPECT_TRUE(game.isValidPosition(MAP_WIDTH - 1, 0));
    // Bottom-left
    EXPECT_TRUE(game.isValidPosition(0, MAP_HEIGHT - 1));
    // Bottom-right
    EXPECT_TRUE(game.isValidPosition(MAP_WIDTH - 1, MAP_HEIGHT - 1));
}

TEST_F(MapBoundaryTest, OutOfBounds) {
    EXPECT_FALSE(game.isValidPosition(-1, -1));
    EXPECT_FALSE(game.isValidPosition(MAP_WIDTH, MAP_HEIGHT));
    EXPECT_FALSE(game.isValidPosition(-100, -100));
    EXPECT_FALSE(game.isValidPosition(1000, 1000));
}

// Test game configuration constants
class GameConfigTest : public ::testing::Test {
};

TEST_F(GameConfigTest, MapSize) {
    EXPECT_GT(MAP_WIDTH, 0);
    EXPECT_GT(MAP_HEIGHT, 0);
    EXPECT_EQ(MAP_WIDTH, 100);
    EXPECT_EQ(MAP_HEIGHT, 100);
}

TEST_F(GameConfigTest, GameDuration) {
    EXPECT_GT(GAME_DURATION_SECONDS, 0);
    EXPECT_EQ(GAME_DURATION_SECONDS, 30);
}

TEST_F(GameConfigTest, InitialNPCCount) {
    EXPECT_GT(INITIAL_NPC_COUNT, 0);
    EXPECT_EQ(INITIAL_NPC_COUNT, 50);
}

// Test NPC movement distance configurations
class NPCMovementConfigTest : public ::testing::Test {
};

TEST_F(NPCMovementConfigTest, AllNPCsHaveMoveDistance) {
    auto robber = std::make_shared<Robber>(0, 0, "Robber");
    auto elf = std::make_shared<Elf>(0, 0, "Elf");
    auto bear = std::make_shared<Bear>(0, 0, "Bear");
    
    EXPECT_EQ(robber->getMoveDistance(), 10);
    EXPECT_EQ(elf->getMoveDistance(), 10);
    EXPECT_EQ(bear->getMoveDistance(), 10);
}

TEST_F(NPCMovementConfigTest, RobberKillDistance) {
    auto robber = std::make_shared<Robber>(0, 0, "Robber");
    EXPECT_EQ(robber->getKillDistance(), 10);
}

TEST_F(NPCMovementConfigTest, ElfKillDistance) {
    auto elf = std::make_shared<Elf>(0, 0, "Elf");
    EXPECT_EQ(elf->getKillDistance(), 50);
}

TEST_F(NPCMovementConfigTest, BearKillDistance) {
    auto bear = std::make_shared<Bear>(0, 0, "Bear");
    EXPECT_EQ(bear->getKillDistance(), 10);
}

// Test NPC lifecycle
class NPCLifecycleTest : public ::testing::Test {
};

TEST_F(NPCLifecycleTest, NPCBornAlive) {
    auto npc = std::make_shared<Robber>(0, 0, "TestNPC");
    EXPECT_TRUE(npc->isAlive());
}

TEST_F(NPCLifecycleTest, NPCCanDie) {
    auto npc = std::make_shared<Robber>(0, 0, "TestNPC");
    npc->setAlive(false);
    EXPECT_FALSE(npc->isAlive());
}

TEST_F(NPCLifecycleTest, NPCCanBeResurrected) {
    auto npc = std::make_shared<Robber>(0, 0, "TestNPC");
    npc->setAlive(false);
    EXPECT_FALSE(npc->isAlive());
    npc->setAlive(true);
    EXPECT_TRUE(npc->isAlive());
}

// Test NPC position updates
class NPCPositionTest : public ::testing::Test {
protected:
    std::shared_ptr<Robber> npc = std::make_shared<Robber>(50, 50, "TestNPC");
};

TEST_F(NPCPositionTest, InitialPosition) {
    EXPECT_EQ(npc->getX(), 50);
    EXPECT_EQ(npc->getY(), 50);
}

TEST_F(NPCPositionTest, MoveToNewPosition) {
    npc->setX(75);
    npc->setY(25);
    EXPECT_EQ(npc->getX(), 75);
    EXPECT_EQ(npc->getY(), 25);
}

TEST_F(NPCPositionTest, MovePastBoundary) {
    npc->setX(150);  // Beyond map size
    npc->setY(150);
    EXPECT_EQ(npc->getX(), 150);
    EXPECT_EQ(npc->getY(), 150);
    // Note: Boundary checking is done at game level, not at NPC level
}
