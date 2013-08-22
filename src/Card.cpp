#include "Card.h"

short compare(Suit_t f, Suit_t s){
    short result(0);
    switch(f){
        case Suit_t::Hearts:     result += 4; break;
        case Suit_t::Spades:     result += 3; break;
        case Suit_t::Diamonds:   result += 2; break;
        case Suit_t::Clubs:      result += 1; break;
        case Suit_t::None:
        default:                 result += 10;break;
    };
    switch(s){
        case Suit_t::Hearts:     result -= 4; break;
        case Suit_t::Spades:     result -= 3; break;
        case Suit_t::Diamonds:   result -= 2; break;
        case Suit_t::Clubs:      result -= 1; break;
        case Suit_t::None:
        default:                 result -= 10;break;
    };
    return result;
}
bool operator==(Suit_t lhs, Suit_t rhs)
    {return compare(lhs, rhs) == 0;}
bool operator!=(Suit_t lhs, Suit_t rhs)
    {return !(lhs == rhs);}
bool operator>=(Suit_t lhs, Suit_t rhs){
    short result(compare(lhs, rhs));
    return result == 0 || result > 0;
}
bool operator<(Suit_t lhs, Suit_t rhs)
    {return !(lhs >= rhs);}
bool operator<=(Suit_t lhs, Suit_t rhs){
    short result(compare(lhs, rhs));
    return result == 0 || result < 0;
}
bool operator>(Suit_t lhs, Suit_t rhs)
    {return !(lhs <= rhs);}

short compare(Uno_t f, Uno_t s){
    short result(0);
    switch(f){
        case Uno_t::Red:    result += 5; break;
        case Uno_t::Blue:   result += 4; break;
        case Uno_t::Green:  result += 3; break;
        case Uno_t::Yellow: result += 2; break;
        case Uno_t::Black:  result += 1; break;
        case Uno_t::None:
        default:            result += 10;break;
    };
    switch(s){
        case Uno_t::Red:    result -= 5; break;
        case Uno_t::Blue:   result -= 4; break;
        case Uno_t::Green:  result -= 3; break;
        case Uno_t::Yellow: result -= 2; break;
        case Uno_t::Black:  result -= 1; break;
        case Uno_t::None:
        default:            result -= 10;break;
    };
    return result;
}
bool operator==(Uno_t lhs, Uno_t rhs)
    {return compare(lhs, rhs) == 0;}
bool operator!=(Uno_t lhs, Uno_t rhs)
    {return !(lhs == rhs);}
bool operator>=(Uno_t lhs, Uno_t rhs)
    {return !(lhs < rhs);}
bool operator<(Uno_t lhs, Uno_t rhs)
    {return compare(lhs, rhs) < 0;}
bool operator<=(Uno_t lhs, Uno_t rhs)
    {return !(lhs > rhs);}
bool operator>(Uno_t lhs, Uno_t rhs)
    {return compare(lhs, rhs) > 0;}

std::string Card::name()const{
    if(cardtype & Card::UnoCard) return this->make_uno_name();
    else return this->make_suit_name();
}
bool Card::operator==(const Card& rhs){
    return this->value == rhs.value;
    if(cardtype & Card::SuitCard)
        return this->suit == rhs.suit;
    if(cardtype & Card::UnoCard)
        return this->color == rhs.color;
}
bool Card::operator!=(const Card& rhs)
    {return !(*this == rhs);}
bool Card::operator>=(const Card& rhs)
    {return !(*this < rhs);}
bool Card::operator<=(const Card& rhs)
    {return !(*this > rhs);}
bool Card::operator>(const Card& rhs){
    if(this->suit > rhs.suit)           return true;
    else if(this->color > rhs.color)    return true;
    else                                return this->value > rhs.value;
}
bool Card::operator<(const Card& rhs){
    if(this->suit < rhs.suit)           return true;
    else if(this->color < rhs.color)    return true;
    else                                return this->value < rhs.value;
}

std::string Card::make_suit_name()const{
    std::string toreturn("");
    switch(this->value){
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            toreturn.push_back(this->value + '0');
            break;
        case 10:
            toreturn += "10";
            break;
        case SuitValue::Joker:
            toreturn += "Joker";
            break;
        case SuitValue::Jack:
            toreturn += "Jack";
            break;
        case SuitValue::Queen:
            toreturn += "Queen";
            break;
        case SuitValue::King:
            toreturn += "King";
            break;
        case SuitValue::Ace:
            toreturn += "Ace";
            break;
        default:
            toreturn += "Error";
            break;
    };
    toreturn += " of ";
    switch(this->suit){
        case Suit_t::Clubs:
            toreturn += "Clubs";
            break;
        case Suit_t::Hearts:
            toreturn += "Hearts";
            break;
        case Suit_t::Spades:
            toreturn += "Spades";
            break;
        case Suit_t::Diamonds:
            toreturn += "Diamonds";
            break;
        case Suit_t::None:
        default:
            toreturn += "Errors";
            break;
    };
    return toreturn;
}
std::string Card::make_uno_name()const{
    std::string toreturn("");
    switch(this->color){
        case Uno_t::Blue:
            toreturn += "Blue";
            break;
        case Uno_t::Red:
            toreturn += "Red";
            break;
        case Uno_t::Green:
            toreturn += "Green";
            break;
        case Uno_t::Yellow:
            toreturn += "Yellow";
            break;
        case Uno_t::Black:
            toreturn += "Black";
            break;
        case Uno_t::None:
        default:
            toreturn += "Error";
            break;
    };
    toreturn.push_back(' ');
    switch(this->value){
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            toreturn.push_back(this->value + '0');
            break;
        case UnoValue::Skip:
            toreturn += "Skip";
            break;
        case UnoValue::Draw_Two:
            toreturn += "Draw Two";
            break;
        case UnoValue::Reverse:
            toreturn += "Reverse";
            break;
        case UnoValue::Wild:
            toreturn += "Wild";
            break;
        case UnoValue::Wild_Draw_Four:
            toreturn += "Wild Draw Four";
            break;
        default:
            toreturn += "Error";
            break;
    };
    return toreturn;
}