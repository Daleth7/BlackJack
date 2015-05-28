#include "BJPlayer_Handler.h"
#include "SuitDeckFactory.h"

#include <random>
#include <chrono>
#include <algorithm>

size_t BJPlayer_Handler::players_left()const
    {return m_players.size();}
BJPlayer BJPlayer_Handler::player(Name key)const{
    if(check_key(key, m_players)) return m_players.at(key);
    else if(check_key(key, m_out_players))
        return m_out_players.at(key);
    else return BJPlayer(0);
}

bool BJPlayer_Handler::player_exists(Name key)const
    {return check_key(key, m_players) || check_key(key, m_out_players);}

size_t BJPlayer_Handler::hand_size(Name key, size_t hand_index)const{
    if(!check_key(key, m_players)) return 0;
    else return m_players.at(key).hand_size(hand_index);
}
size_t BJPlayer_Handler::hand_count(Name key)const{
    if(!check_key(key, m_players)) return 0;
    else return m_players.at(key).hand_count();
}
size_t BJPlayer_Handler::hand_value(
    Name key,
    size_t hand_index
)const{
    if(!check_key(key, m_players)) return 0;
    return m_players.at(key).hand_value(hand_index);
}

Deck::Card_ptr BJPlayer_Handler::card(
    size_t card_index,
    Name key,
    size_t hand_index
)const{
    if(!check_key(key, m_players)) return nullptr;
    return m_players.at(key).card(card_index, hand_index);
}

Deck::Card_ptr BJPlayer_Handler::last_card(
    Name key,
    size_t hand_index
)const{
    if(check_key(key, m_players))
        return m_players.at(key).last_card(hand_index);
    else if(check_key(key, m_out_players))
        return m_out_players.at(key).last_card(hand_index);
    else return nullptr;
}
bool BJPlayer_Handler::out(Name key)const{
    if(key.empty() && m_players.size() == 0) return true;
    else if(key.empty()) return false;
    if(m_out_players.find(key) != m_out_players.end())
        return true;
    else return false;
}

BJPlayer::Money BJPlayer_Handler::pot(
    Name key,
    size_t hand_index
)const{
    if(check_key(key, m_pots)) return m_pots.at(key)[hand_index];
    else return 0;
}
const BJPlayer& BJPlayer_Handler::dealer()const
    {return m_dealer;}
bool BJPlayer_Handler::double_downed(
    Name key,
    size_t hand_index
)const{
    if(!check_key(key, m_double_downed)) return false;
    if(std::find(
        m_double_downed.at(key).begin(),
        m_double_downed.at(key).end(),
        hand_index
    ) != m_double_downed.at(key).end()) return true;
    return false;
}
size_t BJPlayer_Handler::count_double_downed(Name key)const{
    if(key.empty()) return m_double_downed.size();
    else if(check_key(key, m_double_downed))
        return m_double_downed.at(key).size();
    else return 0;
}

void BJPlayer_Handler::reset(){
    m_dealer = BJPlayer(0);
    std::copy(
        m_out_players.begin(), m_out_players.end(),
        std::inserter(m_players, m_players.end())
    );
    m_out_players.clear();
    for(auto iter(m_players.begin()); iter != m_players.end(); ++iter)
        iter->second = BJPlayer(k_starting_amount);
    m_deck->reset();
    this->arbitrary_shuffle();
    m_pots.clear();
    m_double_downed.clear();
}

void BJPlayer_Handler::deal(){
    if(m_deck->size() <= m_players.size() * 2){
        m_deck->reset();
        this->arbitrary_shuffle();
    }
    for(auto iter(m_players.begin()); iter != m_players.end(); ++iter){
        iter->second.add_card(m_deck->draw());
        iter->second.add_card(m_deck->draw());
    }
    m_dealer.add_card(m_deck->draw());
    m_dealer.add_card(m_deck->draw());
}
void BJPlayer_Handler::place_bet(Name key, BJPlayer::Money amount){
    if(!check_key(key, m_players)) return;
    m_pots[key].push_back(m_players[key].bet(amount));
}
bool BJPlayer_Handler::hit(Name key, size_t hand_index){
    if(!check_key(key, m_players)) return false;
    m_players[key].add_card(m_deck->draw(), hand_index);
    if(m_players[key].hand_value(hand_index) > k_blackjack){
        this->transfer_item(m_players, m_out_players, key);
        return true;
    }
    return false;
}
    //Return whether or not dealer has exceeded maximum yet
bool BJPlayer_Handler::dealer_hit(){
    m_dealer.add_card(m_deck->draw());
    if(m_dealer.hand_value() > k_dealer_max)
        return true;
    return false;
}
void BJPlayer_Handler::surrender(Name key){
    if(!check_key(key, m_players)) return;
    m_players[key].clear_hand();
    if(m_players[key].hand_count() == 1)
        this->transfer_item(m_players, m_out_players, key);
}
void BJPlayer_Handler::double_down(Name key, size_t hand_index){
    if(
        !check_key(key, m_players) ||
        m_pots.at(key).at(hand_index) == 0
    ) return;
    auto additional_pot(m_players[key].bet(m_pots.at(key).at(hand_index)));
    m_players[key].add_card(m_deck->draw(), hand_index);
    m_pots[key][hand_index] += additional_pot;
    m_double_downed[key].push_back(hand_index);
    if(m_players[key].hand_value(hand_index) > k_blackjack)
        this->transfer_item(m_players, m_out_players, key);
}
bool BJPlayer_Handler::split(Name key, size_t hand_index){
    if(
        !check_key(key, m_players) ||
        !m_players[key].split(hand_index, m_deck->draw(), m_deck->draw())
    ) return false;
    m_pots[key].insert(
        m_pots[key].begin() + hand_index + 1,
        m_pots[key][hand_index]
    );
    return true;
}

void BJPlayer_Handler::calculate_winnings(){
    auto dealer_hand_value(m_dealer.hand_value() > k_blackjack
        ? 0 : m_dealer.hand_value());
    for(auto iter(m_players.begin()); iter != m_players.end(); ++iter){
        for(size_t hand(0); hand < iter->second.hand_count(); ++hand){
            if(iter->second.hand_value(hand) > k_blackjack){
                iter->second.clear_hand(hand);
                continue;
            }else if( //BlackJack!
                (
                    iter->second.hand_value(hand) == k_blackjack &&
                    iter->second.hand_size(hand) == 2 &&
                    m_dealer.hand_size(hand) != 2
                ) ||
                (
                    iter->second.hand_value(hand) == k_blackjack &&
                    iter->second.hand_size(hand) == 2 &&
                    dealer_hand_value != k_blackjack
                )
            )   iter->second.add_money(3 * m_pots[iter->first][hand]);
            else if(iter->second.hand_value(hand) > dealer_hand_value)
                iter->second.add_money(2 * m_pots[iter->first][hand]);
            else if(iter->second.hand_value(hand) == dealer_hand_value)
                iter->second.add_money(m_pots[iter->first][hand]);
            iter->second.clear_hand(hand);
        }
    }
    for(
        auto iter(m_out_players.begin());
        iter != m_out_players.end();
        ++iter
    ) iter->second.clear_hand();
    std::copy_if(
        m_out_players.begin(), m_out_players.end(),
        std::inserter(m_players, m_players.end()),
        [](const std::pair<Name, BJPlayer>& testee)
            {return testee.second.money() > 0;}
    );
//Clean up
    m_pots.clear();
    m_dealer.clear_hand();
    m_double_downed.clear();
    m_out_players.clear();
}
void BJPlayer_Handler::remove_player(Name key){
    if(!check_key(key, m_players)) return;
    this->transfer_item(m_players, m_out_players, key);
}

void BJPlayer_Handler::erase_player(Name key){
    if(check_key(key, m_players))
        m_players.erase(key);
    else if(check_key(key, m_out_players))
        m_out_players.erase(key);
}
void BJPlayer_Handler::add_player(
    const Name& newname,
    BJPlayer::Money newamount
){
    if(
        m_players.find(newname) != m_players.end() ||
        m_out_players.find(newname) != m_out_players.end()
    ) return;
    m_players.insert(std::make_pair(newname, BJPlayer(newamount)));
}
void BJPlayer_Handler::return_player(
    Name key,
    BJPlayer::Money newamount
){
    if(!check_key(key, m_out_players)) return;
    m_out_players[key].add_money(newamount);
    this->transfer_item(m_out_players, m_players, key);
}

void BJPlayer_Handler::arbitrary_shuffle(){
    //Completely arbitrary shuffling
    const size_t max_shuffles(10);
    size_t shuffle_count(
        std::mt19937(
            std::chrono::system_clock::now().time_since_epoch().count()
        )()
        % max_shuffles + 1
    );
    while(shuffle_count-- > 0) m_deck->shuffle();
}