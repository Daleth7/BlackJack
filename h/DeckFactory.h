#ifndef DECKFACTORY_H
#define DECKFACTORY_H

class Deck;

struct DeckFactory{
    virtual Deck* generate() = 0;
    virtual ~DeckFactory() = default;
};

#endif