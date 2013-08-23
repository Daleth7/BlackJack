#include "BJPlayer.h"

BJPlayer::Money BJPlayer::money()const
    {return m_money;}
size_t BJPlayer::hand_size(size_t hand_index)const{
    if(hand_index >= m_hands.size()) return 0;
    return m_hands[hand_index].size();
}
size_t BJPlayer::hand_count()const
    {return m_hands.size();}
Deck::Card_t BJPlayer::card(
    size_t card_index,
    size_t hand_index
)const{
    if(
        hand_index >= m_hands.size() ||
        card_index >= m_hands[hand_index].size()
    ) return nullptr;
    return m_hands[hand_index][card_index];
}
Deck::Card_t BJPlayer::last_card(size_t hand_index)const{
    if(hand_index >= m_hands.size()) return nullptr;
    return m_hands.at(hand_index).back();
}

size_t BJPlayer::hand_value(size_t hand_index)const{
    if(
        hand_index >= m_hands.size() ||
        m_hands[hand_index].size() == 0
    ) return 0;
    size_t toreturn(0);
    bool ace_present(false);
    for(size_t i(0); i < m_hands[hand_index].size(); ++i){
        if(m_hands[hand_index][i]->value == SuitValue::Ace)
            ace_present = true;
        if(
            m_hands[hand_index][i]->value == SuitValue::Jack ||
            m_hands[hand_index][i]->value == SuitValue::Queen ||
            m_hands[hand_index][i]->value == SuitValue::King
        ) toreturn += 10;
        else toreturn += m_hands[hand_index][i]->value;
    }
    if(ace_present && toreturn <= 11) toreturn += 10;
    return toreturn;
}

void BJPlayer::add_card(
    const Deck::Card_t& newcard,
    size_t hand_index
){
    if(hand_index >= m_hands.size()) return;
    m_hands[hand_index].push_back(newcard);
}
void BJPlayer::clear_hand(size_t hand_index){
    if(hand_index >= m_hands.size()) return;
    if(hand_index == 0)
        m_hands[hand_index].clear();
    else
        m_hands.erase(m_hands.begin() + hand_index);
}
void BJPlayer::clear_all_hands(){
    m_hands.resize(1);
    m_hands[0].clear();
}
BJPlayer::Money BJPlayer::bet(Money request){
    if(m_money == 0) return 0;
    else if(request > m_money) request = m_money;
    m_money -= request;
    return request;
}
void BJPlayer::add_money(Money newmoney)
    {m_money += newmoney;}
void BJPlayer::split(
    size_t hand_index,
    Deck::Card_t newcard1,
    Deck::Card_t newcard2
){
    if(
        hand_index >= m_hands.size() ||
        m_hands[hand_index].size() != 2 ||
        m_hands[hand_index].front()->value
            != m_hands[hand_index].back()->value 
    ) return;
    m_hands.insert(
        m_hands.begin()+hand_index+1,
        {m_hands[hand_index].back()}
    );
    m_hands[hand_index].pop_back();
    if(newcard1 != nullptr) m_hands[hand_index].push_back(newcard1);
    if(newcard2 != nullptr) m_hands[hand_index+1].push_back(newcard2);
}

BJPlayer::BJPlayer(Money starting_amount)
    : m_hands(1)
    , m_money(starting_amount > 0
        ? starting_amount : k_default_starting_amount)
{}