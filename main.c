#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Animal {
    int id;
    char nome[100];
    char tutor[100];
    char servico[15];
    char status[15];
    struct Animal* proximo;
} Animal;

//as duas filas, de aguardando e em andamento
Animal* inicioAguardando = NULL;
Animal* inicioEmAndamento = NULL;

//fiz oq pude e não pude muito pois sou uma mulher cansada
//Vou fazer só amanhã a função de exibir todos os animais, pq tava quebrando td ai eu apaguei logo ne amanha é um novo dia eu acho, e tbm pq
//não sei ainda como vou fazer o "exibir", já q tem 2 filas, 1 pilha e 1 lista (dos finalizados e cancelados). eu have q estudar como
//falta eu fazer p amanhã ainda (fernanda): procurar animal, exibir todas as listas

//bagui de limpar o buffer
void entradaDados(char* dado, size_t tamanhoDado) {
    if (fgets(dado, tamanhoDado, stdin)) {
        dado[strcspn(dado, "\n")] = '\0'; // Remove o '\n' capturado
    }
}

//inserindo os dados na fila de aguardando
Animal* inserirDados(char* nome, char* tutor, char* servico) {

    int contId = 0;
    Animal* novoAnimal = malloc(sizeof(Animal));
    if(!novoAnimal) {
        printf("Erro: nao foi possivel alocar memoria!");
        return NULL;
    }

    novoAnimal->id = ++contId;
    strcpy(novoAnimal->nome, nome);
    strcpy(novoAnimal->tutor, tutor);
    strcpy(novoAnimal->servico, servico);
    strcpy(novoAnimal->status, "Aguardando");
    novoAnimal->proximo = NULL;

    if (inicioAguardando == NULL) {
        inicioAguardando = novoAnimal;
    } else {
        Animal* aux = inicioAguardando;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }
    printf("Animal cadastrado!");
}

//aqui é pro usuario inserir os dados do cadastro
void cadastrarAnimal() {
    char nome[100], tutor[100], servico[15];

    printf("\nInforme o nome do animal:");
    entradaDados(nome, sizeof(nome));
    printf("\nInforme o nome do tutor:");
    entradaDados(tutor, sizeof(tutor));
    printf("\nInforme o servico:");
    entradaDados(servico, sizeof(servico));

    inserirDados(nome, tutor, servico);
}

//remover um animal no começo da fila. não é o cancelar
Animal* removerAnimal() {
    if (inicioAguardando == NULL) {
        printf("Não há animais na fila.\n");
        return NULL;
    }

    Animal* animalRemover = inicioAguardando;
    inicioAguardando = inicioAguardando->proximo;
    animalRemover->proximo = NULL;

    printf("Animal removido!\n");
    return animalRemover;
}

//pra mudar de aguardando p andamento
void addEmAndamento() {
    int contador = 0;
    Animal* animalMudar = inicioEmAndamento;
    while(animalMudar != NULL) {
        contador++;
        animalMudar = animalMudar->proximo;
    }
    if (contador >= 3) {
        printf("Serviço lotado: já tem 3 animais em andamento.\n");
        return;
    }
    if (inicioAguardando == NULL) {
        printf("Não há animais aguardando.\n");
        return;
    }
    Animal* animalMovido = removerAnimal();
    if (animalMovido == NULL) {
        return;
    }

    strcpy(animalMovido->status, "em andamento");
    if (inicioEmAndamento == NULL) {
        inicioEmAndamento = animalMovido;
    } else {
        Animal* temp = inicioEmAndamento;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = animalMovido;
    }

    printf("O animal está em atendimento.");
}


int main() {

    int escolha;

    printf("----- Seja Bem-vindo(a) ao Pet Shop Mundo Cão!-----");
    do {
        printf("\nSelecione a opcao desejada:");
        printf(" \n1- Cadastrar Animal \n2- Procurar animal \n3- Excluir Animal \n4- Exibir Animais \n5- Chamar animal p/atendimento \n20- Sair\n");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                cadastrarAnimal();
            break;
            case 2:
            break;
            case 3:
                removerAnimal();
            break;
            case 4:
            break;
            case 5:
                addEmAndamento();
            break;
            case 20:
                printf("Aff veyr que sono");
            break;
            default:
                printf("Opção inválida");
            break;
        }
    } while (escolha != 20);

    return 0;
}