#include "Grafo.hpp"

Grafo::Grafo(int n) {
    quantidadeVertices = n;
    quantidadeArestas = 0;
    adjacencias.resize(n + 1);
    grau.assign(n + 1, 0);
}

void Grafo::adicionarAresta(int u, int v, ll custo) {
    arestas.push_back({u, v, custo});
    adjacencias[u].push_back({v, custo});
    adjacencias[v].push_back({u, custo});
    grau[u]++;
    grau[v]++;
    quantidadeArestas++;
}