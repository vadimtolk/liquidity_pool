#include "header.h"

LiquidityPool::LiquidityPool(std::shared_ptr<Token> first_token, std::shared_ptr<Token> second_token) : pair{first_token, second_token} {
    first_token_pair.first = first_token;
    first_token_pair.second = 0;

    second_token_pair.first = second_token;
    second_token_pair.second = 0;
}

val64 LiquidityPool::get_first_value() const { return first_token_pair.second; }
val64 LiquidityPool::get_second_value() const { return second_token_pair.second; }

std::map<const User*, long double> LiquidityPool::get_providers() const { return providers; }

void LiquidityPool::get_info() const {
    std::cout << "Tokens pair : " << pair.first->get_name() << " / " << pair.second->get_name() <<
                 "\nValues : " << first_token_pair.second << " / " << second_token_pair.second <<
                 "\nK = " << k << '\n';
}