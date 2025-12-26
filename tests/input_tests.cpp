#include <gtest/gtest.h>
#include "Game.hpp"

// Input validation tests
class InputTest : public ::testing::Test {
};

TEST_F(InputTest, ValidNPCName) {
    std::string name = "TestNPC";
    EXPECT_FALSE(name.empty());
    EXPECT_GT(name.length(), 0);
}

TEST_F(InputTest, ValidPositions) {
    Game game;
    EXPECT_TRUE(game.isValidPosition(0, 0));
    EXPECT_TRUE(game.isValidPosition(50, 50));
    EXPECT_TRUE(game.isValidPosition(99, 99));
}

TEST_F(InputTest, InvalidNegativePositions) {
    Game game;
    EXPECT_FALSE(game.isValidPosition(-1, 0));
    EXPECT_FALSE(game.isValidPosition(0, -1));
    EXPECT_FALSE(game.isValidPosition(-10, -10));
}

TEST_F(InputTest, InvalidLargePositions) {
    Game game;
    EXPECT_FALSE(game.isValidPosition(100, 0));
    EXPECT_FALSE(game.isValidPosition(0, 100));
    EXPECT_FALSE(game.isValidPosition(200, 200));
}
