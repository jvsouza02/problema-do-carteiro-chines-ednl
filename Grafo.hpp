#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <utility>

using ll = long long;

struct Aresta {
    int origem;
    int destino;
    ll custo;
};

class Grafo {
public:
    int quantidadeVertices;
    int quantidadeArestas;

    std::vector<Aresta> arestas;
    std::vector<std::vector<std::pair<int, ll>>> adjacencias;
    std::vector<int> grau;

    Grafo(int n);

    void adicionarAresta(int u, int v, ll custo);
};

#endif