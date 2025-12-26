#include <gtest/gtest.h>
#include "Factory.hpp"
#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"

// Factory pattern tests
class FactoryTest : public ::testing::Test {
protected:
    Factory factory;
};

TEST_F(FactoryTest, CreateRobberWithFactory) {
    auto npc = factory.createNPC(RobberType, 10, 20);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->getTypeName(), "Robber");
    EXPECT_EQ(npc->getX(), 10);
    EXPECT_EQ(npc->getY(), 20);
    EXPECT_TRUE(npc->is_robber());
}

TEST_F(FactoryTest, CreateElfWithFactory) {
    auto npc = factory.createNPC(ElfType, 30, 40);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->getTypeName(), "Elf");
    EXPECT_EQ(npc->getX(), 30);
    EXPECT_EQ(npc->getY(), 40);
    EXPECT_TRUE(npc->is_elf());
}

TEST_F(FactoryTest, CreateBearWithFactory) {
    auto npc = factory.createNPC(BearType, 50, 60);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->getTypeName(), "Bear");
    EXPECT_EQ(npc->getX(), 50);
    EXPECT_EQ(npc->getY(), 60);
    EXPECT_TRUE(npc->is_bear());
}

TEST_F(FactoryTest, CreateUnknownTypeReturnsNull) {
    auto npc = factory.createNPC(Unknown, 0, 0);
    EXPECT_EQ(npc, nullptr);
}

TEST_F(FactoryTest, MultipleCreations) {
    auto npc1 = factory.createNPC(RobberType, 0, 0);
    auto npc2 = factory.createNPC(ElfType, 10, 10);
    auto npc3 = factory.createNPC(BearType, 20, 20);
    
    EXPECT_NE(npc1, nullptr);
    EXPECT_NE(npc2, nullptr);
    EXPECT_NE(npc3, nullptr);
    EXPECT_NE(npc1, npc2);
    EXPECT_NE(npc2, npc3);
    EXPECT_NE(npc1, npc3);
}

TEST_F(FactoryTest, FactoryPreservesType) {
    for (int i = 0; i < 10; ++i) {
        auto robber = factory.createNPC(RobberType, i, i);
        EXPECT_EQ(robber->getType(), RobberType);
    }
}
