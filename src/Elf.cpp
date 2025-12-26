#include "Robber.hpp"
#include "Elf.hpp"
#include "Bear.hpp"
#include "Visitor.hpp"

Elf::Elf(int x, int y, const std::string& name) : NPC(ElfType, x, y, name) {}
Elf::Elf(std::istream &is) : NPC(ElfType, is) {}

void Elf::print() {
    std::cout << "Elf \"" << name << "\": " << *this;
}

std::string Elf::getTypeName() const {
    return "Elf";
}

void Elf::save(std::ostream &os) {
    NPC::save(os);
}
bool Elf::is_elf() const { return true; }

BattleResult Elf::fight(std::shared_ptr<Robber> other, const BattleStats& stats) {
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

BattleResult Elf::fight(std::shared_ptr<Elf> other, const BattleStats& stats) {
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

BattleResult Elf::fight(std::shared_ptr<Bear> other, const BattleStats& stats) {
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

BattleResult Elf::accept_fight(std::shared_ptr<NPC> other, const BattleStats& stats) {
    return other->fight(std::dynamic_pointer_cast<Elf>(shared_from_this()), stats);
}

std::ostream &operator<<(std::ostream &os, Elf &elf) {
    os << "elf: " << *static_cast<NPC *>(&elf) << std::endl;
    return os;
}