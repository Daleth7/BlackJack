#include "UnoDeckFactory.h"
#include "Deck.h"

Deck* UnoDeckFactory::generate(){
    Deck::Deck_t new_deck{
        Deck::Card_t(std::make_shared<Card>(Card(0, Uno_t::Blue))),
        Deck::Card_t(std::make_shared<Card>(Card(0, Uno_t::Red))),
        Deck::Card_t(std::make_shared<Card>(Card(0, Uno_t::Green))),
        Deck::Card_t(std::make_shared<Card>(Card(0, Uno_t::Yellow)))
    };
    for(size_t i(0); i < 2; ++i){
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Skip, Uno_t::Blue))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Skip, Uno_t::Red))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Skip, Uno_t::Green))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Skip, Uno_t::Yellow))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Draw_Two, Uno_t::Blue))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Draw_Two, Uno_t::Red))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Draw_Two, Uno_t::Green))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Draw_Two, Uno_t::Yellow))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Reverse, Uno_t::Blue))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Reverse, Uno_t::Red))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Reverse, Uno_t::Green))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Reverse, Uno_t::Yellow))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Wild, Uno_t::Black))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Wild, Uno_t::Black))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Wild_Draw_Four, Uno_t::Black))),
        new_deck.push_back(std::make_shared<Card>(Card(UnoValue::Wild_Draw_Four, Uno_t::Black)))
        ;
    }
    for(size_t i(1); i <= UnoValue::Skip; ++i){
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Blue)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Blue)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Red)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Red)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Green)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Green)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Yellow)));
        new_deck.push_back(std::make_shared<Card>(Card(i, Uno_t::Yellow)));
    }
    new_deck.sort([](Deck::Card_t f, Deck::Card_t s){return (*f) < (*s);});

    return new Deck(new_deck.begin(), new_deck.end());
}