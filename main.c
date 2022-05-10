#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/random.h>
#include <string.h>

#define MAX_LENGTH_FIRST_NAME 32
#define MAX_LENGTH_LAST_NAME 32
#define MAX_LENGTH_PASSWORD 6
#define MAX_LENGTH_DATA_STRING 512
#define FILENAME "data.txt"

// Função responsável por realizar o print do menu inicial
void print_menu()
{
    printf("\n Menu \n");
    printf("1 - Registrar-se \n");
    printf("2 - Login \n");
    printf("0 - Sair \n");
};

// valida se a senha digitada está de acordo com a politica de senhas estabelecida
// recebe a senha digitada
// retorna 1 para senha de acordo com a politica e 0 para não
int password_is_strong(char *password)
{
    int password_size = strlen(password);

    if (password_size != MAX_LENGTH_PASSWORD)
    {
        return 0;
    }

    return 1;
}

// Função responsável por retornar verificar se o hash da senha digitada corresponde ao hash salvo no arquivo
// Recebe a senha e o hash como paramentro
// Retorna 1 caso os hash sejam equivalentes e 0 caso sejam falsos
char *password_hash(char *password)
{
    char *hashed_password = crypt(password, "$6$rounds=20000$$");
    return hashed_password;
}

// Função responsável por retornar verificar se o hash da senha digitada corresponde ao hash salvo no arquivo
// Recebe a senha e o hash como paramentro
// Retorna 1 caso os hash sejam equivalentes e 0 caso sejam falsos
int password_hash_compare(char *password, char *hashed_password)
{
    if (strcasecmp(hashed_password, password_hash(password)) == 0)
    {
        return 1;
    }

    return 0;
};

// Função responsável por retornar verificar se o hash da senha digitada corresponde ao hash salvo no arquivo
// Recebe a senha e o hash como paramentro
// Retorna 1 caso os hash sejam equivalentes e 0 caso sejam falsos
int first_name_compare(char *input_first_name, char *file_first_name)
{
    if (strcasecmp(input_first_name, file_first_name) == 0)
    {
        return 1;
    }

    return 0;
};

void register_user(char *first_name, char *last_name, char *password)
{
    char *data_string = malloc(MAX_LENGTH_DATA_STRING);

    strcat(data_string, first_name);
    strcat(data_string, " ; ");
    strcat(data_string, last_name);
    strcat(data_string, " ; ");
    strcat(data_string, password_hash(password));
    strcat(data_string, " ;;\n");

    FILE *fp = fopen(FILENAME, "a+");

    fputs(data_string, fp);

    fclose(fp);

    free(data_string);

    printf("Usuário registrado com sucesso! \n\n");
}

void login(char *input_first_name, char *input_password)
{

    int authenticated = 0;
    FILE *fp = fopen(FILENAME, "r");

    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    while (fscanf(fp, "%s ; %s ; %s ;;\n", first_name, last_name, hashed_password) != EOF)
    {
        if (password_hash_compare(input_password, hashed_password) == 1 && first_name_compare(input_first_name, first_name) == 1)
        {
            authenticated = 1;
            break;
        }
    }

    if (authenticated == 1)
    {
        printf("\nLogado!\n");
        printf("Seja bem vindo %s!\n", first_name);
    }
    else
    {
        printf("\nErro ao fazer login, verifique login e senha!\n");
    }

    free(first_name);
    free(last_name);
    free(hashed_password);
}

int main(void)
{
    int choice;
    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *password = malloc(MAX_LENGTH_PASSWORD);

    while (choice != 0)
    {
        print_menu();
        scanf("%i", &choice);

        switch (choice)
        {
        case 1:
            printf("\nRegistre-se\n");

            printf("Digite seu primeiro nome: \n");
            scanf("%s", first_name);

            printf("Digite seu segundo nome: \n");
            scanf("%s", last_name);

            printf("Digite sua senha: \n");
            printf("A senha deve conter 6 caracteres númericos: \n");
            scanf("%s", password);

            while (password_is_strong(password) != 1)
            {
                printf("A senha deve conter 6 caracteres númericos: \n");
                scanf("%s\n", password);
            }

            register_user(first_name, last_name, password);

            break;
        case 2:
            printf("Login\n");

            printf("Digite seu primeiro nome: \n");
            scanf("%s", first_name);

            printf("Digite sua senha: \n");
            printf("A senha deve conter 6 caracteres númericos: \n");
            scanf("%s", password);

            login(first_name, password);

            break;
        case 0:
            printf("Obrigado por usar o sistema!\n");
            exit;

        default:
            printf("Digite uma opção válida!\n");
            break;
        }
    }

    free(first_name);
    free(last_name);
    free(password);
}