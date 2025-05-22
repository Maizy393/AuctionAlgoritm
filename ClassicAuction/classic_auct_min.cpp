#include <iostream>
#include "AuctionCore/auction_core.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::vector<ll>> mat(n, std::vector<ll>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            std::cin >> mat[i][j];

    AuctionCore solver(n, mat);
    std::vector<int> assignment;
    ll minCost = solver.solve(assignment);

    std::cout << "Минимальная стоимость: " << minCost << "\n";
    std::cout << "Назначение (строка -> столбец):\n";
    for (int i = 0; i < n; ++i)
        std::cout << i+1 << " -> " << assignment[i]+1 << "\n";

    return 0;
}