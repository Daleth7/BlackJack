#ifndef BJGAME_FLOW_HANDLER
#define BJGAME_FLOW_HANDLER

#include "BJPlayer_Handler.h"

class BJGame_Flow_Handler{
    public:
        size_t players_left()const;
        void play_turn();
        void start();
        void reset();
            //Return whether or not there is still a player
            //  who wants to play another round
        bool display_round_end();
        template <typename NameIter, typename MoneyIter>
        BJGame_Flow_Handler(NameIter, NameIter, MoneyIter);
    protected:
    //Helpers
        void choose_action(
            const BJPlayer_Handler::Name&,
            size_t& hand_index
        );
    private:
        std::shared_ptr<BJPlayer_Handler>       m_p_handle;
        std::list<BJPlayer_Handler::Name>       m_names;

        static constexpr size_t                 k_blackjack = 21;
};

#include "BJGame_Flow_Handler.inl"

#endif