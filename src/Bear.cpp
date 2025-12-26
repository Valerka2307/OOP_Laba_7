#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Visitor.hpp"

Bear::Bear(int x, int y, const std::string& name) : NPC(BearType, x, y, name) {}
Bear::Bear(std::istream &is) : NPC(BearType, is) {}

void Bear::print() {
    std::cout << "Bear \"" << name << "\": " << *this;
}

std::string Bear::getTypeName() const {
    return "Bear";
}

void Bear::save(std::ostream &os) {
    NPC::save(os);
}
bool Bear::is_bear() const { return true; }

BattleResult Bear::fight(std::shared_ptr<Robber> other, const BattleStats& stats) {
    int attacker_damage = stats.attacker_attack;
    int defender_protection = stats.defender_defense;
    
    int defender_damage = stats.defender_attack;
    int attacker_protection = stats.attacker_defense;
    
    bool attacker_wins = attacker_damage > defender_protection;
    bool defender_wins = defender_damage > attacker_protection;
    
    if (attacker_wins && defender_wins) {
        return MutualDestruction;
    } else if (attacker_wins) {
        return Victory;
    } else if (defender_wins) {
        return Defeat;
    } else {
        return PeaceAndLove;
    }
}

BattleResult Bear::fight(std::shared_ptr<Elf> other, const BattleStats& stats) {
    int attacker_damage = stats.attacker_attack;
    int defender_protection = stats.defender_defense;
    
    int defender_damage = stats.defender_attack;
    int attacker_protection = stats.attacker_defense;
    
    bool attacker_wins = attacker_damage > defender_protection;
    bool defender_wins = defender_damage > attacker_protection;
    
    if (attacker_wins && defender_wins) {
        return MutualDestruction;
    } else if (attacker_wins) {
        return Victory;
    } else if (defender_wins) {
        return Defeat;
    } else {
        return PeaceAndLove;
    }
}

BattleResult Bear::fight(std::shared_ptr<Bear> other, const BattleStats& stats) {
    int attacker_damage = stats.attacker_attack;
    int defender_protection = stats.defender_defense;
    
    int defender_damage = stats.defender_attack;
    int attacker_protection = stats.attacker_defense;
    
    bool attacker_wins = attacker_damage > defender_protection;
    bool defender_wins = defender_damage > attacker_protection;
    
    if (attacker_wins && defender_wins) {
        return MutualDestruction;
    } else if (attacker_wins) {
        return Victory;
    } else if (defender_wins) {
        return Defeat;
    } else {
        return PeaceAndLove;
    }
}

BattleResult Bear::accept_fight(std::shared_ptr<NPC> other, const BattleStats& stats) {
    return other->fight(std::dynamic_pointer_cast<Bear>(shared_from_this()), stats);
}

std::ostream &operator<<(std::ostream &os, Bear &bear) {
    os << "bear: " << *static_cast<NPC *>(&bear) << std::endl;
    return os;
}