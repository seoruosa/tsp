#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <utility> //funcao swap (para versao anterior a C11 usar <algorithm>)
#include "Arquivos.h"
#include "Construcao.h"
#include "Descida.h"
#include "Util.h"

using namespace std;


float perturbacao(int n, vector<int> &s, float **d, float fo, int nivel)
{
  int ntrocasMax = nivel + 1;
  int ntrocas = 0, i, j;

  while (ntrocas < ntrocasMax)
  {
    ntrocas++;
    // seleciona as posicoes das cidades para trocar
    i = rand() % n;
    do{
        j = rand() % n;
    }while (i == j);
    
    float delta1 = calcula_delta(n,s,d,i,j);
    // Faz o movimento
    swap(s[i], s[j]);
    float delta2 = calcula_delta(n,s,d,i,j);
    // Calcular a nova distancia
      fo = fo - delta1 + delta2;
    } 
    return fo;
}

float ILS(int n, vector<int> &s, float **d,
          int nivelMax, // nro de vezes sem melhora em um dado nivel
          int ILSmax) 
{
  int i, j;
  int iter = 0;
  int melhorIter = 0;

  vector<int> so = s;
  float fo = descida_best_improvement(n, so, d);
  
  vector<int> s_l = so;
  float fo_l = fo;

  vector<int> s_star(so);
  float fo_star = fo;
  
  int nivel = 1;
  while (iter - melhorIter < ILSmax)
  {
    int nivel_atual = std::min(nivel, nivelMax);
    ++iter;

    fo_l = perturbacao(n, s_l, d, fo_l, nivel_atual); //mexe em s_l
    fo_l = descida_best_improvement(n, s_l, d);

    if (fo_l < fo)
    {
      so = s_l;
      fo = fo_l;

      melhorIter = iter;
      nivel = 1;

      if (fo_l < fo_star)
      {
        s_star = s_l;
        fo_star = fo_l;
      }
    }
    else
    {
      ++nivel;
    }
    
  }

  s = s_star;
  return fo_star;
}

float SmartILS(int n, vector<int> &s, float **d, 
             int nivel, 
         int nVezesNivel, 
         int ILSMax)
{
   int i, j, iter, MelhorIter;
  float fo, fo_2l;

  

  return fo;
}

