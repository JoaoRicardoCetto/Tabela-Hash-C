#include "tabela_hash.hpp"


void inicializarTabelaHash(THash *hash) {
    hash->chave = new TMatricula*[hash->qtdeChaves];
    
    for (int i = 0; i < hash->qtdeChaves; i++) {
        hash->chave[i] = NULL;
    }
    
    hash->inicio = NULL;
    hash->ultima = NULL;
    hash->totalMatriculas = 0;
}

bool descobreNumeroPrimo(long *num) {
    if (*num <= 1){
        return false;
    }
    if (*num <= 3){
        return true;  
    }
    if (*num % 2 == 0 || *num % 3 == 0){
        return false;
    }
    for (int i = 5; i * i <= *num; i += 6) { // Verificação de números primos
        if (*num % i == 0 || *num % (i + 2) == 0) 
            return false;
    }

    return true;
}

int proximoPrimo(long *tamanho_hash) {
    // Incrementa até encontrar o próximo número primo
    while (!descobreNumeroPrimo(tamanho_hash)) {
        (*tamanho_hash)++;
    }   
    
    return *tamanho_hash;
}

void tamanhoVetorHash(THash *hash) {
    int opcao;
    cout << "Informe o tamanho do Vetor hash (100% / 120% / 150% do tamanho da base de dados). Por padrão, o tamanho da hash será 100%: ";
    cin >> opcao;
    
    switch (opcao) {
        case 120:
            hash->qtdeChaves = long(hash->qtdeChaves * 1.2);
            break;
        case 150:
            hash->qtdeChaves = long(hash->qtdeChaves * 1.5);
            break;
        default:
            hash->qtdeChaves = hash->qtdeChaves * 1;
            break;
    }

    hash->qtdeChaves = proximoPrimo(&hash->qtdeChaves);     // Ajusta o tamanho da tabela para o próximo número primo
}

int funcaoHash(THash *hash, long matricula) {
    return matricula % hash->qtdeChaves;
}

void inserirMatricula(THash *hash, long numero_matricula, string nome) {
    int indice = funcaoHash(hash, numero_matricula);
    bool inserido = false;

    TMatricula *novaMatricula = new TMatricula;
    novaMatricula->matricula = numero_matricula;
    novaMatricula->nomeAluno = nome;
    novaMatricula->prox = hash->chave[indice];
    hash->chave[indice] = novaMatricula;

    if (hash->chave[indice] == NULL) {
        hash->chave[indice] = novaMatricula;
        inserido == true;
    }
    else{
        while(inserido == false){
            if (hash->chave[indice]->prox == NULL){
                hash->chave[indice]->prox = novaMatricula;
            }
            hash->chave[indice] = hash->chave[indice]->prox;

        }
    }
    if(inserido == true){
        hash->totalMatriculas++;
        cout << "O aluno " << novaMatricula->nomeAluno << " Foi cadastrada no sistema!";
    }
    else{
        cout << "Não foi possível cadastrar o aluno " << novaMatricula->nomeAluno;
    }

}

void lerArquivo(THash *hash, string nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    long matricula;
    string nome;

    if (arquivo.is_open()) {
        while (arquivo >> matricula >> nome) {
            inserirMatricula(hash, matricula, nome);
        }
        arquivo.close();
    } else {
        cout << "\nNão foi possível abrir o arquivo: " << nomeArquivo << endl;
    }
}

void gravarNoArquivo(THash *hash, string nomeArquivo) {
    ofstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        for (int i = 0; i < hash->qtdeChaves; i++) {
            TMatricula *atual = hash->chave[i];
            while (atual != NULL) {
                arquivo << atual->matricula << "\n" << atual->nomeAluno << "\n";
                atual = atual->prox;
            }
        }
        arquivo.close();
    } else {
        cout << "\nERRO ao abrir o arquivo: " << nomeArquivo << endl;
    }
}

void menuOpcoes(int *op) {
    cout << "\n============================== MENU ============================== \n";
    cout << "\n1) Escolher Tamanho do Vetor Hash";
    cout << "\n2) Inserir Nova Matrícula";
    cout << "\n3) Cancelar Matrícula (ou Remover Matrícula)";
    cout << "\n4) Pesquisar Matrícula";
    cout << "\n5) Total de Matrículas";
    cout << "\n6) Imprimir Matrículas";
    cout << "\n================================================================== \n";
    cin >> *op;
}

void menu(THash *tabela_hash, string nomeArquivo) {
    int opcao;
    int continuar = 1;
    
    while (continuar == 1) {
        menuOpcoes(&opcao);
        switch (opcao) {
            case 1: {
                inicializarTabelaHash(tabela_hash);
                lerArquivo(tabela_hash, nomeArquivo);
                tamanhoVetorHash(tabela_hash);
                cout << "\n\nTotal de Matrículas: " << tabela_hash->totalMatriculas;
                cout << "\n\nQuantidade de Chaves na Tabela: " << tabela_hash->qtdeChaves;
                break;
            }
            case 2: {
                long numero_matricula;
                string nome;
                cout << "Digite o número da matrícula: ";
                cin >> numero_matricula;
                cout << "Digite o nome do aluno: ";
                cin >> nome;
                inserirMatricula(tabela_hash, numero_matricula, nome);
                break;
            }
            case 3:
                // Lógica para remover matrícula
                break;
            case 4:
                // Lógica para pesquisar matrícula
                break;
            case 5:
                cout << "Total de Matrículas: " << tabela_hash->totalMatriculas << endl;
                break;
            case 6:
                // Lógica para imprimir matrículas
                break;
            default:
                cout << "\nOpção Inválida!" << endl;
                break;
        }

        cout << "\nPara continuar com outras operações, digite 1. Para sair, digite 0: ";
        cin >> continuar;
    }
}

int main() {
    THash *hash = new THash;
    hash->qtdeChaves = 100; // Definir um valor inicial ou calcular com base na base de dados

    string nomeArq = "../Matriculas.txt";
    menu(hash, nomeArq);

    delete[] hash->chave; // Libera a memória alocada para o vetor de chaves
    delete hash; // Libera a memória alocada para a estrutura hash
    return 0;
}
