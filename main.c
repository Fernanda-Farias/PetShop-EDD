//Fernanda Oliveira de Farias e Giovanna Costa Oliveira
//Favor testar com carinho
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_MAX 50 //tamanho max da pilha de saída

typedef struct Animal {
    int id;
    char nome[100];
    char tutor[100];
    char servico[15];
    char status[15]; //Aguardando, em andamento, finalizado, entregue e cancelado
    struct Animal* proximo;
} Animal;

//saída tipo pilha
typedef struct {
    Animal* itens[SIZE_MAX];
    int topo;
} PilhaOut;

//declaracao das filas/pilhas/listas
PilhaOut pilhaOut;
Animal* inicioAguardando = NULL;
Animal* inicioEmAndamento = NULL;
Animal* inicioEntregueOuCancel = NULL;
Animal* inicioFinalizado = NULL;
Animal* todosAnimais = NULL;

//declaracao de variaveis globais (contadores)
int contId = 0;
int contEmAndamento = 0;

//Funcao que aloca memoria para a struct Animal
Animal* alocar_estrutura() {
    Animal* animal = (Animal*)malloc(sizeof(Animal));
    if(!animal) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }
    return animal;
}

// Procedimento para desalocar memória de uma struct
void desalocar_estrutura(Animal* animal){
    free(animal);
    animal = NULL;
}

// Funcao para liberar toda a memória alocada para uma lista encadeada
void liberar_memoria(Animal* animal) {
    Animal* atual = animal;
    while (atual != NULL){
        Animal* proximo = atual->proximo;
        desalocar_estrutura(atual);
        atual = proximo;
    }
}

//limpa o buffer de entrada
void entradaDados(char* dado, size_t tamanhoDado) {
    if (fgets(dado, tamanhoDado, stdin)) {
        dado[strcspn(dado, "\n")] = '\0';
    }
}

//verifica se todos os animais estao na lista de todos os animais
void verificarTodosAnimais(Animal* animal) {
    Animal* aux = todosAnimais;
    while (aux != NULL && aux->id != animal->id) {
        aux = aux->proximo;
    }

    if (aux == NULL) {
        //adiciona o animal se não estiver na lista
        animal->proximo = todosAnimais;
        todosAnimais = animal;
    }
}

//insere os dados do animal cadastrado
Animal* inserirDados(char* nome, char* tutor, int servico) {
    Animal* novoAnimal = malloc(sizeof(Animal));
    if (!novoAnimal) {
        printf("Erro! Nao foi possivel alocar memoria.\n");
        return NULL;
    }

    novoAnimal->id = ++contId;
    strcpy(novoAnimal->nome, nome);
    strcpy(novoAnimal->tutor, tutor);
    strcpy(novoAnimal->status, "Aguardando");
    novoAnimal->proximo = NULL;

    //definir o servico pelo numero que o usuario selecionou
    if (servico == 1) {
        strcpy(novoAnimal->servico, "Banho");
    } else if (servico == 2) {
        strcpy(novoAnimal->servico, "Tosa");
    } else if (servico == 3) {
        strcpy(novoAnimal->servico, "Banho e Tosa");
    } else {
        printf("\nServico invalido! Animal nao cadastrado.");
        free(novoAnimal);
        return NULL;
    }

    //adiciona o animal na fila de animais aguardando atendimento
    if (inicioAguardando == NULL) {
        inicioAguardando = novoAnimal;
    } else {
        Animal* aux = inicioAguardando;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }
    verificarTodosAnimais(novoAnimal);
    printf("\nAnimal cadastrado!");
    return novoAnimal;
}

//cadastra o animal (recebe os dados do usuario)
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

//iniciar pilha
void iniciarPilhaOut(PilhaOut *p) {
    p->topo = -1;
}

//verificar se a pilha de saida esta cheia: 1 cheia(50) e 0 <= size_max
//int pra retornar um valor 
int lotouPilhaOut (PilhaOut *p) {
    return p->topo == SIZE_MAX -1;
}

//empilhar animal
void pushPilhaOut(PilhaOut *p, Animal* animal) {
    if (lotouPilhaOut(p)) {
        printf("Erro! A pilha de saida dos animais esta cheia!\n");
        return;
    }
    p->itens[++(p->topo)] = animal;
}

//procura um animal pelo Id
Animal* procurarAnimal() {
    Animal* animalProcurado = todosAnimais;
    int id;

    printf("\nInforme o Id do animal: ");
    scanf("%d", &id);
    getchar();

    while (animalProcurado) {
        if (animalProcurado->id == id) {
            printf("\nDados do animal: \nId: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s", animalProcurado->id, animalProcurado->nome, animalProcurado->tutor, animalProcurado->servico, animalProcurado->status);
            return animalProcurado;
        }
        animalProcurado = animalProcurado->proximo;
    }
    printf("\nO animal nao foi encontrado!");
    return NULL;
}

//remove um animal de uma lista especifica pra alocar ele pra nova lista (operacao realizada quando o animal muda de status para outro)
void removerAnimalDaLista(Animal** lista, int id) {
    Animal* atual = *lista;
    Animal* anterior = NULL;

    while (atual != NULL) {
        if (atual->id == id) {
            if (anterior == NULL) {
                *lista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

//adiciona um animal na nova lista quando ele muda o status
void addAnimalNoDestino(Animal* animal, char novoStatus[15], Animal** localDestino){
    Animal* novoAnimal = malloc(sizeof(Animal));

    if (!novoAnimal) {
        printf("\nErro ao alocar memoria!");
        return;
    }

//recebe os dados do animal
    novoAnimal->id = animal->id;
    strcpy(novoAnimal->nome, animal->nome);
    strcpy(novoAnimal->tutor, animal->tutor);
    strcpy(novoAnimal->servico, animal->servico);
    strcpy(novoAnimal->status, novoStatus);
    novoAnimal->proximo = NULL;

//adiciona na lista de destino
    if (*localDestino == NULL) {
        *localDestino = novoAnimal;
    } else {
        Animal* aux = *localDestino;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novoAnimal;
    }
    printf("\nAnimal com Id %d movido para '%s'.", animal->id, novoStatus);
}

//move o primeiro animal da fila de "Aguardando" para a fila de "Em Andamento"
void moverAnimalAguardando() {
    if (inicioAguardando == NULL) {
        printf("\nNao ha animais aguardando atendimento.");
        return;
    }
    if (contEmAndamento >= 3) {
        printf("\nNao foi possivel adicionar um animal em andamento, pois ja tem 3 animais na fila.");
        return;
    }

//pega o primeiro animal da fila de "Aguardando"
    Animal* animal = inicioAguardando;
    inicioAguardando = inicioAguardando->proximo; //atualiza a fila apos a remocao

//adiciona na lista de "Em andamento"
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
    printf("\nAnimal com Id %d esta com atendimento em andamento.", animal->id);
}

/*
 *Um animal atendido primeiro pode realizar um servico de banho e tosa, e o animal seguinte realiza um servico so de banho.
 * Como 3 animais podem estar em andamento ao mesmo tempo, pode ocorrer de o segundo animal terminar primeiro que o primeiro animal,
 * pois o servico dele era menor. Nesse caso, o atendimento finalizado não tem ordem, é o que acabar primeiro. Portanto, o usuario
 * (possivelmente a pessoa que estava executando o servico), é quem informa o animal que ela finalizou para ir para a pilha de entrega
 */
 void moverAnimalEmAndamento() {
    Animal* animal = procurarAnimal();

    if (animal == NULL || strcmp(animal->status, "Em andamento") != 0) {
        printf("Animal invalido!");
        return;
    }

//move para a lista de "Finalizados"
    strcpy(animal->status, "Finalizado");
    pushPilhaOut(&pilhaOut, animal);
    addAnimalNoDestino(animal, "Finalizado", &inicioFinalizado);

//remove da lista de "Em Andamento"
    removerAnimalDaLista(&inicioEmAndamento, animal->id);
    contEmAndamento--;
    verificarTodosAnimais(animal);
    printf("\nAtendimento do animal %s com ID %d finalizado.", animal->nome, animal->id);
}

//Exibir os animais finalizados utilizando pilha
void exibirFinalizados (PilhaOut *p) {
    if (p->topo == -1) {
        printf("Nao ha animais na pilha de finalizados.\n");
        return;
    }
    printf("---Exibindo animais finalizados---\n");
    for (int i = p->topo; i>= 0; i--) {
        Animal* pet = p->itens[i];
        printf("Id: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s\n", pet->id, pet->nome, pet->tutor, pet->servico, pet->status);
    }
}

//exibe uma lista/fila especifica
void exibirAnimaisLocal(Animal* destino) {
    if (destino == NULL) {
        printf("Nenhum animal nesta lista.\n");
        return;
    }

    Animal* aux = destino;
    while (aux != NULL) {
        printf("Id: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s\n", aux->id, aux->nome, aux->tutor, aux->servico, aux->status);
        aux = aux->proximo;
    }
}

//cancela o servico apenas se o status for "Aguardando"
void cancelarServico() {
    Animal* animalCancelar = procurarAnimal();

    if (strcmp(animalCancelar->status, "Aguardando") == 0) {
        removerAnimalDaLista(&inicioAguardando, animalCancelar->id);
        strcpy(animalCancelar->status, "Cancelado");
        addAnimalNoDestino(animalCancelar, "Cancelado", &inicioEntregueOuCancel);
        printf("\nAnimal %s de ID %d excluido com sucesso!", animalCancelar->nome, animalCancelar->id);
    } else {
        printf("\nErro ao remover o animal, pois ele nao esta mais aguardando atendimento.");
    }
}

//exibe todos os animais que passaram pelo sistema
void exibirTodosAnimais() {
    if (todosAnimais == NULL) {
        printf("Ainda nao ha animais cadastrados.\n");
        return;
    }
    printf("---Exibindo todos os animais---");
    Animal* animais = todosAnimais;

    while (animais != NULL) {
        printf("\nId: %d, Nome: %s, Tutor: %s, Servico: %s, Status: %s\n", animais->id, animais->nome, animais->tutor, animais->servico, animais->status);
        animais = animais->proximo;
    }
}

//atualiza algum dado do animal, de acordo com a necessidade do usuario (não atualiza id porque vira bagunça)
void atualizarDadoAnimal() {
    Animal* animalAlterar = procurarAnimal();
    int escolha;
    char nomeOuTutor[100];

    if (animalAlterar == NULL) {
        return;
    }

    printf("\nQual dado voce deseja alterar do animal?");
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
                int novoServico;
                printf("\nQual novo servico voce deseja realizar:");
                printf("\n1- Banho | 2- Tosa | 3- Banho e tosa: ");
                scanf("%d", &novoServico);
                getchar();

                if (novoServico == 1) {
                    strcpy(animalAlterar->servico, "Banho");
                } else if (novoServico == 2) {
                    strcpy(animalAlterar->servico, "Tosa");
                } else if (novoServico == 3) {
                    strcpy(animalAlterar->servico, "Banho e Tosa");
                } else {
                    printf("\nServico invalido!");
                }
            } else {
                printf("\nNao foi possivel alterar o servico, pois o animal não esta mais aguardando o atendimento.");
            }
        break;
        case 4:
            if (strcmp(animalAlterar->status, "Aguardando") == 0) {
                moverAnimalAguardando();
            } else if (strcmp(animalAlterar->status, "Em andamento") == 0) {
                moverAnimalEmAndamento();
            } else if (strcmp(animalAlterar->status, "Finalizado") == 0) {
                entregarPets();
            }else {
                printf("Erro ao alterar status!");
            }
        break;
        default:
            printf("Opcao Invalida!");
        break;
    }
}

// Entrega com no minimo 3 animais, usando pilha
void entregarPets() {
    int minimo = pilhaOut.topo + 1;
    if (minimo < 3) {
        printf("Erro! Entregar com no minimo 3 animais em espera.\n");
        return;
    }
    printf("---Entregando---\n");
    while (pilhaOut.topo >= 0) {
        Animal* animalEntregue = pilhaOut.itens[pilhaOut.topo--];
        strcpy(animalEntregue->status,"Entregue");
        addAnimalNoDestino(animalEntregue, "Entregue", &inicioEntregueOuCancel);
        removerAnimalDaLista(&inicioFinalizado, animalEntregue->id);
        printf("O animal %s, com Id %d foi entregue\n", animalEntregue->nome, animalEntregue->id);
    }    
}

int main() {
    int escolha, opcao;
    iniciarPilhaOut(&pilhaOut);

    printf("----- Seja Bem vindo(a) ao Pet Shop Mundo Cao! -----");
    do {
        printf("\nSelecione a opcao desejada:");
        printf(" \n1- Cadastrar Animal \n2- Pesquisar animal \n3- Visualizar animais por status \n4- Exibir todos os Animais \n5- Chamar animal p/atendimento");
        printf("\n6- Cancelar servico \n7- Atualizar dado do animal \n8- Finalizar servico \n9- Entregar animais \n10- Sair \nOpcao: ");
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
                printf("Qual status voce quer visualizar os animais: ");
                printf("\n1- Aguardando \n2- Em andamento \n3- Finalizado \n4- Cancelado/Entregue");
                printf("\nOpcao: ");
                scanf("%d", &opcao);
                getchar();

                if (opcao == 1) {
                    exibirAnimaisLocal(inicioAguardando);
                } else if (opcao == 2) {
                    exibirAnimaisLocal(inicioEmAndamento);
                } else if (opcao == 3) {
                    exibirFinalizados(&pilhaOut);
                } else if (opcao == 4) {
                    exibirAnimaisLocal(inicioEntregueOuCancel);
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
            case 9:
                entregarPets();
            break;
            case 10:
                liberar_memoria(inicioAguardando);
                liberar_memoria(inicioEmAndamento);
                liberar_memoria(inicioEntregueOuCancel);
                liberar_memoria(inicioFinalizado);
                liberar_memoria(todosAnimais);
                printf("Memoria liberada! Saindo do sistema...");
            break;
            default:
                printf("Opção inválida");
            break;

        }
    } while (escolha != 10);

    return 0;
}
