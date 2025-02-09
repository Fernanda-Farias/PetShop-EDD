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

Animal* inicioAguardando = NULL;
Animal* inicioEmAndamento = NULL;
Animal* inicioEntregueOuCancel = NULL;
Animal* inicioFinalizado = NULL;
Animal* todosAnimais = NULL;

int contId = 0;
int contEmAndamento = 0;

// limpar o buffer
void entradaDados(char* dado, size_t tamanhoDado) {
    if (fgets(dado, tamanhoDado, stdin)) {
        dado[strcspn(dado, "\n")] = '\0'; // Remove o '\n'
    }
}

// inserindo os dados na fila
Animal* inserirDados(char* nome, char* tutor, int servico) {
    Animal* novoAnimal = malloc(sizeof(Animal));
    if (!novoAnimal) {
        printf("Erro: nao foi possivel alocar memoria!\n");
        return NULL;
    }

    novoAnimal->id = ++contId;
    strcpy(novoAnimal->nome, nome);
    strcpy(novoAnimal->tutor, tutor);
    strcpy(novoAnimal->status, "Aguardando");
    novoAnimal->proximo = NULL;

    // Definir o serviço
    if (servico == 1) {
        strcpy(novoAnimal->servico, "Banho");
    } else if (servico == 2) {
        strcpy(novoAnimal->servico, "Tosa");
    } else if (servico == 3) {
        strcpy(novoAnimal->servico, "Banho e Tosa");
    } else {
        printf("Servico invalido! Animal nao cadastrado.\n");
        free(novoAnimal);
        return NULL;
    }

    // Adicionar na fila de aguardando
    if (inicioAguardando == NULL) {
        inicioAguardando = novoAnimal;
    } else {
        Animal* aux = inicioAguardando;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }

    // Adicionar na lista de todos os animais
    if (todosAnimais == NULL) {
        todosAnimais = novoAnimal;
    } else {
        Animal* aux = todosAnimais;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }
    printf("Animal cadastrado!\n");
}

// cadastro do animal
void cadastrarAnimal() {
    char nome[100], tutor[100];
    int servico;

    printf("\nInforme o nome do animal: ");
    entradaDados(nome, sizeof(nome));
    printf("\nInforme o nome do tutor: ");
    entradaDados(tutor, sizeof(tutor));
    printf("\nQual servico voce deseja realizar:");
    printf("\n1- Banho | 2- Tosa | 3- Banho e tosa: ");
    scanf("%d", &servico);
    getchar();

    inserirDados(nome, tutor, servico);
}

// procurar um animal pelo Id
Animal* procurarAnimal() {
    Animal* animalProcurado = todosAnimais;
    int id;

    printf("Informe o Id do animal: ");
    scanf("%d", &id);

    while (animalProcurado) {
        if (animalProcurado->id == id) {
            printf("Dados do animal: \nId: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s", animalProcurado->id, animalProcurado->nome, animalProcurado->tutor, animalProcurado->servico, animalProcurado->status);
            return animalProcurado;
        }
        animalProcurado = animalProcurado->proximo;
    }
    printf("O animal não foi encontrado!\n");
    return NULL;
}

// funcao p remover um animal de uma lista especifica
void removerAnimalDaLista(Animal** lista, int id) {
    Animal* atual = *lista;
    Animal* anterior = NULL;

    while (atual) {
        if (atual->id == id) {
            if (anterior == NULL) {
                *lista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

// Funcao p add um animal na nova lista quando ele muda o status
void addAnimalNoDestino(Animal* animal, char novoStatus[15], Animal** localDestino){
    Animal* novoAnimal = malloc(sizeof(Animal));
    if (!novoAnimal) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    *novoAnimal = *animal;
    strcpy(novoAnimal->status, novoStatus);
    novoAnimal->proximo = NULL;

    // Adicionar na lista de destino
    if (*localDestino == NULL) {
        *localDestino = novoAnimal;
    } else {
        Animal* aux = *localDestino;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }

    printf("Animal com Id %d movido para '%s'.\n", animal->id, novoStatus);
}

// Mover o primeiro animal da fila de aguardando p em andamento
void moverAnimalAguardando() {
    if (inicioAguardando == NULL) {
        printf("Nao ha animais aguardando atendimento.\n");
        return;
    }
    if (contEmAndamento >= 3) {
        printf("Nao foi possivel adicionar um animal em andamento, pois ja ha 3 animais na fila.\n");
        return;
    }

    // Pegar o primeiro animal da fila de aguardando
    Animal* animal = inicioAguardando;
    inicioAguardando = inicioAguardando->proximo; // Atualizar o início da fila

    // Adicionar na lista de "Em andamento"
    strcpy(animal->status, "Em andamento");
    animal->proximo = NULL;

    if (inicioEmAndamento == NULL) {
        inicioEmAndamento = animal;
    } else {
        Animal* aux = inicioEmAndamento;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = animal;
    }

    contEmAndamento++;
    printf("Animal com Id %d está com atendimentio em andamento.\n", animal->id);
}

// Mover um animal escolhido pelo usuario (lista) p ser finalizado
void moverAnimalEmAndamento() {

    Animal* animal = procurarAnimal();

    if (animal == NULL || strcmp(animal->status, "Em andamento") != 0) {
        printf("Animal invalido!");
        return;
    }

    // Mover pra lista de finalizados
    strcpy(animal->status, "Finalizado");
    addAnimalNoDestino(animal, "Finalizado", &inicioFinalizado);

    // Remover da lista de andamento
    removerAnimalDaLista(&inicioEmAndamento, animal->id);
    contEmAndamento--;
    printf("Atendimento do animal com ID %d finalizado.\n", animal->id);
}

// Função p exibir uma lista/fila especifica
void exibirAnimaisLocal(Animal* destino) {
    if (destino == NULL) {
        printf("Nenhum animal nesta lista.\n");
        return;
    }
    while (destino != NULL) {
        printf("Id: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s\n", destino->id, destino->nome, destino->tutor, destino->servico, destino->status);
        destino = destino->proximo;
    }
}

// cancelar servico se tiver aguardando
void cancelarServico() {
    Animal* animalCancelar = procurarAnimal();

    if (strcmp(animalCancelar->status, "Aguardando") == 0) {
        removerAnimalDaLista(inicioAguardando, animalCancelar->id);
        printf("Animal de id %d excluido com sucesso!", animalCancelar->id);
    } else if (strcpy(animalCancelar->status, "Finalizado")!= 0) {
        printf("Erro ao remover o animal, pois ele não está mais aguardando atendimento.");
    } else {
        printf("Erro ao remover animal!");
    }

}

// exibir todos os animais que passaram no sistema
void exibirTodosAnimais() {
    printf("---Exibindo todos os animais---");
    while (todosAnimais != NULL) {
        printf("\nId: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s\n", todosAnimais->id, todosAnimais->nome, todosAnimais->tutor, todosAnimais->servico, todosAnimais->status);
        todosAnimais = todosAnimais->proximo;
    }
}

// atualizar algum dado do animal. não botei o id pq ner, vai bagunçar meu progresso
void atualizarDadoAnimal() {
    Animal* animalAlterar = procurarAnimal();
    int escolha;
    char nomeOuTutor[100];

    printf("Qual dado você deseja alterar do animal?");
    printf("\n1- Nome \n2- Tutor \n3- Servico \n4- Status \nOpcao: ");
    scanf("%d", &escolha);
    getchar();

    switch (escolha) {
        case 1:
            printf("Informe o novo nome do animal: ");
            entradaDados(nomeOuTutor, sizeof(nomeOuTutor));
            strcpy(animalAlterar->nome, nomeOuTutor);
        break;
        case 2:
            printf("Informe o novo nome do tutor: ");
            entradaDados(nomeOuTutor, sizeof(nomeOuTutor));
            strcpy(animalAlterar->tutor, nomeOuTutor);
        break;
        case 3:
            if (strcmp(animalAlterar->status, "Aguardando") == 0) {
                moverAnimalAguardando();
            } else if (strcmp(animalAlterar->status, "Aguardando") != 0) {
                printf("Não foi possivel alterar o servico, pois o animal não está mais aguardando o atendimento.");
            } else {
                printf("Erro ao alterar servico!");
            }
        break;
        case 4:
            if (strcmp(animalAlterar->status, "Aguardando") == 0) {
                moverAnimalAguardando();
            } else if (strcmp(animalAlterar->status, "Em andamento") == 0) {
                moverAnimalEmAndamento();
            } else {
                printf("Erro ao alterar status!");
            }
        break;
    }
}

int main() {
    int escolha, opcao;
    //APÓS FAZER A PILHA E A LISTA DE ANIMAIS CANCELADOS E ENTREGUES, REVER O CODIGO, POIS ALGUNS ITENS PRECISAM QUE ADICIONE A PILHA E LISTA
    //Que eu lembre: rever o exibirAnimaisLocal, pra ver se tbm roda cm pilha; cancelarServico p add servicos cancelados na lista de cancelados e entregues;
    //rever o atualizar dado animal, pra adicionar a possibilidade dos outros status; rever o if e else q tem na main
    //amanhã vou verificar duas coisas q tao dando errado: a lista de todos os animais, pq os animais tão sumindo, e o atualizardados, pq ele ta quebrando

    printf("----- Seja Bem-vindo(a) ao Pet Shop Mundo Cão!-----");
    do {
        printf("\nSelecione a opcao desejada:");
        printf(" \n1- Cadastrar Animal \n2- Procurar animal \n3- Visualizar animais por status \n4- Exibir todos os Animais \n5- Chamar animal p/atendimento");
        printf("\n6- Cancelar servico \n7- Atualizar dado do animal \n8- Finalizar servico  \n20- Sair \nOpcao: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                cadastrarAnimal();
            break;
            case 2:
                procurarAnimal();
            break;
            case 3:
                printf("Qual status você quer visualizar os animais: ");
                printf("\n1- Aguardando \n2- Em andamento \n3- Finalizado \n4- Cancelado/Entregue");
                printf("\nOpcao: ");
                scanf("%d", &opcao);
                getchar();

                if (opcao == 1) {
                    exibirAnimaisLocal(inicioAguardando);
                } else if (opcao == 2) {
                    exibirAnimaisLocal(inicioEmAndamento);
                } else if (opcao == 3) {
                    printf("pendente");
                } else if (opcao == 4) {
                    printf("pendente");
                } else {
                    printf("Opcao Invalida!");
                }
            break;
            case 4:
                exibirTodosAnimais();
            break;
            case 5:
                moverAnimalAguardando();
            break;
            case 6:
                cancelarServico();
            break;
            case 7:
                atualizarDadoAnimal();
            break;
            case 8:
                moverAnimalEmAndamento();
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
