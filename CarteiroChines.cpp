#include "CarteiroChines.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <iomanip>

using namespace std;

static const ll INFINITO = (1LL << 60);

void exibirMatrizDistancias(const Grafo& grafo) {
    int n = grafo.quantidadeVertices;
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INFINITO));
    
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0;
    }
    
    for (const auto& a : grafo.arestas) {
        if (dist[a.origem][a.destino] > a.custo) {
            dist[a.origem][a.destino] = a.custo;
            dist[a.destino][a.origem] = a.custo;
        }
    }
    
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    cout << "\n=== MATRIZ DE DISTANCIAS MINIMAS ===\n\n";
    cout << "     ";
    for (int j = 1; j <= n; j++) {
        cout << setw(6) << j;
    }
    cout << "\n     ";
    for (int j = 1; j <= n; j++) {
        cout << "------";
    }
    cout << "\n";
    
    for (int i = 1; i <= n; i++) {
        cout << setw(3) << i << " |";
        for (int j = 1; j <= n; j++) {
            if (dist[i][j] == INFINITO) {
                cout << setw(6) << "INF";
            } else {
                cout << setw(6) << dist[i][j];
            }
        }
        cout << "\n";
    }
}

void CarteiroChines::resolver(const Grafo& grafo) {
    int n = grafo.quantidadeVertices;

    exibirMatrizDistancias(grafo); 

    ll custoOriginal = 0;
    for (const auto& a : grafo.arestas) {
        custoOriginal += a.custo;
    }

    vector<bool> visitado(n + 1, false);
    stack<int> pilha;

    int inicio = -1;
    for (int i = 1; i <= n; i++) {
        if (grafo.grau[i] > 0) {
            inicio = i;
            break;
        }
    }

    if (inicio == -1) {
        cout << "Grafo sem arestas.\n";
        return;
    }

    pilha.push(inicio);
    visitado[inicio] = true;

    while (!pilha.empty()) {
        int u = pilha.top();
        pilha.pop();
        for (auto [v, _] : grafo.adjacencias[u]) {
            if (!visitado[v]) {
                visitado[v] = true;
                pilha.push(v);
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        if (grafo.grau[i] > 0 && !visitado[i]) {
            cout << "Grafo desconexo. CPP não aplicável.\n";
            return;
        }
    }

    vector<int> verticesImpares;
    for (int i = 1; i <= n; i++) {
        if (grafo.grau[i] % 2 == 1) {
            verticesImpares.push_back(i);
        }
    }

    int k = verticesImpares.size();
    ll custoAdicional = 0;

    // Multigrafo
    vector<unordered_map<int, int>> multigrafo(n + 1);
    for (const auto& a : grafo.arestas) {
        multigrafo[a.origem][a.destino]++;
        multigrafo[a.destino][a.origem]++;
    }

    if (k > 0) {
        vector<vector<ll>> dist(k, vector<ll>(n + 1, INFINITO));
        vector<vector<int>> pai(k, vector<int>(n + 1, -1));

        for (int i = 0; i < k; i++) {
            int origem = verticesImpares[i];
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> fila;

            dist[i][origem] = 0;
            fila.push({0, origem});

            while (!fila.empty()) {
                auto [d, u] = fila.top();
                fila.pop();

                if (d != dist[i][u]) continue;

                for (auto [v, custo] : grafo.adjacencias[u]) {
                    if (dist[i][v] > d + custo) {
                        dist[i][v] = d + custo;
                        pai[i][v] = u;
                        fila.push({dist[i][v], v});
                    }
                }
            }
        }

        int TOTAL = 1 << k;
        vector<ll> dp(TOTAL, INFINITO);
        vector<int> escolha(TOTAL, -1);
        dp[0] = 0;

        for (int mask = 0; mask < TOTAL; mask++) {
            if (dp[mask] == INFINITO) continue;

            int i;
            for (i = 0; i < k; i++)
                if (!(mask & (1 << i))) break;

            if (i == k) continue;

            for (int j = i + 1; j < k; j++) {
                if (mask & (1 << j)) continue;

                int novoMask = mask | (1 << i) | (1 << j);
                ll novoCusto = dp[mask] + dist[i][verticesImpares[j]];

                if (novoCusto < dp[novoMask]) {
                    dp[novoMask] = novoCusto;
                    escolha[novoMask] = (i << 8) | j;
                }
            }
        }

        int atual = TOTAL - 1;
        while (atual) {
            int codigo = escolha[atual];
            int i = (codigo >> 8) & 0xFF;
            int j = codigo & 0xFF;

            int a = verticesImpares[i];
            int b = verticesImpares[j];

            custoAdicional += dist[i][b];

            int v = b;
            while (v != a) {
                int u = pai[i][v];
                multigrafo[u][v]++;
                multigrafo[v][u]++;
                v = u;
            }

            atual &= ~(1 << i);
            atual &= ~(1 << j);
        }
    }

    vector<int> circuito;
    stack<int> caminho;
    caminho.push(inicio);

    while (!caminho.empty()) {
        int u = caminho.top();
        if (!multigrafo[u].empty()) {
            int v = multigrafo[u].begin()->first;
            if (--multigrafo[u][v] == 0) multigrafo[u].erase(v);
            if (--multigrafo[v][u] == 0) multigrafo[v].erase(u);
            caminho.push(v);
        } else {
            circuito.push_back(u);
            caminho.pop();
        }
    }

    reverse(circuito.begin(), circuito.end());

    cout << "\nCusto original: " << custoOriginal << "\n";
    cout << "Custo adicional: " << custoAdicional << "\n";
    cout << "Custo total minimo: " << custoOriginal + custoAdicional << "\n";
    cout << "Circuito Euleriano:\n";

    for (size_t i = 0; i < circuito.size(); i++) {
        if (i) cout << " -> ";
        cout << circuito[i];
    }
    cout << "\n";
}