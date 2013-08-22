#include "Deck.h"
#include <iterator>
#include <algorithm>

//Read-Only
size_t Deck::size()const
    {return m_deck.size();}
size_t Deck::trash_size()const
    {return m_trash.size();}
const Deck::RandGen& Deck::generator()const
    {return m_gen;}
bool Deck::always_shuffle()const
    {return m_always_shuffle;}

//Other methods
Deck::Card_t Deck::draw(){
    if(m_deck.size() == 0) return nullptr;
    Card_t toreturn(m_deck.back());
    m_deck.pop_back();
    m_trash.push_back(toreturn);
    if(m_always_shuffle) this->shuffle();
    return toreturn;
}
    //by default places card at the bottom of the deck
void Deck::insert(const Card& newcard){
    m_deck.push_front(std::make_shared<Card>(newcard));
    if(m_always_shuffle) this->shuffle();
}
    //This accepts iterators pointing to Card
template <typename InIter>
void Deck::insert(InIter curr, InIter end){
    std::copy(curr, end, std::front_inserter(m_deck));
    if(m_always_shuffle) this->shuffle();
}
void Deck::shuffle(){
    Deck_t new_deck(0);
    while(m_deck.size() > 0){
        auto iter(m_deck.begin());
        advance(iter, m_gen() % m_deck.size());
        new_deck.push_front(*iter);
        m_deck.erase(iter);
    }
    m_deck = new_deck;
}
void Deck::reset(){
    if(m_trash.size() > 0)
        std::copy(
            m_trash.begin(), m_trash.end(),
            std::back_inserter(m_deck)
        ), m_trash.clear();
    this->sort();
    if(m_always_shuffle) this->shuffle();
}
void Deck::always_shuffle(bool inAlwaysShuffle)
    {m_always_shuffle = inAlwaysShuffle;}
    //Returns card from top of trash pile to bottom of deck
void Deck::return_card(size_t number_of_cards){
    Deck_t hold(0);
    while(number_of_cards-- > 0 && m_trash.size() > 0)
        hold.push_back(m_trash.back()), m_trash.pop_back();
    std::copy(
        hold.begin(), hold.end(),
        std::front_inserter(m_deck)
    );
    if(m_always_shuffle) this->shuffle();
}
void Deck::sort()
    {m_deck.sort([](Card_t f, Card_t s){return (*f) < (*s);});}

#include <chrono>
size_t get_seed(){
    return
        std::chrono::system_clock::now()
        . time_since_epoch()
        . count()
    ;
}
#include <functional>
//Constructors and Destructor
Deck::Deck(bool inAlwaysShuffle)
    : m_deck(0)
    , m_trash(0)
    , m_always_shuffle(inAlwaysShuffle)
    , m_gen(get_seed())
{}