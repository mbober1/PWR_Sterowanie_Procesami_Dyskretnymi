#pragma once

#include <algorithm>
#include <map>
#include <vector>

template <class T> class Graf {
private:
  int counter = 1; //ile jest wierzchołków w grafie
  static void buildTree(Graf<T> *g, std::vector<T> values, int parent = 0); //budowanie drzewa

public:
  std::map<int, T> vertices; //mapa wierzchołków
  std::map<int, std::vector<int>> adj; // mapa (lista) sąsiadów 
  Graf();
  ~Graf();
  int addVertex(T value); // dodawanie wierzchołka
  void addEdge(int v, int w); // dodawanie krawędzi
  static Graf<T> tree(std::vector<T> values); // tworzenie drzewa
};

template <typename T> Graf<T>::Graf() {}
template <typename T> Graf<T>::~Graf() {}

template <typename T> int Graf<T>::addVertex(T value) { // dodawanie wierzchołka
  vertices[counter] = value; // do mapy wierzchołka dodaję wierzchołek o wartości value
  return counter++;
}

template <typename T> void Graf<T>::addEdge(int v, int w) {
  adj[v].push_back(w);
}

template <typename T>
void Graf<T>::buildTree(Graf<T> *g, std::vector<T> values, int parent) {
  if (values.size() > 0) { // czy zostały jakieś wartości do dodania
    for (int i = 0; i < values.size(); ++i) { // dla każdej wartości
      int v = g->addVertex(values[i]); // dodaj wierzchołek z tą wartością
      g->addEdge(parent, v); // dodaj krawędź od rodzica do wierzchołka

      std::vector<T> newValues; // nowy wektor
      std::copy_if(std::begin(values), std::end(values), // kopiuję do niego wszystkie
                   std::back_inserter(newValues), // wartości oprócz aktualnie użytej
                   [values, i](T x) { return x != values[i]; }); 

      buildTree(g, newValues, v); // budowanie drzewa
    }
  }
}

template <typename T> Graf<T> Graf<T>::tree(std::vector<T> values) {
  Graf<T> g;
  buildTree(&g, values);
  return g;
}
