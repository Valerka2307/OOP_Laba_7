#pragma once

#include <memory>

class NPC;
class Robber;
class Elf;
class Bear;

enum BattleResult {
    Victory,
    Defeat,
    PeaceAndLove,
    MutualDestruction
};

struct BattleStats {
    int attacker_attack;
    int attacker_defense;
    int defender_attack;
    int defender_defense;
};

class BattleVisitor {
  public:
    
    virtual BattleResult fight(std::shared_ptr<Robber>, const BattleStats&) = 0;
    virtual BattleResult fight(std::shared_ptr<Elf>, const BattleStats&) = 0;
    virtual BattleResult fight(std::shared_ptr<Bear>, const BattleStats&) = 0;

    virtual BattleResult accept_fight(std::shared_ptr<NPC>, const BattleStats&) = 0;
    virtual ~BattleVisitor() = default;
};