#include <iostream>
#include "Grafo.hpp"
#include "CarteiroChines.hpp"

using namespace std;

int main() {
    int n, m;

    cout << "Quantidade de vertices: ";
    cin >> n;

    cout << "Quantidade de arestas: ";
    cin >> m;

    Grafo grafo(n);

    cout << "\nDigite as arestas no formato:\n";
    cout << "origem destino custo\n\n";

    for (int i = 1; i <= m; i++) {
        int u, v;
        ll custo;
        cout << "Aresta " << i << ": ";
        cin >> u >> v >> custo;
        grafo.adicionarAresta(u, v, custo);
    }

    cout << "\n--- Resultado ---\n";
    CarteiroChines::resolver(grafo);

    return 0;
}