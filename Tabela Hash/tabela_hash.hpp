#ifndef TABELA_HASH_HPP
#define TABELA_HASH_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Matricula{
    string nomeAluno;
    long matricula;
    TMatricula *prox;
    TMatricula *inicio; //Primeira matricula da chave
    TMatricula *ultima; //Ultima matricula da chave

}; typedef struct Matricula TMatricula;

struct TabelaHash{
    TMatricula **chave;
    long qtdeChaves;
    long totalMatriculas;

}; typedef struct TabelaHash THash;

void inicializarTabelaHash(THash *hash, long tamanho);

bool descobreNumeroPrimo(long *num);

int proximoPrimo(long *tamanho_hash);

void tamanhoVetorHash(long *tamanhoHash);

int proximoPrimo(long tamanho_hash); 

int funcaoHash(THash *hash, long matricula);

void inserirMatricula(THash *hash, long tamanho, long numero_matricula, string nome);

void lerArquivo(THash *hash, string nomeArquivo);

void gravarNoArquivo(THash *THash, string nomeArquivo);

void menuOpcoes(int *op);

void menu(THash *tabela_Hash, long tamanhoHash, string nomeArquivo);

#endif