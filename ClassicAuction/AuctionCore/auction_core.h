#ifndef AUCTION_CORE_HPP
#define AUCTION_CORE_HPP

#include <vector>
#include <limits>

using ll = long long;
static const ll INF = std::numeric_limits<ll>::max();

class AuctionCore {
public:
    // n × n матрица
    AuctionCore(int n, const std::vector<std::vector<ll>>& matrix);

    // Запускает алгоритм (нормализация + покрытие + модификация)
    // Возвращает минимальную стоимость, заполняет assignment[i]=j
    ll solve(std::vector<int>& assignment);

private:
    int n;
    std::vector<std::vector<ll>> C, original;
    std::vector<int> matchL, matchR;
    std::vector<char> seen, visL, visR;

    bool dfsAug(int u);
    int  maxMatching();
    void dfsCover(int u);
};

#endif // AUCTION_CORE_HPP