#include "BJGame_Flow_Handler.h"

#include <memory>
#include <iostream>
#include <iomanip>
#include <list>
#include <utility>
#include <algorithm>
using std::cout;

std::ostream& PrintDeck(Deck);

std::istream& read_line(std::string& dest, char delim = '\n');
std::istream& read_number(unsigned long& dest);

constexpr long double ver(1.0);

int main(){
    cout << std::fixed << std::setprecision(1) << "Welcome to Black Jack v" << ver << '!';

    cout << "\nHow many players will there be: ";
    unsigned long pcount(0);
    read_number(pcount);
    std::pair<
        std::list<std::string>,
        std::list<unsigned long long>
    > newplayers({});
    while(pcount-- > 0){
        std::string newname("");
        while(true){
            cout << "\nPlayer " << (pcount+1) << ", please enter your name: ";
            read_line(newname);
            if(std::find(
                newplayers.first.begin(), newplayers.first.end(),
                newname
            ) == newplayers.first.end()) break;
            cout << "\nName taken. Please enter in a different name.\n";
        }
        unsigned long samount(0);
        cout << "\nNow enter your starting amount: $";
        read_number(samount);
        newplayers.first.push_back(newname);
        newplayers.second.push_back(samount);
    }

    BJGame_Flow_Handler game(
        newplayers.first.begin(), newplayers.first.end(),
        newplayers.second.begin()
    );
    try{
        do{
            game.start();
            game.play_turn();
        }while(game.display_round_end());
    }catch(const std::exception& e){
        cout << e.what();
    }

    cout << std::fixed << std::setprecision(1) << "\nThank you for playing Black Jack v" << ver << "!\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}

std::ostream& PrintDeck(Deck deck){
    while(deck.size() > 0){
        auto card(deck.draw());
        switch(card->value){
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                cout << card->value;
                break;
            case SuitValue::Joker:
                cout << "Joker";
                break;
            case SuitValue::Jack:
                cout << "Jack";
                break;
            case SuitValue::Queen:
                cout << "Queen";
                break;
            case SuitValue::King:
                cout << "King";
                break;
            case SuitValue::Ace:
                cout << "Ace";
                break;
            default:
                cout << "Error";
                break;
        };
        cout << "\tof ";
        switch(card->suit){
            case Suit_t::Clubs:
                cout << "Clubs.";
                break;
            case Suit_t::Hearts:
                cout << "Hearts.";
                break;
            case Suit_t::Spades:
                cout << "Spades.";
                break;
            case Suit_t::Diamonds:
                cout << "Diamonds.";
                break;
            case Suit_t::None:
            default:
                cout << "Errors.";
                break;
        };
        cout << '\n';
    }
    return cout;
}

std::istream& read_line(std::string& dest, char delim){
    std::getline(std::cin, dest, delim);
    return std::cin;
}

std::istream& read_number(unsigned long& dest){
    using namespace std;
    while(!(cin >> dest)){
        cin.clear();
        cout
            << "\nError. Not an acceptable number. "
            << "Please enter a number: "
        ;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return cin;
}
