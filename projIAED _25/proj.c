#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BATCH 20
#define MAX_VACCINES 1000

#define ERR_TOO_MANY_VACCINES   "too many vaccines"
#define ERR_DUPLICATE_BATCH     "duplicate batch number"
#define ERR_INVALID_BATCH       "invalid batch"
#define ERR_INVALID_NAME        "invalid name"
#define ERR_INVALID_DATE        "invalid date"
#define ERR_INVALID_QUANTITY    "invalid quantity"

typedef struct {
    char *name;                     /**< vaccine name*/
    char batch[MAX_BATCH + 1];      /**< batch id*/
    int day, month, year;           /**< expiration date*/
    int doses;                      /**< doses avaiable*/
} vaccineBatch;

typedef struct Inoculation {
    char *user;                     /**< user name*/
    char batch[MAX_BATCH + 1];      /**< batch usado*/
    int day, month, year;           /**< inoculation date*/
    struct Inoculation *prox;
} Inoculation;

vaccineBatch vaccines[MAX_VACCINES];    /**< store vaccine batches*/
int totalVaccines = 0;                  /**< batch counter*/
Inoculation *inoculations = NULL;        /**< linked list of inoculations*/

/**< Função para validar nome da vacina*/
int checkVaccineName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (isspace((unsigned char)name[i])) {
            printf("%s\n", ERR_INVALID_NAME);
            return 0;
        }
    }
    return 1;
}

/**< Função para validar batch (20 caracteres hexadecimais)*/
int checkBatchName(const char *batch) {
    int len = strlen(batch);
    if (len > MAX_BATCH) {
        printf("%s\n", ERR_INVALID_BATCH);
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isdigit(batch[i]) && !(batch[i] >= 'A' && batch[i] <= 'F')){
            printf("%s\n", ERR_INVALID_BATCH);
            return 0;
        }
    }
    return 1;
}

/**< Função para validar data*/
int checkDate(int *day, int *month, int *year) {
    if (*day < 1 || *day > 31 || *month < 1 || *month > 12 || *year < 0) {
        printf("%s\n", ERR_INVALID_DATE);
        return 0;
    }
    return 1;
}

/**< Adicionar um novo batch*/
void addBatch(const char *batch, int day, int month, int year, int doses, const char *name) {
    if (totalVaccines >= MAX_VACCINES) {
        printf("%s\n", ERR_TOO_MANY_VACCINES);
        return;
    }

    if (!checkVaccineName(name) || !checkBatchName(batch) || !checkDate(&day, &month, &year) || doses <= 0) {
        return;
    }

    /**< Verificar se o batch já existe*/
    for (int i = 0; i < totalVaccines; i++) {
        if (strcmp(vaccines[i].batch, batch) == 0) {
            printf("%s\n", ERR_DUPLICATE_BATCH);
            return;
        }
    }

    if (doses <= 0) {
        printf("%s\n", ERR_INVALID_QUANTITY);
        return;
    }

    /**< Aloca memória para um novo batch*/
    vaccineBatch *newBatch = (vaccineBatch *)malloc(sizeof(vaccineBatch));
    if (!newBatch) {
        printf("No memory\n");
        return;
    }

    /**< Aloca dinamicamente o name da vacina*/
    newBatch->name = strdup(name);
    if (!newBatch->name) {
        printf("No memory\n");
        free(newBatch);
        return;
    }

    /**< Copia os outros dados*/
    strncpy(newBatch->batch, batch, MAX_BATCH);
    newBatch->batch[MAX_BATCH] = '\0';
    newBatch->day = day;
    newBatch->month = month;
    newBatch->year = year;
    newBatch->doses = doses;

    /**< Adiciona à lista*/
    //vaccines[totalVaccines++] = newBatch;
}

/**< Adicionar inoculação*/
void addInocultion(const char *user, const char *batch, int day, int month, int year) {
    if (!checkDate(&day, &month, &year)) {
        return;
    }

    /**< Verificar se o batch existe e se ainda está válido*/
    int i, found = -1;
    for (i = 0; i < totalVaccines; i++) {
        if (strcmp(vaccines[i].batch, batch) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        return;
    }

    /**< Verificar validade*/
    if (year > vaccines[found].year ||
        (year == vaccines[found].year && month > vaccines[found].month) ||
        (year == vaccines[found].year && month == vaccines[found].month &&
         day > vaccines[found].day)) {
        return;
    }

    /**< Reduzir número de doses*/
    if (vaccines[found].doses <= 0) {
        return;
    }
    vaccines[found].doses--;

    /**< Criar inoculation*/
    Inoculation *inoculation = (Inoculation *)malloc(sizeof(Inoculation));
    if (!inoculation) {
        printf("No memory\n");
        return;
    }

    inoculation->user = strdup(user);
    if (!inoculation->user) {
        printf("No memory\n");
        free(inoculation);
        return;
    }

    strncpy(inoculation->batch, batch, MAX_BATCH);
    inoculation->batch[MAX_BATCH] = '\0';
    inoculation->day = day;
    inoculation->month = month;
    inoculation->year = year;
    inoculation->prox = inoculations;
    inoculations = inoculation;
}

/**< Exibir todas as vaccines registradas*/
void listVaccines() {
    for (int i = 0; i < totalVaccines; i++) {
        printf("Vaccine: %s, Batch: %s, Expiration Date: %02d-%02d-%d, Doses: %d\n",
               vaccines[i].name, vaccines[i].batch, vaccines[i].day, vaccines[i].month, vaccines[i].year, vaccines[i].doses);
    }
}

/**< Exibir todas as inoculações*/
void listInoculations() {
    Inoculation *inoculation = inoculations;
    while (inoculation) {
        printf("user: \"%s\", batch: %s, Data: %02d-%02d-%d\n",
               inoculation->user, inoculation->batch, inoculation->day,
               inoculation->month, inoculation->year);
        inoculation = inoculation->prox;
    }
}

/**< Liberar memória antes de encerrar o programa
void freeMemory() {
    Inoculation *inoculation = inoculations;
    while (inoculation) {
        Inoculation *temp = inoculation;
        inoculation = inoculation->prox;
        free(temp->user);
        free(temp);
    }
    for (int i = 0; i < totalVaccines; i++) {
        free(vaccines[i]->name); // Libera o nome
        free(vaccines[i]);       // Libera o batch
    }
}*/

int main() {
    char action;

    scanf(" %c", &action);  // Captura apenas um caractere
    printf("Você digitou: %c\n", action);
    
    getchar();  // Limpa o \n deixado no buffer

    // Processa a ação com base na entrada
    switch (action) {
        case 'q':
            //freeMemory();
            printf("Encerrando o programa.\n");
            return 0;
        case 'c':
            printf("Processando comando 'c'\n");
            char batch[MAX_BATCH + 1];
            int day, month, year, doses;
            char *name = (char *)malloc(51 * sizeof(char));
            if (name == NULL) {
                printf("No memory\n");
                break;
            }

            getchar(); // Limpa o \n deixado no buffer
            char input[1000];
            fgets(input, sizeof(input), stdin);
            char* token = strtok(input, " ");
            if (token == NULL || sscanf(token, "%20s", batch) != 1) {
                printf("Erro: Não foi possível ler o batch.\n");
                free(name); // Libera a memória alocada antes de sair
                break;
            }

            token = strtok(NULL, " ");
            if (token == NULL || sscanf(token, "%d-%d-%d", &day, &month, &year) != 3) {
                printf("Erro: Não foi possível ler a data.\n");
                free(name); // Libera a memória alocada antes de sair
                break;
            }

            token = strtok(NULL, " ");
            if (token == NULL || sscanf(token, "%d", &doses) != 1) {
                printf("Erro: Não foi possível ler o número de doses.\n");
                free(name); // Libera a memória alocada antes de sair
                break;
            }

            token = strtok(NULL, "\n");
            if (token == NULL || sscanf(token, "%50s", name) != 1) {
                printf("Erro: Não foi possível ler o nome.\n");
                free(name); // Libera a memória alocada antes de sair
                break;
            }

            // Exibe os valores lidos
            printf("batch: %s, date: %02d-%02d-%d, doses: %d, name: %s\n", batch, day, month, year, doses, name);

            /**if (sscanf(strtok(NULL, " "), "%20s", batch) != 1) break;
            if (sscanf(strtok(NULL, " "), "%d-%d-%d", &day, &month, &year) != 3) break;
            if (sscanf(strtok(NULL, " "), "%d", &doses) != 1) break;
            if (sscanf(strtok(NULL, "\n"), "%50s", name) != 1) break;
            printf("batch: %s, date: %02d-%02d-%d, doses: %d, name: %s\n", batch, day, month, year, doses, name);*/

            addBatch(batch, day, month, year, doses, name);
            free(name);
            break;
        case 'l':
            break;
        case 'a':
            break;
        case 'r':
            break;
        case 'd':
            break;
        case 'u':
            break;
        case 't':
            break;
        default:
            printf("Comando desconhecido.\n");
            break;
    }
    
    //freeMemory();
    return 0;
}
