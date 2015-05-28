#ifndef CARD_H
#define CARD_H

//By default all comparison functions for
//   card types(e.g. suits or Uno) will
//   return:
//      1) -1 for less than
//      1) 0 for equal to
//      1) 1 for greater than
//In addition, all weakly-typed enumerations
//   shall be wrapped in a namespace to avoid
//   name conflicts. However, they shall
//   still be able to be implicitly cast
//   to primitive types.

//Standard 4-suit deck
enum class Suit_t{
    Hearts,
    Spades,
    Diamonds,
    Clubs,
    None
};
short compare(Suit_t f, Suit_t s);
bool operator==(Suit_t lhs, Suit_t rhs);
bool operator!=(Suit_t lhs, Suit_t rhs);
bool operator>=(Suit_t lhs, Suit_t rhs);
bool operator<(Suit_t lhs, Suit_t rhs);
bool operator<=(Suit_t lhs, Suit_t rhs);
bool operator>(Suit_t lhs, Suit_t rhs);

namespace SuitValue{
    enum : unsigned{
        Ace = 1,
        Jack = 11,
        Queen = 12,
        King = 13,
        Joker = 0
    };
}

//Standard Uno deck
enum class Uno_t{
    Red,
    Blue,
    Green,
    Yellow,
    Black,
    None
};
short compare(Uno_t f, Uno_t s);
bool operator==(Uno_t lhs, Uno_t rhs);
bool operator!=(Uno_t lhs, Uno_t rhs);
bool operator>=(Uno_t lhs, Uno_t rhs);
bool operator<(Uno_t lhs, Uno_t rhs);
bool operator<=(Uno_t lhs, Uno_t rhs);
bool operator>(Uno_t lhs, Uno_t rhs);

namespace UnoValue{
    enum : unsigned{
        Skip = 10,
        Draw_Two = 11,
        Reverse = 12,
        Wild = 13,
        Wild_Draw_Four = 14
    };
}

#include <string>

struct Card{
    std::string name()const;

    Card(unsigned inValue, Suit_t inSuit)
        : value((inValue > 0 && inValue < SuitValue::King+1)
            ? inValue : SuitValue::Joker)
        , suit(inSuit)
        , cardtype(Card::SuitCard)
    {}
    Card(unsigned inValue, Uno_t inColor)
        : value(inValue < UnoValue::Wild_Draw_Four ? inValue : 0)
        , color(inColor)
        , cardtype(Card::UnoCard)
    {}

    bool operator==(const Card& rhs);
    bool operator!=(const Card& rhs);
    bool operator>=(const Card& rhs);
    bool operator<=(const Card& rhs);
    bool operator>(const Card& rhs);
    bool operator<(const Card& rhs);

    Card(const Card&)               = default;
    Card(Card&&)                    = default;
    Card& operator=(const Card&)    = delete;
    Card& operator=(Card&&)         = delete;
    ~Card()                         = default;
    const unsigned    value;
    union{
        const Suit_t       suit;
        const Uno_t       color;
    };

    protected:
        const char cardtype;
    //Helpers
        std::string make_suit_name()const;
        std::string make_uno_name()const;

        enum : char{
            SuitCard = 1 << 0,
            UnoCard = 1 << 1
        };
};

#endif