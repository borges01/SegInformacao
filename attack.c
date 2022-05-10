#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/random.h>
#include <string.h>

#define FILENAME "data.txt"
#define ATTACKFILENAME "data-attack.txt"
#define MAX_LENGTH_FIRST_NAME 32
#define MAX_LENGTH_LAST_NAME 32
#define MAX_LENGTH_PASSWORD 6
#define MAX_LENGTH_DATA_STRING 512

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
int password_hash_compare(char *database_hashed_password, char *attack_hashed_password)
{
    if (strcasecmp(database_hashed_password, attack_hashed_password) == 0)
    {
        return 1;
    }

    return 0;
};

void save_data(char *password, FILE *fp)
{
    char *data_string = malloc(MAX_LENGTH_DATA_STRING);

    strcat(data_string, password);
    strcat(data_string, " ; ");
    strcat(data_string, password_hash(password));
    strcat(data_string, " ;;\n");

    fputs(data_string, fp);

    free(data_string);
}

int compare_hash(char *database_hashed_password)
{
    int password_hash_match = 0;
    FILE *fp = fopen(ATTACKFILENAME, "r");

    char *file_password = malloc(MAX_LENGTH_PASSWORD);
    char *attack_hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    while (fscanf(fp, "%s ; %s ;;\n", file_password, attack_hashed_password) != EOF)
    {
        if (password_hash_compare(database_hashed_password, attack_hashed_password) == 1)
        {
            password_hash_match = 1;
            break;
        }
    }

    if (password_hash_match == 1)
    {
        printf("\nSenha encontrada!\n");
        printf("Senha: %s \nHash: %s \n", file_password, attack_hashed_password);
    }

    fclose(fp);
    free(file_password);
    free(attack_hashed_password);

    return password_hash_match;
}

void find_password_hash()
{

    FILE *fp = fopen(FILENAME, "r");

    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    int password_hash_match = 0;

    printf("Percorrendo arquivo de senhas em busca de correspondencias. \n");
    printf("Aguarde...\n");

    while (fscanf(fp, "%s ; %s ; %s ;;\n", first_name, last_name, hashed_password) != EOF)
    {
        if ((compare_hash(hashed_password)) == 1)
        {
            password_hash_match = 1;
            break;
        }
    }

    if (password_hash_match == 0)
    {
        printf("\nNenhuma correspondencia encontrada!\n");
    }

    fclose(fp);

    free(first_name);
    free(last_name);
    free(hashed_password);
}

void generate_all_numbers()
{
    FILE *file = fopen(ATTACKFILENAME, "a+");
    char *data_string = malloc(MAX_LENGTH_PASSWORD);

    printf("\nGerando arquivo com todas as senhas de 6 digitos do 0 ao 9.\n");
    printf("Aguarde...\n");

    for (int a = 0; a <= 9; a++)
    {
        for (int b = 0; b <= 9; b++)
        {
            for (int c = 0; c <= 9; c++)
            {
                for (int d = 0; d <= 9; d++)
                {
                    for (int e = 0; e <= 9; e++)
                    {
                        for (int f = 0; f <= 9; f++)
                        {
                            sprintf(data_string, "%d%d%d%d%d%d", a, b, c, d, e, f);

                            save_data(data_string, file);
                        }
                    }
                }
            }
        }
    }

    fclose(file);
    free(data_string);

    printf("\nArquivo de senhas gerado.\n");
}
int main(void)
{
    FILE *fp = fopen(ATTACKFILENAME, "r");

    if (fp)
    {
        find_password_hash();
    }
    else
    {
        printf("Arquivo não encontrado. \n");
        generate_all_numbers();
    }

    fclose(fp);

    return 0;
}