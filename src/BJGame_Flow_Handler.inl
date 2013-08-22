template <typename NameIter, typename MoneyIter>
BJGame_Flow_Handler::BJGame_Flow_Handler(
    NameIter currname,
    NameIter endname,
    MoneyIter curramount
)
    : m_player_handler(BJPlayer_Handler::instance(
        currname, endname, curramount))
    , m_names(currname, endname)
{}