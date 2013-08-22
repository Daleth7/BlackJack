    //This accepts iterators pointing to Card
template <typename InIter>
Deck::Deck(InIter curr, InIter end, bool inAlwaysShuffle)
    : m_deck(0)
    , m_trash(0)
    , m_always_shuffle(inAlwaysShuffle)
    , m_gen(get_seed())
{
    while(curr != end)
        m_deck.push_back(std::make_shared<Card>(**(curr++)));
    if(m_always_shuffle) this->shuffle();
}