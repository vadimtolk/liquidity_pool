#include "header.h"

std::map<std::string, std::shared_ptr<Token>> tokens = {};
extern const std::shared_ptr<Token> futhex = Token::createToken(100000, 1000000, "Futhex", "FUTH");

int main() {
    const std::shared_ptr<Token> memefuth = Token::createToken(25000, 10000000, "Memefuth", "MFH");

    const LiquidityPool l_pool(futhex, memefuth);

    tokens.insert( {"Futhex", futhex} );
    tokens.insert( {"Memefuth", memefuth} );

    User vadim(1);

    vadim.deposit(futhex, 200000);
    vadim.deposit(memefuth, 150000);

    vadim.get_info();

    futhex->get_info();
    memefuth->get_info();
    
    vadim.provide_liquidity(l_pool, std::make_pair(futhex, 10000), std::make_pair(memefuth, 10000));

    vadim.get_info();
    l_pool.get_info();

    vadim.provide_liquidity(l_pool, std::make_pair(futhex, 140000), std::make_pair(memefuth, 140000));

    vadim.get_info();
    l_pool.get_info();

    futhex->get_info();
    memefuth->get_info();

    User boris(2);
    boris.deposit(futhex, 40000);

    boris.swap(l_pool, std::make_pair(futhex, memefuth), 35900, swapfee_observer);

    l_pool.get_info();

    boris.get_info();
    vadim.get_info();

    futhex->get_info();
    memefuth->get_info();

    std::vector<std::shared_ptr<Token>> tokenst = {futhex, memefuth};

    vadim.unstake(l_pool, tokenst);

    vadim.get_info();
    vadim.get_balance();

    std::cout << "User : " << sizeof(User) << "\nToken : " << sizeof(Token) << "\nLpool : " << sizeof(LiquidityPool);

    return 0;
}