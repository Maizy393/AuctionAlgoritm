#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;
using ll = long long;
const ll INF = numeric_limits<ll>::max();

int n;
vector<vector<ll>> C, original;          // рабочая и исходная матрицы
vector<int> matchL, matchR;              // matchL[i]=j, matchR[j]=i
vector<char> seen;                       // вспом. для DFS по нулям
vector<char> visL, visR;                 // достижимые вершины (для покрытия)

// ---------- DFS-matching: augmenting path ----------
bool dfsAug(int u) {
    for (int v = 0; v < n; ++v)
        if (C[u][v] == 0 && !seen[v]) {
            seen[v] = 1;
            if (matchR[v] == -1 || dfsAug(matchR[v])) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
    return false;
}

// ---------- ищем максимальное паросочетание по нулям ----------
int maxMatching() {
    fill(matchL.begin(), matchL.end(), -1);
    fill(matchR.begin(), matchR.end(), -1);
    int sz = 0;
    for (int u = 0; u < n; ++u) {
        seen.assign(n, 0);
        if (dfsAug(u)) ++sz;
    }
    return sz;
}

// ---------- DFS от свободных строк (строим Z_U, Z_V) ----------
void dfsCover(int u) {
    visL[u] = 1;
    for (int v = 0; v < n; ++v)
        if (C[u][v] == 0 && !visR[v]) {
            visR[v] = 1;
            if (matchR[v] != -1 && !visL[matchR[v]])
                dfsCover(matchR[v]);
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ввод размерности
    if (!(cin >> n)) return 0;
    original.assign(n, vector<ll>(n));
    C.assign(n, vector<ll>(n));

    // читаем исходную матрицу и находим максимум
    ll maxval = LLONG_MIN;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> original[i][j];
            maxval = max(maxval, original[i][j]);
        }
    }
    // преобразуем задачу в минимизацию: C[i][j] = maxval - original[i][j]
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = maxval - original[i][j];

    matchL.resize(n);
    matchR.resize(n);

    // шаг 1: вычитаем минимумы строк
    for (int i = 0; i < n; ++i) {
        ll mn = *min_element(C[i].begin(), C[i].end());
        for (int j = 0; j < n; ++j) C[i][j] -= mn;
    }
    // шаг 2: вычитаем минимумы столбцов
    for (int j = 0; j < n; ++j) {
        ll mn = C[0][j];
        for (int i = 1; i < n; ++i) mn = min(mn, C[i][j]);
        for (int i = 0; i < n; ++i) C[i][j] -= mn;
    }

    // основной цикл: matching, покрытие, delta, модификация
    while (true) {
        if (maxMatching() == n) break;

        visL.assign(n, 0);
        visR.assign(n, 0);
        for (int u = 0; u < n; ++u)
            if (matchL[u] == -1)
                dfsCover(u);

        // вычисляем delta среди непокрытых
        ll delta = INF;
        for (int i = 0; i < n; ++i) if (visL[i])
                for (int j = 0; j < n; ++j) if (!visR[j])
                        delta = min(delta, C[i][j]);

        // модификация матрицы
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                if (visL[i] && !visR[j])      C[i][j] -= delta;
                else if (!visL[i] && visR[j]) C[i][j] += delta;
            }
    }

    // финальное matching
    maxMatching();

    // вычисляем максимальную стоимость по original
    ll total = 0;
    for (int i = 0; i < n; ++i)
        total += original[i][matchL[i]];

    cout << "Максимальная стоимость: " << total << '\n';
    cout << "Назначение (строка -> столбец):\n";
    for (int i = 0; i < n; ++i)
        cout << i+1 << " -> " << matchL[i]+1 << '\n';

    return 0;
}
