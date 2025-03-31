#include "header.h"

User::User(unsigned address) : address{address} {}

void User::get_info() const {
    std::cout << "User : " << address << "\nWallet :\n";

    if (!wallet.empty()) {
        for (const auto& [token, amount] : wallet) {
            std::cout << '\t' << token->name << " : " << amount << '\n';
        }
    } else std::cout << "\tNothing in wallet...\n";

    std::cout << "Staked :\n";

    if (!staked.empty()) {
        for (const auto& [token, amount] : staked) {
            std::cout << '\t' << token->name << " : " << amount << '\n';
        }
    } else std::cout << "\tNothing in staked...\n";
}

void User::deposit(std::shared_ptr<Token> token, val64 amount) const {
    auto it = wallet.find(token);

    if (it != wallet.end()) {
            token->mcap += amount;
            it->second += amount;
    } else {
            wallet.insert({token, amount});
            token->mcap += amount;
            token->holders++;
    }

    std::cout << "Success! You deposited " << amount << " " << token->name << '\n';
}

void User::provide_liquidity(const LiquidityPool& l_pool, std::pair<std::shared_ptr<Token>, val64> first_token, std::pair<std::shared_ptr<Token>, val64> second_token) const {
    if (wallet[first_token.first] >= first_token.second && wallet[second_token.first] >= second_token.second && first_token.second == second_token.second) {
        l_pool.providers[this] = 0;
        
        wallet[first_token.first] -= first_token.second;
        wallet[second_token.first] -= second_token.second;

        auto first_token_iterator = staked.find(first_token.first);
        auto second_token_iterator = staked.find(second_token.first);

        if (first_token_iterator != staked.end()) staked[first_token.first] += first_token.second;
        else staked[first_token.first] = first_token.second;

        if (second_token_iterator != staked.end()) staked[second_token.first] += second_token.second;
        else staked[second_token.first] = second_token.second;

        first_token.first->in_pool += first_token.second;
        second_token.first->in_pool += second_token.second;

        l_pool.first_token_pair.second += first_token.second;
        l_pool.second_token_pair.second += second_token.second;

        l_pool.k = l_pool.first_token_pair.second * l_pool.second_token_pair.second;

        std::cout << "Success! You provided " << first_token.second << " / " << second_token.second << " for pair : " << first_token.first->name << " / " << second_token.first->name << '\n';
    } else std::cout << "Please, research your balance. You must provide liquidity on equal halves\n";
}

void User::swap(const LiquidityPool& l_pool, const std::pair<std::shared_ptr<Token>, std::shared_ptr<Token>> tokens_to_swap, val64 amount, SwapFeeContract smart_contract) const {
    val64 tokens_to_get = l_pool.second_token_pair.second - (l_pool.k / (l_pool.first_token_pair.second + amount));
    val64 swapfee = swapfee_observer(l_pool, 2);

    if (wallet[tokens_to_swap.first] >= amount && tokens_to_get <= l_pool.get_second_value()) {
        wallet[tokens_to_swap.first] -= amount;

        if (wallet[futhex] >= swapfee) {
            auto it = wallet.find(tokens_to_swap.second);
            wallet[futhex] -= swapfee;

            if (it != wallet.end()) wallet[tokens_to_swap.second] += tokens_to_get;
            else {
                wallet[tokens_to_swap.second] = tokens_to_get;
                tokens_to_swap.second->holders++;
            }

            l_pool.first_token_pair.second += amount;
            l_pool.second_token_pair.second -= tokens_to_get;

            tokens_to_swap.first->in_pool += amount;
            tokens_to_swap.second->in_pool -= tokens_to_get;

            for (auto& [provider, fee] : l_pool.providers) {
                provider->staked[tokens_to_swap.first] += (amount / l_pool.providers.size());
                provider->staked[tokens_to_swap.second] -= (tokens_to_get / l_pool.providers.size());
                
                auto it = provider->staked.find(futhex);

                if (it != provider->staked.end()) provider->staked[futhex] += (swapfee / l_pool.providers.size());
                else provider->staked[futhex] = (swapfee / l_pool.providers.size());
            }

            std::cout << "Success! You swaped " << amount << " " << tokens_to_swap.first->ticker << " to " << tokens_to_get << " " << tokens_to_swap.second->ticker << ". Swapfee : " << swapfee << "\n";
        } else std::cout << "You haven't fee to swap.\n";
    } else std::cout << "No more liquidity in pool or you balance is low.\n";
}

void User::unstake(const LiquidityPool& l_pool, std::vector<std::shared_ptr<Token>> tokens_to_unstake) const {  
    for (auto& token : tokens_to_unstake) {
        std::cout << "You unstaked " << staked[token] << " " << token->name << '\n';
        token->in_pool -= staked[token];
        l_pool.first_token_pair.second -= staked[token];
        wallet[token] += staked[token];
        staked.erase(token);
    }
}

void User::get_balance() const {
    fbal64 total = 0;

    for (const auto& [token, value] : wallet) {
        total += token->price * value;
    }
    
    for (const auto& [token, value] : staked) {
        total += token->price * value;
    }

    std::cout << "User " << address << " | Balance = " << total << " $\n"; 
}