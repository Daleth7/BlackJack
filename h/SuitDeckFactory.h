#ifndef SUITDECKFACTORY_H
#define SUITDECKFACTORY_H

#include "DeckFactory.h"

struct SuitDeckFactory : public DeckFactory{
    virtual Deck* generate();
    virtual Deck* generate_w_joker();
};

#endif