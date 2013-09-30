#ifndef DECK_H
#define DECK_H

#include "Card.h"

#include <list>
#include <random>
#include <memory>

class Deck{
    public:
        using Card_ptr    = std::shared_ptr<Card>;
        using Deck_t    = std::list<Card_ptr>;
        using RandGen   = std::mt19937;
    //Read-Only
        size_t size()const;
        size_t trash_size()const;
        const RandGen& generator()const;
        bool always_shuffle()const;
    //Other methods
        Card_ptr draw();
        //by default places card at the bottom of the deck
        void insert(const Card&);
        //This accepts iterators pointing to Card
        template <typename InIter>
            void insert(InIter, InIter);
        void shuffle();
        void reset();
        void always_shuffle(bool);
        //Returns card from top of trash pile to bottom of deck
        void return_card(size_t = 1);
        void sort();
    //Constructors and Destructor
        Deck(bool = false);
        //This accepts iterators pointing to Card
        template <typename InIter>
            Deck(InIter, InIter, bool = false);

        Deck(const Deck&)               = default;
        Deck(Deck&&)                    = default;
        Deck& operator=(const Deck&)    = default;
        Deck& operator=(Deck&&)         = default;
        ~Deck()                         = default;
    private:
        Deck_t
                            m_deck,
                            m_trash
        ;
        bool                m_always_shuffle;
        RandGen             m_gen;
};

size_t get_seed();
#include "Deck.inl"

#endif