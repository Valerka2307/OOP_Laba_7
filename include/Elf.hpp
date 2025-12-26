#pragma once
#include "NPC.hpp"

class Elf : public NPC {
  public:
    Elf(int x, int y, const std::string& name = "");
    Elf(std::istream &is);

    void print() override;
    std::string getTypeName() const override;
    int getMoveDistance() const override { return 10; }
    int getKillDistance() const override { return 50; }

    bool is_elf() const override;

    BattleResult fight(std::shared_ptr<Robber> other, const BattleStats&) override;
    BattleResult fight(std::shared_ptr<Elf> other, const BattleStats&) override;
    BattleResult fight(std::shared_ptr<Bear> other, const BattleStats&) override;

    BattleResult accept_fight(std::shared_ptr<NPC>, const BattleStats&) override;
    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Elf &elf);
    ~Elf() override = default;
};