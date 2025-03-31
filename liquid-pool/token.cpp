#include "header.h"

Token::Token(unsigned mcap, unsigned printed, std::string name, std::string ticker) : mcap{mcap}, printed{printed}, name{name}, ticker{ticker} {
    price = static_cast<long double>(mcap) / printed;
    //std::cout << name << " created\n";
}

std::shared_ptr<Token> Token::createToken(unsigned mcap, unsigned printed, std::string name, std::string ticker) {
    minted++;
    return std::shared_ptr<Token>(new Token(mcap, printed, name, ticker));
}

void Token::check_to_delist() {
    for (auto it = tokens.begin(); it != tokens.end(); ) {
        if (it->second->mcap < 1500) {
            it = tokens.erase(it);
        } else {
            ++it;
        }
    }
}

const std::string Token::get_name() const {
    return name;
}

void Token::get_info() const {
    std::cout << "\nToken : " << name << " ($" << ticker << "). \nMarketCap : " << mcap << " $\nHolders : " << holders << "\nLiquidity : " << in_pool << "\nPrice : " << price << " $\nTotal printed : " << printed << '\n';
}

Token::~Token() {
    //std::cout << name << " delisted\n";
    minted--;
}