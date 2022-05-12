#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/random.h>
#include <string.h>

// constantes responsáveis por definir o tamanho máximo de cada campo
#define MAX_LENGTH_FIRST_NAME 32
#define MAX_LENGTH_LAST_NAME 32
#define USERNAME_LENGTH 6
#define PASSWORD_LENGTH 6
#define MAX_LENGTH_DATA_STRING 512
// constantes responsáveis por definir o nome padrão para os arquivos de dados e de ataque
#define FILENAME "data.txt"
#define ATTACKFILENAME "data-attack.txt"

// Função responsável por retornar o hash de uma senha
// Recebe a senha e o hash como paramentro
// Retorna o poteiro de uma string com o hash gerado
char *password_hash(char *password)
{
    char *hashed_password = crypt(password, "$6$rounds=20000$$");
    return hashed_password;
}

// Função responsável por comparar dois hash
// Recebe o hash extraido do arquivo de dados e o hash de ataque
// Retorna 1 caso os hash sejam equivalentes e 0 caso não sejam
int password_hash_compare(char *database_hashed_password, char *attack_hashed_password)
{
    if (strcasecmp(database_hashed_password, attack_hashed_password) == 0)
    {
        return 1;
    }

    return 0;
};

// função responsável por comparar o hash extraido do arquivo de dados com os hashs do arquivo de ataque
// percorrerá o arquivo de ataque por completo em busca de correspondencias
// recebe como parametro o hash extraido do arquivo de dados
// retorna 1 caso encontre correspondecia e 0 caso não encontre
int find_hash_match(char *database_hashed_password)
{
    // váriavel que terá o retorno caso existe correspondencia ou não entre os hashs
    int password_hash_match = 0;
    FILE *fp = fopen(ATTACKFILENAME, "r");

    char *file_password = malloc(PASSWORD_LENGTH);
    char *attack_hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    // percorre o arquivo todo e armazena as linhas nas respectivas variaveis
    while (fscanf(fp, "%s ; %s ;;\n", file_password, attack_hashed_password) != EOF)
    {
        if (password_hash_compare(database_hashed_password, attack_hashed_password) == 1)
        {
            password_hash_match = 1;
            break;
        }
    }

    // caso encontre correspondencias exibe no terminal qual é a senha e o hash correspondente
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
// função responsável por percorrer o arquivo de dados e chamar a função que verifica se existe algum hash correspondente com o arquivo de ataque
void find_password_hash()
{
    FILE *fp = fopen(FILENAME, "r");

    if (!fp)
    {
        printf("%s não encontrado no diretório. \n", FILENAME);
        return;
    }

    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *username = malloc(USERNAME_LENGTH);
    char *hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    int password_hash_match = 0;

    printf("Percorrendo arquivo de senhas em busca de correspondencias. \n");
    printf("Aguarde...\n");

    // percorre o arquivo de dados
    while (fscanf(fp, "%s ; %s ; %s ; %s ;;\n", first_name, last_name, username, hashed_password) != EOF)
    {
        // chama a função responsável por comparar o hash do arquivo de dados com o hash do arquivo de ataque
        //  se encontrar atribui 1 para a variavel password_hash_match e sai do loop
        if ((find_hash_match(hashed_password)) == 1)
        {
            password_hash_match = 1;
            break;
        }
    }

    // caso não encontre nenhuma correspondencia exibe uma mensagem dizendo que não foi encontrado
    if (password_hash_match == 0)
    {
        printf("\nNenhuma correspondencia encontrada!\n");
    }

    fclose(fp);

    free(first_name);
    free(last_name);
    free(hashed_password);
}

// função responsável por salvar os dados no arquivo de ataque
// recebe a senha de 6 digitos e o ponteiro de um arquivo
// irá salvar no arquivo a senha e o seu respectivo hash, separados pelo seguinte padrão
// senha ; hash ;;
void save_data(char *password, FILE *fp)
{
    // ponteiro para a string que irá conter a linha que será salva no arquivo
    char *data_string = malloc(MAX_LENGTH_DATA_STRING);

    strcat(data_string, password);
    strcat(data_string, " ; ");
    strcat(data_string, password_hash(password));
    strcat(data_string, " ;;\n");

    // escreve a string no final do arquivo
    fputs(data_string, fp);

    free(data_string);
}

// função responsável por gerar todas as senhas de 6 digitos, do 0 ao 9
void generate_all_numbers()
{
    FILE *file = fopen(ATTACKFILENAME, "a+");
    char *data_string = malloc(PASSWORD_LENGTH);

    printf("\nGerando arquivo com todas as senhas de 6 digitos do 0 ao 9.\n");
    printf("Aguarde...\n");
    // laços de repetição para gerar a combinaçao das senhas
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
                            // converte os números inteiros dos iteradores dos laços de repetição em uma string
                            sprintf(data_string, "%d%d%d%d%d%d", a, b, c, d, e, f);

                            // passa a string gerada para a função responsável por salvar os dados no arquivo de ataque
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
    // verifica se o arquivo de ataque existe no diretório
    FILE *fp = fopen(ATTACKFILENAME, "r");

    if (fp)
    {
        // se o arquivo existir, chama a função responsável por comparar o arquivo de dados com o arquivo de ataque
        find_password_hash();
    }
    else
    {
        // caso o arquivo não exista, chama-se a função responsável por gerar todas as senhas e criar o arquivo contendo elas
        printf("%s não encontrado. \n", ATTACKFILENAME);
        generate_all_numbers();
    }

    fclose(fp);

    return 0;
}