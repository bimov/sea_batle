#ifndef DOUBLE_DAMAGE_H
#define DOUBLE_DAMAGE_H

#include "i_ability.h"

class DoubleDamage: public iAbility {
private:
    Field* field;
    int x;
    int y;
    Output output;
public:
    DoubleDamage(Field* field);
    void use_ability();
    virtual ~DoubleDamage() = default;
};
#endif