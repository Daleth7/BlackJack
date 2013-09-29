#ifndef BJPLAYER_HANDLER_H
#define BJPLAYER_HANDLER_H

#include "BJPlayer.h"
class Deck;

#include <map>
#include <memory>
#include <list>
    //This class only manages calls to member functions and a couple
    //  other factors. Any functionality involving input or GUI will
    //  have to be handled via some other method.
    //Functions that take a Name parameter and have default arguments
    //  should only be used without explicit arguments if and only if
    //  there is only one player.
class BJPlayer_Handler{
    public:
        using Name              = std::string;
        using HandPots          = std::vector<BJPlayer::Money>;
        using HandDoubleDowns   = std::list<size_t>;

        template <typename NameIter, typename MoneyIter>
        static std::shared_ptr<BJPlayer_Handler> instance(
            NameIter, NameIter,
            MoneyIter
        );
    //Read-only
        size_t players_left()const;
        BJPlayer player(Name = "")const;
        bool player_exists(Name)const;
        size_t hand_size(Name = "", size_t hand_index = 0)const;
        size_t hand_count(Name = "")const;
        size_t hand_value(Name = "", size_t hand_index = 0)const;
        Deck::Card_ptr card(
            size_t card_index = 0,
            Name = "",
            size_t hand_index = 0
        )const;
        Deck::Card_ptr last_card(Name = "", size_t hand_index = 0)const;
        bool out(Name = "")const;
        BJPlayer::Money pot(Name = "", size_t hand_index = 0)const;
        const BJPlayer& dealer()const;
        bool double_downed(Name = "", size_t hand_index = 0)const;
        size_t count_double_downed(Name = "")const;
    //Player Actions
            //Reset everything from deck to players' money
        void reset();
            //Start of a round; deals two cards to each player
        void deal();
        void place_bet(Name = "", BJPlayer::Money = k_default_bet);
            //Return whether or not the player's hand is a bust
        bool hit(Name = "", size_t hand_index = 0);
            //Return whether or not dealer has exceeded maximum yet
        bool dealer_hit();
        void surrender(Name = "");
        void double_down(Name = "", size_t hand_index = 0);
            //Return whether or not the split was successful
        bool split(Name = "", size_t hand_index = 0);
    //Other modifiers
            //Clean up cards and award pots to the winners
        void calculate_winner();
        void remove_player(Name = "");
        void erase_player(Name = "");
        void add_player(
            const Name& = "",
            BJPlayer::Money = k_default_starting_amount
        );
        void return_player(
            Name = "",
            BJPlayer::Money = k_default_starting_amount
        );
    //Constructors and destructor
        BJPlayer_Handler(const BJPlayer_Handler&)               = delete;
        BJPlayer_Handler(BJPlayer_Handler&&)                    = default;
        BJPlayer_Handler& operator=(const BJPlayer_Handler&)    = delete;
        BJPlayer_Handler& operator=(BJPlayer_Handler&&)         = delete;
        ~BJPlayer_Handler()                                     = default;
    protected:
        template <typename NameIter, typename MoneyIter>
        BJPlayer_Handler(
            NameIter, NameIter,
            MoneyIter
        );
    //Helpers
        void arbitrary_shuffle();
            //Returns whether or not the key is valid, i.e. player exists
        template <class Container>
            bool check_key(Name&, const Container&)const;
            //Need to centralize transfer system
        template <class Mapped, typename Key>
            void transfer_item(Mapped& src, Mapped& dest, Key key);
    private:
        BJPlayer                            m_dealer;
        std::map<Name, BJPlayer>
                                            m_players,
                                            m_out_players
        ;
        std::unique_ptr<Deck>               m_deck;
        std::map<Name, HandPots>            m_pots;
        std::map<Name, HandDoubleDowns>     m_double_downed;
        const BJPlayer::Money               k_starting_amount;
        static constexpr BJPlayer::Money
                                            k_default_bet = 20,
                                            k_default_starting_amount = 1000
        ;
        static constexpr size_t
                                            k_blackjack = 21,
                                            k_dealer_max = 17
        ;
};

#include "BJPlayer_Handler.inl"

#endif
