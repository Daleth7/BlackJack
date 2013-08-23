#ifndef BJPLAYER_H
#define BJPLAYER_H

#include "Deck.h"

#include <vector>

    //This class will always ensure the player has at least one hand
class BJPlayer{
    public:
        using Hand              = std::vector<Deck::Card_t>;
        using Hand_Collection   = std::vector<Hand>;
        using Money             = unsigned long long int;
    //Read-only
        Money money()const;
        size_t hand_size(size_t hand_index = 0)const;
        size_t hand_count()const;
        Deck::Card_t card(
            size_t card_index = 0,
            size_t hand_index = 0
        )const;
        Deck::Card_t last_card(size_t hand_index = 0)const;
        size_t hand_value(size_t hand_index = 0)const;
    //Modifiers
        void add_card(const Deck::Card_t&, size_t hand_index = 0);
        void clear_hand(size_t hand_index = 0);
        void clear_all_hands();
        Money bet(Money);
        void add_money(Money);
        void split(
            size_t hand_index = 0,
            Deck::Card_t = nullptr,
            Deck::Card_t = nullptr
        );
    //Constructors and destructor
        BJPlayer(Money = k_default_starting_amount);
        BJPlayer(const BJPlayer&)             = default;
        BJPlayer(BJPlayer&&)                  = default;
        BJPlayer& operator=(const BJPlayer&)  = default;
        BJPlayer& operator=(BJPlayer&&)       = default;
        ~BJPlayer()                           = default;
    private:
        Hand_Collection m_hands;
        Money           m_money;
        static constexpr Money k_default_starting_amount = 1000;
};

#endif