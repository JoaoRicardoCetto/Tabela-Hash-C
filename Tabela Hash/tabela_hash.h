//FEITO POR: JOÃO RICARDO MATTEDI CETTO

#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct TipoMatricula {
    char nome[50];
    long long int num_matricula;
    struct TipoMatricula *proximo;

}TMatricula;

typedef struct TipoLista {
    TMatricula *inicio;
    TMatricula *fim;
    int total;
}TLista;

typedef struct TabelaHash {
    TLista *lista;
    int quantidade, tamanho, metodo;
    double A;
    clock_t t0, t1;
}THash;


TLista *inicializaLista(int tamanho);

int tamanhoTabelaHash();

int descobreNumeroPrimo(long long int num);

int proximoPrimo(int tamanho_hash);

void defineMetodoDeEnderecamentoHash(THash *hash);

int tamanhoVetorHash(THash *hash);

THash *inicializaHash();

void lerArquivo(THash *hash);

void gravarNoArquivo(TLista *L, int tamanho);

int funcaoHash(THash *hash, long long int matricula, int tamanho);

void inserirMatricula(THash *hash, long long int matricula, char *nome);

void removerMatricula(THash *hash, long long int matricula);

TMatricula *pesquisar(TLista *L, long long int matricula); 

void imprimirMatricula(TMatricula *matricula);

void imprimirListaDeMatriculas(TLista *L, int tamanho);

long long int ler_matricula();

void ler_nome(char *nome);

void opcoesMenu(int *op);

void menu(THash *hash);

#endif