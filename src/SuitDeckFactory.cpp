#include "SuitDeckFactory.h"
#include "Deck.h"

#include <cstddef>
using std::size_t;

Deck* SuitDeckFactory::generate(){
    Deck::Deck_t new_deck(0);
    for(size_t i(1); i <= SuitValue::King; ++i){
        new_deck.push_back(std::make_shared<Card>(Card(i, Suit_t::Hearts)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Suit_t::Spades)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Suit_t::Diamonds)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Suit_t::Clubs)));
    }
    new_deck.sort([](const Deck::Card_t& f, const Deck::Card_t& s){return (*f) < (*s);});
    return new Deck(new_deck.begin(), new_deck.end());
}
Deck* SuitDeckFactory::generate_w_joker(){
    Deck* toreturn = SuitDeckFactory::generate();
    toreturn->insert(Card(SuitValue::Joker, Suit_t::Hearts));
    toreturn->insert(Card(SuitValue::Joker, Suit_t::Spades));
    return toreturn;
}
