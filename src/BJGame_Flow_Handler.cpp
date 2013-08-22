#include "BJGame_Flow_Handler.h"

#include <cctype>

#include <iostream>
#include <limits>

std::istream& read_line(std::string& dest, char delim = '\n');
std::istream& read_number(unsigned long long& dest);

const std::string
                    k_surrender("surrender"),
                    k_hit("hit"),
                    k_stand("stand"),
                    k_double_down("double down")
;

size_t BJGame_Flow_Handler::players_left()const
    {return m_player_handler->players_left();}
bool BJGame_Flow_Handler::play_turn(){
    using namespace std;
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        if(
            m_player_handler->out(*iter) ||
            m_player_handler->double_downed(*iter)
        ) continue;
    //Dealer hand
        cout << "\nDealer's current hand:\n\tHidden, ";
        for(
            size_t i(m_player_handler->dealer().hand_size()-1);
            i > 0;
            --i
        ){
            cout
                << m_player_handler->dealer().card(i-1)->name()
                << (i != 1 ? ", " : "")
            ;
        }
    //Player hand
        cout << "\n\n" << *iter << "'s current hand:\n\t";
        for(
            size_t i(m_player_handler->player(*iter).hand_size());
            i > 0;
            --i
        ){
            cout
                << m_player_handler->player(*iter).card(i-1)->name()
                << (i != 1 ? ", " : "")
            ;
        }
        if(
            m_player_handler->player(*iter).hand_value() == k_blackjack &&
            m_player_handler->player(*iter).hand_size() == 2
        ) cout << "\nBlack Jack!\n";

    //Now choose what to do
        this->choose_action(*iter);
        
    //Display Player hand updated
        cout << "\n" << *iter << "'s current hand:\n\t";
        for(
            size_t i(m_player_handler->player(*iter).hand_size());
            i > 0;
            --i
        ){
            cout
                << m_player_handler->player(*iter).card(i-1)->name()
                << (i != 1 ? ", " : "")
            ;
        }

        cout << "\n\nPress enter to start the next player's turn.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if(m_player_handler->players_left() > 0)
        return m_player_handler->dealer_hit();
    else
        return true;
}

void BJGame_Flow_Handler::start(){
    using namespace std;
    cout << "\nAre there any new players who wish to join?";
    std::string newname("");
    while(newname != "none"){
        cout << " Please enter your name(\"none\" to skip): ";
        read_line(newname);
        cout << '\n';
        for(size_t i(0); i < newname.size(); ++i)
            ::tolower(newname[i]);
        if(newname != "none"){
            cout
                << "Now enter your starting amount, " << newname
                << "(0 to undo): $"
            ;
            BJPlayer::Money newamount(0);
            read_number(newamount);
            m_player_handler->add_player(newname, newamount);
        }
    }
    cout << "\nTime to place your bets!";
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        if(m_player_handler->out(*iter)) continue;
        cout << '\n' << *iter << ", how much would you like to bet?\n";
        cout << "Type a whole number: $";
        BJPlayer::Money newbet(0);
        read_number(newbet);
        m_player_handler->place_bet(*iter, newbet);
        cout << "\n";
    }
    m_player_handler->deal();
}

void BJGame_Flow_Handler::reset()
    {m_player_handler->reset();}

bool BJGame_Flow_Handler::display_round_end(){
    std::cout << "\nRound over!\n";
//Dealer hand
    std::cout << "\nDealer's hand:\n\t";
    for(
        size_t i(m_player_handler->dealer().hand_size());
        i > 0;
        --i
    ){
        std::cout
            << m_player_handler->dealer().card(i-1)->name()
            << (i != 1 ? ", " : "")
        ;
    }
    m_player_handler->calculate_winner();
    using namespace std;
    for(auto iter(m_names.begin()); iter != m_names.end(); ++iter){
        cout
            << '\n' << *iter << ", you have $"
            << m_player_handler->player(*iter).money()
            << '.'
        << '\n';
        cout << "Would you like to play again, " << *iter << '?';
        do{
            cout << "\nPlease type \"yes\" or \"no\": ";
            std::string choice("");
            read_line(choice);
            cout << '\n';
            for(size_t i(0); i < choice.size(); ++i)
                ::tolower(choice[i]);
            if(
                choice == "yes" &&
                m_player_handler->player(*iter).money() == 0
            ){
                cout
                    << "Please enter in a new money amount"
                    << "(0 to quit): $"
                ;
                BJPlayer::Money newamount(0);
                read_number(newamount);
                if(newamount > 0)
                    m_player_handler->return_player(*iter, newamount);
                break;
            }else if(choice == "yes")
                break;
            else if(
                choice == "no" &&
                m_player_handler->player(*iter).money() == 0
            ){
                m_player_handler->erase_player(*iter);
                break;
            }else if(choice == "no"){
                m_player_handler->remove_player(*iter);
                break;
            }else
                cout << "Unrecognised command. Please try again.";
        }while(true);
    }
    if(m_player_handler->players_left() > 0) return true;
    else return false;
}

//Helpers
void BJGame_Flow_Handler::choose_action(
    const BJPlayer_Handler::Name& name,
    size_t hand_index
){
    using namespace std;
    cout << '\n' << name << ", what would you like to do?";
    while(true){
        cout
            << "\nType \"hit\", "
            << "\"double down\", "
            << "\"surrender\", "
            << "or \"stand\": "
        ;
        std::string choice("");
        read_line(choice);
        cout << '\n';
        for(size_t i(0); i < choice.size(); ++i)
            ::tolower(choice[i]);
        if(choice == k_surrender){
            m_player_handler->surrender(name);
            break;
        }else if(choice == k_hit){
            std::string subchoice("yes");
            while(subchoice == "yes"){
                m_player_handler->hit(name);
                cout
                    << "You drew a "
                    << m_player_handler
                        ->  player(name)
                        .   card(
                                m_player_handler
                                ->  player(name)
                                .   hand_size(hand_index)-1
                            )
                        ->  name()
                << '\n';
                if(m_player_handler->out(name)){
                    cout << "BUST!";
                    break;
                }
                cout << "Hit again(\"yes\" or \"no\")? ";
                read_line(subchoice);
            }
            break;
        }else if(choice == k_double_down){
            m_player_handler->double_down(name);
            cout
                << "You drew a "
                << m_player_handler
                    ->  player(name)
                    .   card(
                            m_player_handler
                            ->  player(name)
                            .   hand_size(hand_index)-1
                        )
                    ->  name()
            << '\n';
            if(m_player_handler->out(name))
                cout << "BUST!";
            break;
        }else if(choice == k_stand)
            break;
        else
            cout << "Unrecognised command. Please try again.";
    }
}

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