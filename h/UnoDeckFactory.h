#ifndef UNODECKFACTORY_H
#define UNODECKFACTORY_H

#include "DeckFactory.h"

struct UnoDeckFactory : public DeckFactory{
    virtual Deck* generate();
};

#endif