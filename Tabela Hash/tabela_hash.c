// FEITO POR: JOAO RICARDO MATTEDI CETTO

#include "tabela_hash.h"

TLista *inicializaLista(int tamanho){
    TLista *lista;
    lista = (TLista*) malloc(tamanho*sizeof(TLista));
    
    for(int i = 0; i < tamanho; i++){
        lista[i].inicio = NULL;
        lista[i].fim = NULL;
    }
    
    return lista;
}


int tamanhoTabelaHash(){
        long long int matricula;
        int cont;
        char nome[50];
        cont = 0;
        FILE *arquivo = NULL;
        arquivo =fopen("../Matriculas.txt", "r");
        if (arquivo != NULL) {
            while (!feof(arquivo)) {   
                fscanf(arquivo,"%[^\n]s", nome);
                fscanf(arquivo,"%I64d\n", &matricula);
                cont++;
            }
        }
        fclose(arquivo);
        printf("\n\nQuantidade de matriculas: %d",cont);
        return cont;
}

int descobreNumeroPrimo(long long int num) {
    int i;

    if (num <= 1){
        return 0;
    }
    if (num <= 3){
        return 1;  
    }
    if (num % 2 == 0 || num % 3 == 0){
        return 0;
    }
    for (i = 5; i * i <= num; i += 6) {      // Verificação de números primos
        if (num % i == 0 || num % (i + 2) == 0) 
            return 1;
    }

    return 1;
}

int proximoPrimo(int tamanho_hash) {
    while (descobreNumeroPrimo(tamanho_hash) != 1) {      // Incrementa até encontrar o próximo número primo

        tamanho_hash++;
    }   
    return tamanho_hash;
}


void defineMetodoDeEnderecamentoHash(THash *hash) {
    int op;
    printf("\n\n----------------------- DEFINIR METODO DE ENDERECAMENTO HASH -----------------------\n");
    printf("\nDigite 1 para utilizar o metodo divisao. Como Default, será utilizado o metodo da multiplicacao\n");
    scanf("%d",&op);

    switch (op) {
        case 1:
            hash->metodo = 1;
            break;
        default:
            hash->metodo = 2;
            break;
    }
}


int tamanhoVetorHash(THash *hash) {
    int opcao, tamanho;

    printf("\n\nEscolha quantos porcento maior que a quantidade de matriculas será a hash (como padrão, sera 150%%)\n");
    printf("\n100%% ");
    printf("\n120%% ");
    printf("\n150%% \n\n");
    scanf("%d",&opcao);

    hash->t0 = clock();

    switch (opcao) {
        case 100:
            tamanho = proximoPrimo(tamanhoTabelaHash());
            break;
        case 120:
            tamanho = proximoPrimo((tamanhoTabelaHash()*1.2));
            break;
        default:
            tamanho = proximoPrimo((tamanhoTabelaHash()*1.5));
            break;
    }

    printf("\nO tamanho da tabela hash e: %d",tamanho);

    return tamanho;
}
    

THash *inicializaHash() {
    THash *hash=(THash*)malloc(sizeof(THash));

    defineMetodoDeEnderecamentoHash(hash);
    
    hash->A=(double)rand() / RAND_MAX;
    hash->tamanho = tamanhoVetorHash(hash);
    hash->lista = inicializaLista(hash->tamanho);
    hash->quantidade = 0;
    
    return hash;
}


void lerArquivo(THash *hash){
        long long int valor;
        int posicao;
        char nome[50];
        
        FILE *arquivo = fopen("../Matriculas.txt","r");
        if (arquivo != NULL) {
            while (!feof(arquivo)) {
                fscanf(arquivo,"%[^\n]s",nome);
                fscanf(arquivo,"%I64d\n",&valor);
                
                posicao = funcaoHash(hash, valor, hash->tamanho);
                if((pesquisar(&hash->lista[posicao], valor)==NULL)) {
                    inserirMatricula(hash, valor, nome);
                }
            }       
                
        }
        fclose(arquivo); 
}

void gravarNoArquivo(TLista *L, int tamanho){
    TMatricula *atual;
    FILE *arquivo = fopen("../Matriculas.txt","w");
    int i;
    for(i = 0; i < tamanho; i++) {
        atual = L[i].inicio;
        while (atual!=NULL) {   
            fprintf(arquivo,"%s\n", atual->nome);
            fprintf(arquivo,"%I64d\n", atual->num_matricula);
            atual=atual->proximo;
        }
    }
    
    fclose(arquivo);
}


int funcaoHash(THash *hash, long long int matricula, int tamanho){
    int posicao;
    long long int aux;
    long double aux1;
    
    if(hash->metodo==1) {
    posicao = matricula % tamanho;
    return posicao;
    }
    if(hash->metodo==2) {
        aux1 = matricula * hash->A;
        aux = aux1; //atribui ao aux apenas a parte inteira do numero
        aux1= aux1 - aux; //zera a parte inteira e apenas as casas decimais sao guardadas no aux1
        posicao = aux1 * tamanho;
    }
    return posicao;
}


void inserirMatricula(THash *hash, long long int matricula, char *nome){
    TMatricula *novo_elemento=(TMatricula*) malloc(sizeof(TMatricula)); 
    int i = funcaoHash(hash, matricula, hash->tamanho); //calcula a chave em que a matricula sera inserida
    
    novo_elemento->num_matricula = matricula;
    strcpy(novo_elemento->nome, nome); 
    novo_elemento->proximo = NULL;
    
    TMatricula *atual;  
    TMatricula *anterior;
    
    int inserido = 0;

    if(hash->lista[i].inicio == NULL) { 
        hash->lista[i].inicio = novo_elemento;
        hash->lista[i].fim = novo_elemento;
        inserido = 1;
    }
    else {
        atual = hash->lista[i].inicio;
        anterior = NULL;
        while (atual!=NULL) {
            if (atual->num_matricula >= novo_elemento->num_matricula) {
                novo_elemento->proximo = atual;
                if(atual==hash->lista[i].inicio) {
                    hash->lista[i].inicio = novo_elemento;
                }
                else {
                    anterior->proximo = novo_elemento;
                }
                inserido = 1;
                break;
            }
            anterior = atual;
            atual = atual->proximo;
        }
        if(inserido == 0){
            anterior->proximo = novo_elemento;
            hash->lista[i].fim = novo_elemento;
            inserido = 1;
        }       
    }
    if (inserido != 0) {
        hash->quantidade++;
    }
        
}


void removerMatricula(THash *hash, long long int matricula){
    TMatricula *atual, *anterior;
    int i = funcaoHash(hash, matricula, hash->tamanho);
    atual = hash->lista[i].inicio;
    anterior = NULL;
    int remover = 0;

    if (hash->lista[i].inicio != NULL) {
        if(atual->num_matricula == matricula) {
            hash->lista[i].inicio = hash->lista[i].inicio->proximo;
            remover = 1;
        }
        else {
            while (atual != NULL) {
                if (atual->num_matricula == matricula) {
                    anterior->proximo = atual->proximo;
                    if(atual == hash->lista[i].inicio) {
                        hash->lista[i].inicio = anterior;
                        anterior->proximo = NULL;
                    }
                    remover = 1;
                    break;
                }
                anterior = atual;
                atual = atual->proximo;
            }
        }
    }
    if(remover != 1){
        printf("\n\nERRO: nao foi possivel remover a matricula!");
    }
    else{ 
        printf("\n\nMatricula removida com sucesso");
        hash->quantidade--;
        free(atual);
    }

    

}

TMatricula *pesquisar(TLista *L, long long int matricula){
    TMatricula *pesq;
    pesq = L->inicio;
    if(L->inicio != NULL) {
        while (pesq != NULL) {
            if (pesq->num_matricula == matricula) {
                return pesq;
            }
        pesq = pesq->proximo;
        }
    }
    pesq = NULL;
    return pesq;
}

void imprimirMatricula(TMatricula *matricula){
    if(matricula != NULL) {
        printf("\n\n -------------- INFORMACOES DA MATRICULA: --------------");
        printf("\n\n Nome: %s", matricula->nome);
        printf("\n Matricula: %I64d\n", matricula->num_matricula);
        printf("\n---------------------------------------------------------");
    }
    else
        printf("\n\nERRO: A matricula nao existe\n");
}

void imprimirListaDeMatricula(TLista *L, int tamanho){
    TMatricula *atual;
    int i;
   
    printf("\n -------------- MATRICULAS --------------\n");
        for(i=0; i < tamanho; i++) {
        atual = L[i].inicio;
        if(atual == NULL) {
            printf("\n [%d] - Espaco Vazio\n", i); //printa os espacos vazios
        }
        while (atual != NULL) {
            printf("\n[%05d] Matricula: %I64d\nNome: %s \n", i, atual->num_matricula, atual->nome);
            atual=atual->proximo;
        }
    }
    printf("\n----------------------------------------\n\n");
}

long long int ler_matricula(){
    long long int matricula;
    printf("\ninsira o numero de matricula do aluno: ");
    scanf("%I64d",&matricula);
    fflush(stdin);
    return matricula;
}
void ler_nome(char *nome){
    printf("\nInsira o Nome do aluno: ");
    scanf("%[^\n]s",nome);
    fflush(stdin);
}


void opcoesMenu(int *op){
    printf("\n\nESCOLHA UMA OPCAO:\n");
    printf("\n1) Inserir matricula");
    printf("\n2) Imprimir matriculas");
    printf("\n3) Remover matricula");
    printf("\n4) Pesquisar matricula");
    printf("\n5) Quantidade de Matriculas.");
    printf("\n0) Finalizar programa");
    printf("\n\n");
    scanf("%d", op);
    fflush(stdin);

}


void menu(THash *hash) {
    double tempo;
    int opcao, posicao;
    long long int matricula;
    char nome[50];
    
    hash->t1=clock();
    tempo = (double) (hash->t1 - hash->t0) / CLOCKS_PER_SEC;
    printf("\nTempo de processamento (s): %f", tempo);
    
    do {
        opcoesMenu(&opcao);   
        switch (opcao) {
        case 1:
            matricula = ler_matricula();
            posicao = funcaoHash(hash , matricula, hash->tamanho);
            if(pesquisar(&hash->lista[posicao], matricula) == NULL) {
                ler_nome(nome);
                inserirMatricula(hash, matricula, nome);
                printf("\n\nMatricula inserida com sucesso!\n");
            }
            else {
                printf("\n\nERRO: A matricula já está cadastrada no sistema\n");
            }
            break;
        case 2:
            imprimirListaDeMatricula(hash->lista, hash->tamanho);
            break;
        case 3:
            matricula = ler_matricula();
            posicao = funcaoHash(hash, matricula, hash->tamanho);
            removerMatricula(hash, matricula);
            break;
        case 4:
            matricula = ler_matricula();
            posicao = funcaoHash(hash, matricula, hash->tamanho);

            imprimirMatricula(pesquisar(&hash->lista[posicao], matricula));
            break;
        case 5:
            printf("\n\n\n\nQuantidade de matriculas cadastradas no sistema: %d\n\n", hash->quantidade);
            break;
        case 0:
            printf("Programa Finalizado!");
            break;
        default:
            break;
        }
    } while (opcao != 0);

}

int main(){
    THash *tabela_hash = inicializaHash();
    
    lerArquivo(tabela_hash);
    menu(tabela_hash);
    
    gravarNoArquivo(tabela_hash->lista, tabela_hash->tamanho); //Grava no final do programa para salvar as alterações
    free(tabela_hash);

    return 0;
}

