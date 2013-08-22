#include "SuitDeckFactory.h"

template <typename NameIter, typename MoneyIter>
std::shared_ptr<BJPlayer_Handler> BJPlayer_Handler::instance(
    NameIter currname,
    NameIter endname,
    MoneyIter curramount
){
    static std::shared_ptr<BJPlayer_Handler>
        singleton_ptr(std::make_shared<BJPlayer_Handler>(BJPlayer_Handler(
            currname, endname,
            curramount
        )));
    return singleton_ptr;
}

template <typename NameIter, typename MoneyIter>
BJPlayer_Handler::BJPlayer_Handler(
    NameIter currname,
    NameIter endname,
    MoneyIter curramount
)
    : m_dealer(0)
    , m_players({})
    , m_out_players({})
    , m_deck(SuitDeckFactory().generate())
    , m_pots({})
    , m_double_downed({})
    , k_starting_amount(*curramount)
{
    while(currname != endname)
        m_players.insert(std::make_pair(
            *(currname++),
            BJPlayer(*(curramount++))
        ));
    this->arbitrary_shuffle();
}

template <class Container>
bool BJPlayer_Handler::check_key(
    Name& testee,
    const Container& container
)const{
    if(testee.empty()) testee = m_players.begin()->first;
    if(container.find(testee) != container.end())
        return true;
    else return false;
}