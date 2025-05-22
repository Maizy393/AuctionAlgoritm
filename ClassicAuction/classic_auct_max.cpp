#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "AuctionCore/auction_core.h"

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Читаем размер и исходную матрицу
    int n;
    cin >> n;
    vector<vector<ll>> orig(n, vector<ll>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> orig[i][j];

    // Переводим задачу максимизации в минимизацию:
    // C[i][j] = M - orig[i][j], где M = максимум всех orig
    ll M = numeric_limits<ll>::min();
    for (auto &row : orig)
        for (ll x : row)
            M = max(M, x);

    vector<vector<ll>> cost(n, vector<ll>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cost[i][j] = M - orig[i][j];

    // Решаем «минимизирующую» задачу
    AuctionCore solver(n, cost);
    vector<int> assignment;
    solver.solve(assignment);

    // Восстанавливаем максимальную стоимость по original
    ll maxCost = 0;
    for (int i = 0; i < n; ++i)
        maxCost += orig[i][assignment[i]];

    // Вывод результатов
    cout << "Максимальная стоимость: " << maxCost << "\n";
    cout << "Назначение (строка -> столбец):\n";
    for (int i = 0; i < n; ++i)
        cout << i+1 << " -> " << assignment[i]+1 << "\n";

    return 0;
}