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

    // ----------- ввод -----------
    if (!(cin >> n)) return 0;
    original.assign(n, vector<ll>(n));
    C.assign(n, vector<ll>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            cin >> original[i][j];
            C[i][j] = original[i][j];
        }

    // ----------- шаг 1: вычитаем минимумы строк -----------
    for (int i = 0; i < n; ++i) {
        ll mn = *min_element(C[i].begin(), C[i].end());
        for (int j = 0; j < n; ++j) C[i][j] -= mn;
    }

    // ----------- шаг 2: вычитаем минимумы столбцов -----------
    for (int j = 0; j < n; ++j) {
        ll mn = C[0][j];
        for (int i = 1; i < n; ++i) mn = min(mn, C[i][j]);
        for (int i = 0; i < n; ++i) C[i][j] -= mn;
    }

    // подготовка структур
    matchL.resize(n);
    matchR.resize(n);

    // ----------- основной цикл -----------
    while (true) {
        if (maxMatching() == n) break;          // полное паросочетание найдено

        // --- строим минимальное покрытие нулей ---
        visL.assign(n, 0);
        visR.assign(n, 0);
        for (int u = 0; u < n; ++u)
            if (matchL[u] == -1)                // свободная строка
                dfsCover(u);

        // строки НЕ покрыты, если visL[i]=1; столбцы НЕ покрыты, если visR[j]=0
        // delta = min среди непокрытых клеток (visL=1 & visR=0)
        ll delta = INF;
        for (int i = 0; i < n; ++i)
            if (visL[i])                // строка непокрыта
                for (int j = 0; j < n; ++j)
                    if (!visR[j])       // столбец непокрыт
                        delta = min(delta, C[i][j]);

        // --- модификация матрицы ---
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                if (visL[i] && !visR[j])        // непокрытая клетка
                    C[i][j] -= delta;
                else if (!visL[i] && visR[j])   // двойное покрытие
                    C[i][j] += delta;
            }
        // после модификации появляются новые нули → цикл повторяем
    }

    // ----------- вывод результата -----------
    ll total = 0;
    for (int i = 0; i < n; ++i)
        total += original[i][matchL[i]];

    cout << "Минимальная стоимость: " << total << '\n';
    cout << "Назначение (строка -> столбец):\n";
    for (int i = 0; i < n; ++i)
        cout << i + 1 << " -> " << matchL[i] + 1 << '\n';

    return 0;
}