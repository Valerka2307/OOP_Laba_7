#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Visitor.hpp"

Robber::Robber(int x, int y, const std::string& name) : NPC(RobberType, x, y, name) {}
Robber::Robber(std::istream &is) : NPC(RobberType, is) {}

void Robber::print() {
    std::cout << "Robber \"" << name << "\": " << *this;
}

std::string Robber::getTypeName() const {
    return "Robber";
}

void Robber::save(std::ostream &os) {
    NPC::save(os);
}
bool Robber::is_robber() const { return true; }

BattleResult Robber::fight(std::shared_ptr<Robber> other, const BattleStats& stats) {
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

BattleResult Robber::fight(std::shared_ptr<Elf> other, const BattleStats& stats) {
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

BattleResult Robber::fight(std::shared_ptr<Bear> other, const BattleStats& stats) {
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

BattleResult Robber::accept_fight(std::shared_ptr<NPC> other, const BattleStats& stats) {
    return other->fight(std::dynamic_pointer_cast<Robber>(shared_from_this()), stats);
}

std::ostream &operator<<(std::ostream &os, Robber &robber) {
    os << "robber: " << *static_cast<NPC *>(&robber) << std::endl;
    return os;
}