#pragma once
#include "NPC.hpp"

class Bear : public NPC {
  public:
    Bear(int x, int y, const std::string& name = "");
    Bear(std::istream &is);

    void print() override;
    std::string getTypeName() const override;
    int getMoveDistance() const override { return 10; }
    int getKillDistance() const override { return 10; }

    bool is_bear() const override;

    BattleResult fight(std::shared_ptr<Robber> other, const BattleStats&) override;
    BattleResult fight(std::shared_ptr<Elf> other, const BattleStats&) override;
    BattleResult fight(std::shared_ptr<Bear> other, const BattleStats&) override;

    BattleResult accept_fight(std::shared_ptr<NPC>, const BattleStats&) override;
    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Bear &bear);
    ~Bear() override = default;
};