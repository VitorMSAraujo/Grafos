#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
using namespace std;

const int MAX_VERTICES = 300;

//================================================================================
// Representação usando Matriz de Adjacência
//================================================================================
class GraphMatrizAdjacencia {
public:
    int num_vertices;
    vector<vector<int>> matriz;

    // Construtor: inicializa a matriz com zeros
    GraphMatrizAdjacencia(int n) : num_vertices(n), matriz(n, vector<int>(n, 0)) { }

    // Exibe a matriz de adjacência
    void exibirMatriz() const {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                cout << matriz[i][j] << " ";
            }
            cout << "\n";
        }
    }

    // Carrega o grafo a partir de um arquivo: a primeira linha contém o número de vértices
    // e as linhas seguintes contêm os elementos da matriz (separados por espaços)
    static GraphMatrizAdjacencia carregarGrafoMatriz(const string &nomeArquivo) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir o arquivo: " << nomeArquivo << "\n";
            exit(EXIT_FAILURE);
        }
        int n;
        arquivo >> n;
        GraphMatrizAdjacencia grafo(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                arquivo >> grafo.matriz[i][j];
            }
        }
        arquivo.close();
        return grafo;
    }
};

//================================================================================
// Representação usando Lista de Adjacência
//================================================================================
struct No {
    int vertice;
    int peso;  // Armazena o peso da aresta
    No* prox;
    No(int v, int p) : vertice(v), peso(p), prox(nullptr) { }
};

class GraphListaAdjacencia {
public:
    int num_vertices;
    vector<No*> lista; // Vetor de ponteiros para os nós

    GraphListaAdjacencia(int n) : num_vertices(n), lista(n, nullptr) { }

    ~GraphListaAdjacencia() {
        // Libera a memória de cada lista
        for (int i = 0; i < num_vertices; i++) {
            No* atual = lista[i];
            while (atual != nullptr) {
                No* temp = atual;
                atual = atual->prox;
                delete temp;
            }
        }
    }

    // Adiciona uma aresta (vértice 'u' para 'v' com o peso dado) no início da lista
    void adicionarAresta(int u, int v, int peso) {
        No* novoNo = new No(v, peso);
        novoNo->prox = lista[u];
        lista[u] = novoNo;
    }

    // Exibe a lista de adjacência
    void exibirLista() const {
        for (int i = 0; i < num_vertices; i++) {
            cout << "Vértice " << i << ": ";
            No* temp = lista[i];
            while (temp != nullptr) {
                cout << temp->vertice << " -> ";
                temp = temp->prox;
            }
            cout << "NULL\n\n";
        }
    }
};

// Converte uma matriz de adjacência para uma lista de adjacência sem duplicar as arestas 
GraphListaAdjacencia converterMatrizParaLista(const GraphMatrizAdjacencia &grafoMatriz) {
    GraphListaAdjacencia grafoLista(grafoMatriz.num_vertices);
    for (int i = 0; i < grafoMatriz.num_vertices; i++) {
        // Para evitar duplicação de arestas em grafo não direcionado,
        // percorremos apenas j = i+1 e inserimos arestas em ambas as direções.
        for (int j = i + 1; j < grafoMatriz.num_vertices; j++) {
            if (grafoMatriz.matriz[i][j] != 0) {
                grafoLista.adicionarAresta(i, j, grafoMatriz.matriz[i][j]);
                grafoLista.adicionarAresta(j, i, grafoMatriz.matriz[i][j]);
            }
        }
    }
    return grafoLista;
}

//================================================================================
// Função auxiliar para imprimir um caminho usando os antecessores
//================================================================================
void imprimirCaminho(const vector<int>& antecessor, int v) {
    if (antecessor[v] == -1) {
        cout << v;
        return;
    }
    imprimirCaminho(antecessor, antecessor[v]);
    cout << " -> " << v;
}

//================================================================================
// Busca em Largura (BFS)
//================================================================================
void bfs(const GraphMatrizAdjacencia &grafo, int s, int t) {
    vector<bool> visitado(grafo.num_vertices, false);
    vector<int> antecessor(grafo.num_vertices, -1);
    queue<int> fila;
    
    visitado[s] = true;
    fila.push(s);
    
    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();
        for (int v = 0; v < grafo.num_vertices; v++) {
            if (grafo.matriz[u][v] != 0 && !visitado[v]) {
                visitado[v] = true;
                antecessor[v] = u;
                fila.push(v);
                if (v == t) {
                    cout << "Caminho BFS de " << s << " até " << t << ": ";
                    imprimirCaminho(antecessor, t);
                    cout << "\n";
                    return;
                }
            }
        }
    }
    cout << "Não há caminho entre " << s << " e " << t << "\n";
}

//================================================================================
// Busca em Profundidade (DFS) usando pilha (sem recursão)
//================================================================================
void dfs(const GraphMatrizAdjacencia &grafo, int s, int t) {
    vector<bool> visitado(grafo.num_vertices, false);
    vector<int> antecessor(grafo.num_vertices, -1);
    stack<int> pilha;
    
    pilha.push(s);
    visitado[s] = true;
    
    while (!pilha.empty()) {
        int u = pilha.top();
        pilha.pop();
        if (u == t) {
            cout << "Caminho DFS de " << s << " até " << t << ": ";
            imprimirCaminho(antecessor, t);
            cout << "\n";
            return;
        }
        for (int v = 0; v < grafo.num_vertices; v++) {
            if (grafo.matriz[u][v] != 0 && !visitado[v]) {
                visitado[v] = true;
                antecessor[v] = u;
                pilha.push(v);
            }
        }
    }
    cout << "Não há caminho entre " << s << " e " << t << "\n";
}

//================================================================================
// Função principal (main)
//================================================================================
int main() {
    // Carrega o grafo a partir de um arquivo que contém a matriz de adjacência
    GraphMatrizAdjacencia grafoMatriz = GraphMatrizAdjacencia::carregarGrafoMatriz("pcv50.txt");

    cout << "Matriz de Adjacência:" << "\n";
    grafoMatriz.exibirMatriz();
    
    // Converte a matriz para lista de adjacência
    GraphListaAdjacencia grafoLista = converterMatrizParaLista(grafoMatriz);
    cout << "\nLista de Adjacência:" << "\n";
    grafoLista.exibirLista();
    
    // Realiza busca em largura e em profundidade
    int origem, destino;
    cout << "Digite a origem da busca: ";
    cin >> origem;
    cout << "Digite o destino da busca: ";
    cin >> destino;
    
    cout << "\nBusca em Largura (BFS):\n";
    bfs(grafoMatriz, origem, destino);
    
    cout << "\nBusca em Profundidade (DFS):\n";
    dfs(grafoMatriz, origem, destino);
    
    return 0;
}
