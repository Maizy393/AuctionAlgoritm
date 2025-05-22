#include "auction_core.h"
#include <algorithm>

AuctionCore::AuctionCore(int _n, const std::vector<std::vector<ll>>& matrix)
        : n(_n),
          C(matrix),
          original(matrix),
          matchL(n), matchR(n),
          seen(n), visL(n), visR(n)
{}

bool AuctionCore::dfsAug(int u) {
    for (int v = 0; v < n; ++v) {
        if (C[u][v] == 0 && !seen[v]) {
            seen[v] = 1;
            if (matchR[v] < 0 || dfsAug(matchR[v])) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int AuctionCore::maxMatching() {
    std::fill(matchL.begin(), matchL.end(), -1);
    std::fill(matchR.begin(), matchR.end(), -1);
    int cnt = 0;
    for (int u = 0; u < n; ++u) {
        seen.assign(n, 0);
        if (dfsAug(u)) ++cnt;
    }
    return cnt;
}

void AuctionCore::dfsCover(int u) {
    visL[u] = 1;
    for (int v = 0; v < n; ++v) {
        if (C[u][v] == 0 && !visR[v]) {
            visR[v] = 1;
            if (matchR[v] >= 0 && !visL[matchR[v]])
                dfsCover(matchR[v]);
        }
    }
}

ll AuctionCore::solve(std::vector<int>& assignment) {
    // Шаг 1: нормализация строк
    for (int i = 0; i < n; ++i) {
        ll mn = *std::min_element(C[i].begin(), C[i].end());
        for (int j = 0; j < n; ++j)
            C[i][j] -= mn;
    }
    // Шаг 2: нормализация столбцов
    for (int j = 0; j < n; ++j) {
        ll mn = C[0][j];
        for (int i = 1; i < n; ++i) mn = std::min(mn, C[i][j]);
        for (int i = 0; i < n; ++i) C[i][j] -= mn;
    }

    // Основной цикл: matching + покрытие + модификация
    while (true) {
        if (maxMatching() == n) break;

        visL.assign(n, 0);
        visR.assign(n, 0);
        for (int u = 0; u < n; ++u)
            if (matchL[u] < 0)
                dfsCover(u);

        ll delta = INF;
        for (int i = 0; i < n; ++i) if (visL[i])
                for (int j = 0; j < n; ++j) if (!visR[j])
                        delta = std::min(delta, C[i][j]);

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                if (visL[i] && !visR[j])       C[i][j] -= delta;
                else if (!visL[i] && visR[j])  C[i][j] += delta;
            }
    }

    // Финальный matching и сбор результата
    maxMatching();
    assignment.assign(n, -1);
    ll total = 0;
    for (int i = 0; i < n; ++i) {
        assignment[i] = matchL[i];
        total += original[i][matchL[i]];
    }
    return total;
}