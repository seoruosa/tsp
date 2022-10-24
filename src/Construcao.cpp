#include <cstdlib>
#include <iostream>
#include <climits>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include "Util.h"
#include "Construcao.h"

using namespace std;

void constroi_solucao(int n, vector<int>& s, float **distancia)
{
    s.clear(); // limpa solucao
    //insere todas cidades sequencialmente
    for (int j=0; j < n; j++) s.push_back(j);
}

/* Constroi uma solucao de forma gulosa, no caso,
   implementa o Metodo construtivo do vizinho mais proximo */
void constroi_solucao_gulosa_vizinho_mais_proximo(int n, vector<int> &s, float **d)
{
  set<int> nao_visitada; //lista das cidades ainda nao visitadas

  // inserir um elemento no final de uma lista
  for (int i = 1; i < n; i++)
    nao_visitada.insert(i);

  s.clear();      // limpa solucao
  s.push_back(0); // A cidade de origem é a cidade 0

  /*
	Implementar o loop do metodo construtivo, 
	inserindo sempre a cidade mais proxima ainda
	nao visitada na solucao.
*/

  while (!nao_visitada.empty())
  {
    int cidade_mais_proxima;                       // armazena a cidade mais proxima para inserir na solucao
    float min_dist = numeric_limits<float>::max(); // armazena a menor distancia

    int ultima_cidade = s.back();

    for (auto &cidade_candidata : nao_visitada)
    {
      float dist = d[ultima_cidade][cidade_candidata];
      if (dist < min_dist)
      {
        min_dist = dist;
        cidade_mais_proxima = cidade_candidata;
      }
    }

    nao_visitada.erase(cidade_mais_proxima);
    s.push_back(cidade_mais_proxima);
  }
}



/* Constroi uma solucao de forma aleatoria */
void constroi_solucao_aleatoria(int n, vector<int> &s, float **d)
{

    constroi_solucao(n, s, d);

    //Para c++ 11
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rand_seed(seed);
    shuffle ( s.begin(), s.end(), rand_seed );

    //Para c++ 98
    //random_shuffle ( s.begin(), s.end() );

}

/* Constroi uma solucao pela insercao mais barata */
void constroi_solucao_gulosa_insercao_mais_barata(int n, vector<int> *s, float **d)
{
    vector<int> nao_visitada;

    /* Inicio da Fase de Construcao de uma solucao */
    cout << "\n Construindo nova solucao ...\n";

    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista  */
        nao_visitada.push_back(i);

    }
    s->clear();
    s->push_back(0);  /* A cidade origem é a cidade 0 */

    int j = 1;
    int mais_proxima;
    float dist;

    int cid_i, cid_j, cid_k, pos;
    int melhor_i, melhor_k;
    float sij, melhor_sij;


    /* Insere as duas proximas cidades pela heuristica do vizinho mais proximo */

    while ( j < 3){
        dist = INT_MAX;
        int i = 0;
        int pos_i = 0;

        while (i < nao_visitada.size()) {
            if (d[s->at(j-1)][nao_visitada[i]] < dist){
                dist = d[s->at(j-1)][nao_visitada[i]];
                mais_proxima = nao_visitada[i];
                pos_i = i;
            }
            i++;
        }
        /* Insere a cidade mais proxima apos a ultima cidade inserida*/
        s->push_back(mais_proxima);
        /* Apaga a cidade mais_proxima da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_i);

        j++;
    }


   /* Formada uma subrota inicial com tres cidades, aplicar o metodo da
      insercao mais barata para decidir qual cidade inserir entre cada
      par de cidades i e j. A cidade k escolhida sera aquela que minimizar
      custo(k) = d(i,k) + d(k,j) - d(i,j) */

    while (j < n) {
      melhor_sij = INT_MAX;

      int k = 0;
      int pos_k;
      /* Calcula os custos para cada cidade k ser inserida entre as cidades i e j */
      while (k < nao_visitada.size()) {
        cid_k = nao_visitada[k];
        for (int i = 0; i < j; i++) {
	      cid_i = s->at(i);
          if ((i+1) != j) cid_j = s->at(i+1);
          else cid_j = 0;

          sij = d[cid_i][cid_k] + d[cid_k][cid_j] - d[cid_i][cid_j];

          if (sij < melhor_sij) {
 	        melhor_i = cid_i;
            //melhor_j = cid_j;
            melhor_k = cid_k;
            melhor_sij = sij;
            pos = i + 1;
            pos_k = k;
          }
        }
        k++;
      }

      /* Adiciona cada nova cidade k entre as cidades i e j que produzir a insercao mais barata */
      //insere_meio_vetor(s,melhor_k,pos,j);
      s->insert(s->begin()+pos,melhor_k);

      //imprime_rota(s,n);

      /* Apaga a cidade mais barata da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_k);

      j++;
    }
}

/* Constroi uma solucao parcialmente gulosa pelo metodo do vizinho mais proximo */
void constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(int n, vector<int> &s, float **d, float alpha)
{

    vector<int> nao_visitadas;

    /* Inicio da Fase de Construcao de uma solucao */
    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista das cidades nao visitadas */
        nao_visitadas.push_back(i);

    }

    //limpa solucao corrente
    s.clear();
    s.push_back(0);  /* A cidade origem � a cidade 0 */

    
    //Ordena lista
    ordena_dist_crescente ordem;
    ordem.d = d; // fornece a matriz de distancia para usar na ordenacao
    ordem.index = s[0];


    int j = 1;
    while (j < n){
      int tam_lista = 1 + std::ceil(((float)(nao_visitadas.size() - 1))*alpha);
      int pos_lista = randomico(0, tam_lista);

      /* Ordenando a lista de cidade nao visitadas */
      ordem.index = s.back();
      stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem);
      
      int cidade_escolhida = nao_visitadas[pos_lista];
      s.push_back(cidade_escolhida);
      
      nao_visitadas.erase(nao_visitadas.begin() + pos_lista);

      ++j;
    }
}

/* Constroi uma solucao parcialmente gulosa com base no metodo da insercao mais barata */
void constroi_solucao_parcialmente_gulosa_insercao_mais_barata(int n, vector<int> &s, float **d, float alpha)
{
  vector<int> nao_visitadas;

  int melhor_cidade;
  float menor_dist = numeric_limits<float>::max();
  float dist;
  
  int idx_cidade;

  /* Inicio da Fase de Construcao de uma solucao */
  for (int i=1; i<n; i++)
  {
      nao_visitadas.push_back(i);
  }

  //limpa solucao corrente
  s.clear();
  s.push_back(0);  /* A cidade origem � a cidade 0 */

  //Ordena lista
  ordena_dist_crescente ordem;
  ordem.d = d; // fornece a matriz de distancia para usar na ordenacao
  ordem.index = s[0];
  stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem);

  // Adiciona a segunda cidade
  s.push_back(nao_visitadas[0]);
  nao_visitadas.erase(nao_visitadas.begin());

  // Adiciona a terceira cidade
  for (idx_cidade = 0; idx_cidade < nao_visitadas.size(); idx_cidade++)
  {
    dist = d[s[0]][nao_visitadas[idx_cidade]] + d[nao_visitadas[idx_cidade]][s.back()];

    if(dist < menor_dist)
    {
      melhor_cidade = idx_cidade;
      menor_dist = dist;
    }
  }

  s.push_back(nao_visitadas[melhor_cidade]);
  nao_visitadas.erase(nao_visitadas.begin() + melhor_cidade);
  
  // Adiciona as cidades que tragam o menor incremento
  while (!nao_visitadas.empty())
  {
    int proxima_cidade = nao_visitadas.back();
    nao_visitadas.pop_back();

    // busca o menor incremento
    vector<float> custo_insercao_vec;
    vector<int> pos_custo_insercao;

    int posicao;
    for (posicao = 0; posicao < s.size(); posicao++)
    {
      custo_insercao_vec.push_back(custo_insercao(s, posicao, d, proxima_cidade));
      pos_custo_insercao.push_back(posicao);
    }
    
    auto ordem_custo = [custo_insercao_vec](int i,int j) {
      return ( custo_insercao_vec[i] < custo_insercao_vec[j]);
    };

    stable_sort(pos_custo_insercao.begin(), pos_custo_insercao.end(), ordem_custo);

    int tam_lista = 1 + std::ceil(((float)(s.size() - 1))*alpha);
    int pos_lista = randomico(0, tam_lista);

    melhor_cidade = pos_custo_insercao[pos_lista];
    
    // adiciona cidade
    s.insert(s.begin() + melhor_cidade, proxima_cidade);    
  }    
}
