#include "BJGame_Flow_Handler.h"

#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Basic_Logger.h"

#define log_if(...)             \
        if(log_enabled)         \
            log(__VA_ARGS__)    \

namespace{
    std::istream& read_line(std::string& dest, char delim = '\n');
    std::istream& read_number(unsigned long long& dest);

    void switch_log_state();

    const std::string
                        k_surrender("surrender"),
                        k_hit("hit"),
                        k_stand("stand"),
                        k_double_down("double down"),
                        k_split("split"),
                        k_switchlog("switch log")
    ;

    bool log_enabled(false);
}

size_t BJGame_Flow_Handler::players_left()const
    {return m_p_handle->players_left();}

void BJGame_Flow_Handler::play_turn(){
    using namespace std;
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        if(
            m_p_handle->out(*iter) ||
            m_p_handle->double_downed(*iter)
        ) continue;
        for(
            size_t hand(0);
            hand < m_p_handle->hand_count(*iter);
            ++hand
        ){
        //Dealer hand
            string hand_msg
                ("\nDealer's current hand:\n\tHidden, ");

            for(
                size_t i(m_p_handle->dealer().hand_size(hand)-1);
                i > 0;
                --i
            ){
                if(m_p_handle->dealer().card(i-1, hand) == nullptr){
                    const string deal_err("Error: Dealer with small hand.");
                    if(log_enabled){
                        log(deal_err);
                        switch_log_state();
                    }
                    throw runtime_error(deal_err.c_str());
                }
                hand_msg
                    += m_p_handle->dealer().card(i-1, hand)->name()
                    + (i != 1 ? ", " : "")
                ;
            }
            cout << hand_msg;
            log_if(hand_msg);

        //Player hand
            hand_msg.clear();
            hand_msg += "\n\n" + *iter + "'s current hand:\n\t";
            for(
                size_t i(m_p_handle->hand_size(*iter, hand));
                i > 0;
                --i
            ){
                hand_msg
                    += m_p_handle->card(i-1, *iter, hand)->name()
                    + (i != 1 ? ", " : "")
                ;
            }
            if(
                m_p_handle->hand_value(*iter, hand) == k_blackjack &&
                m_p_handle->hand_size(*iter, hand) == 2
            ) hand_msg += "\nBlack Jack!\n";
            cout << hand_msg;
            log_if(hand_msg + "\n");

        //Now choose what to do
            log_if(*iter + " is making a decision...");
            this->choose_action(*iter, hand);
        }
        if(std::distance(iter, m_names.end()) != 1){
            cout << "\n\nPress enter to start the next player's turn.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if(m_p_handle->players_left() > 0){
        while(!m_p_handle->dealer_hit());
        log_if("Players left:", m_p_handle->players_left());
    }
    log_if("No players left to play.");
}

void BJGame_Flow_Handler::start(){
#ifndef DEBUG_MODE
    using namespace std;

    cout << "\nAre there any new players who wish to join?";
    std::string newname("");
    while(newname != "none"){
        cout << " Please enter your name(\"none\" to skip): ";
        read_line(newname);
        cout << '\n';
        for(size_t i(0); i < newname.size(); ++i)
            ::tolower(newname[i]);

        if(newname == k_switchlog)
            switch_log_state();
        else if(newname != "none"){
            log_if("Added new player: " + newname);
            cout
                << "Now enter your starting amount, " << newname
                << "(0 to undo): $"
            ;
            BJPlayer::Money newamount(0);
            read_number(newamount);
            if(newamount == 0)
                continue;
            m_p_handle->add_player(newname, newamount);
            m_names.push_back(newname);
            log_if(newamount);
        }
    }
    cout << "\nTime to place your bets!";
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        if(m_p_handle->out(*iter)) continue;
        log_if(*iter + " placed a bet.");
        cout << '\n' << *iter << ", how much would you like to bet?\n";
        cout << "Type a whole number: $";
        BJPlayer::Money newbet(0);
        read_number(newbet);
        log_if(newbet);
        m_p_handle->place_bet(*iter, newbet);
        cout << "\n";
    }
    m_p_handle->deal();
#else
    
#endif
}

void BJGame_Flow_Handler::reset()
    {m_p_handle->reset();}
#include <sstream>
bool BJGame_Flow_Handler::display_round_end(){
    std::cout << "\nRound over!\n";
    log_if("Round over.");
//Dealer hand
    std::string msg("\nDealer's hand:\n\t");
    for(
        size_t i(m_p_handle->dealer().hand_size());
        i > 0;
        --i
    ){
        msg
            += m_p_handle->dealer().card(i-1)->name()
            + (i != 1 ? ", " : "")
        ;
    }
    std::cout << msg;
    log_if(msg);

    m_p_handle->calculate_winnings();
    msg.clear();
    using namespace std;
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        std::stringstream catalyst("");
        catalyst << m_p_handle->player(*iter).money();
        msg
            += "\n" + *iter + ", you have $" + catalyst.str() + ".\n"
            "Would you like to play again, " + *iter + "?"
        ;
        cout << msg;
        log_if(msg);

        do{
            cout << "\nPlease type \"yes\" or \"no\": ";
            std::string choice("");
            read_line(choice);
            cout << '\n';
            log_if("Did player want to play again? " + choice);

            for(size_t i(0); i < choice.size(); ++i)
                ::tolower(choice[i]);
            if(
                choice == "yes" &&
                m_p_handle->player(*iter).money() == 0
            ){
                cout
                    << "Please enter in a new money amount"
                    << "(0 to quit): $"
                ;
                BJPlayer::Money newamount(0);
                read_number(newamount);
                if(newamount > 0){
                    m_p_handle->erase_player(*iter);
                    m_p_handle->add_player(*iter, newamount);
                }
                log_if(*iter + " now playing with new amount.", newamount);
                break;
            }else if(choice == "yes"){
                log_if(
                    *iter + " currently has:",
                    m_p_handle->player(*iter).money()
                );
                break;
            }else if(choice == "no"){
                log_if(*iter + " has opted out.");
                m_p_handle->erase_player(*iter);
                --(iter = m_names.erase(iter));
                break;
            }else
                cout << "Unrecognised command. Please try again.";
        }while(true);
    }
    if(m_p_handle->players_left() > 0) return true;
    else if(log_enabled){
        log("Game over.");
        switch_log_state();
    }
    return false;
}

//Helpers
void BJGame_Flow_Handler::choose_action(
    const BJPlayer_Handler::Name& name,
    size_t& currHand
){
    using namespace std;
    cout << '\n' << name << ", what would you like to do?";
    while(true){
        cout
            << "\nType \"hit\", "
            << "\"double down\", "
            << "\"surrender\", "
            << "\"split\", "
            << "or \"stand\": "
        ;
        std::string choice("");
        read_line(choice);
        log_if("Player chose: \"" + choice + "\"");
        cout << '\n';
        for(size_t i(0); i < choice.size(); ++i)
            ::tolower(choice[i]);

        if(choice == k_surrender){
            m_p_handle->surrender(name);
            break;
        }else if(choice == k_hit){
            std::string subchoice("yes");
            while(subchoice == "yes"){
                bool bust(m_p_handle->hit(name, currHand));

                if(m_p_handle->last_card(name, currHand) == nullptr)
                    throw std::runtime_error("Empty hand.");
                const string drawn_msg(
                    "You drew a "
                    + m_p_handle
                        -> last_card(name, currHand)
                        -> name()
                    + "\n"
                );
                cout << drawn_msg;
                log_if(drawn_msg);

                if(bust){
                    cout << "BUST!";
                    log_if("Bust!");
                    break;
                }
                cout << "Hit again(\"yes\" or \"no\")? ";
                read_line(subchoice);
                log_if("Player hit again? " + subchoice);
            }
            break;
        }else if(choice == k_double_down){
            m_p_handle->double_down(name);

            if(m_p_handle->last_card(name, currHand) == nullptr)
                throw std::runtime_error("Empty hand.");
            const string drawn_msg(
                "You drew a "
                + m_p_handle
                    -> last_card(name, currHand)
                    -> name()
                + "\n"
            );
            cout << drawn_msg;
            log_if(drawn_msg);
            if(m_p_handle->out(name)){
                cout << "BUST!";
                log_if("BUST!");
            }
            break;
        }else if(choice == k_split){
                //decrement hand so next iteration goes back
                //  to the same hand
            if(
                ! m_p_handle
                -> split(
                    name
                    , (currHand > 0 ? currHand-- : currHand)
                )
            ){
                cout << "Cannot split";
                log_if("Split unsuccessful");
                ++currHand;
            }
        }else if(choice == k_stand)
            break;
        else if(choice == k_switchlog)
            switch_log_state();
        else
            cout << "Unrecognised command. Please try again.";
    }
}

#include <fstream>

namespace{
    std::istream& read_line(std::string& dest, char delim){
        std::getline(std::cin, dest, delim);
        return std::cin;
    }

    std::istream& read_number(unsigned long long& dest){
        using namespace std;
        while(!(std::cin >> dest)){
            cin.clear();
            cout
                << "\nError. Not an acceptable number. "
                << "Please enter a number: "
            ;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return std::cin;
    }

    void switch_log_state(){
        static std::ofstream dest("log.dat", std::ios_base::app);
        if((log_enabled = !log_enabled)){
            std::cout << "Logging enabled.";
            Basic_Logger::output = &dest;
            log("Logging started.");
        }else{
            std::cout << "Logging disabled.";
            log("Logging ended.");
            Basic_Logger::output = nullptr;
        }
    }
}