#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <iomanip>

typedef unsigned long long val64;
typedef long double fbal64;

class Token;
class LiquidityPool;
class User;

using SwapFeeContract = val64 (*)(const LiquidityPool& l_pool, const unsigned& total_users);

extern std::map<std::string, std::shared_ptr<Token>> tokens;
extern const std::shared_ptr<Token> futhex;

val64 swapfee_observer(const LiquidityPool& l_pool, const unsigned& total_users);

class User {
private:
    mutable std::map<std::shared_ptr<Token>, val64> wallet;
    mutable std::map<std::shared_ptr<Token>, val64> staked;
    const unsigned address;

public:
    User(unsigned address);
    void get_info() const;
    void deposit(std::shared_ptr<Token> token, val64 amount) const;
    void provide_liquidity(const LiquidityPool& l_pool, const std::pair<std::shared_ptr<Token>, val64> first_token, const std::pair<std::shared_ptr<Token>, val64> second_token) const;
    void swap(const LiquidityPool& l_pool, const std::pair<std::shared_ptr<Token>, std::shared_ptr<Token>> tokens_to_swap, val64 amout, SwapFeeContract smart_contract) const;
    void unstake(const LiquidityPool& l_pool, std::vector<std::shared_ptr<Token>> tokens_to_unstake) const;
    void get_balance() const;
};

class Token {
    friend User;

private:
    static inline unsigned minted = 0;
    unsigned holders = 0;
    unsigned in_pool = 0;

    mutable unsigned mcap = 0;
    mutable unsigned printed = 0;
    mutable fbal64 price = 0;

    const std::string name;
    const std::string ticker;

    Token(unsigned mcap, unsigned printed, std::string name, std::string ticker);

public:
    static std::shared_ptr<Token> createToken(unsigned mcap, unsigned printed, std::string name, std::string ticker);
    static void check_to_delist();

    const std::string get_name() const;
    void get_info() const;

    ~Token();
};

class LiquidityPool {
    friend User;
    friend val64 swapfee_observer(const LiquidityPool& l_pool, const unsigned& total_users);

private:
    const std::pair<std::shared_ptr<Token>, std::shared_ptr<Token>> pair;
    mutable std::map<const User*, long double> providers;

    mutable std::pair<std::shared_ptr<Token>, val64> first_token_pair;
    mutable std::pair<std::shared_ptr<Token>, val64> second_token_pair;

    mutable val64 k = 1;

public:
    LiquidityPool(std::shared_ptr<Token> first_token, std::shared_ptr<Token> second_token);

    val64 get_first_value() const;
    val64 get_second_value() const;

    std::map<const User*, long double> get_providers() const;

    void get_info() const;
};