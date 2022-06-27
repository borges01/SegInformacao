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
// constantes responsáveis por definir o nome padrão para o arquivo de dados
#define FILENAME "data.txt"

// Função responsável por realizar o print do menu inicial
void print_menu()
{
    printf("\nMenu \n");
    printf("1 - Registrar-se \n");
    printf("2 - Login \n");
    printf("0 - Sair \n");
};

// valida se o usuário digitado possui 6 digitos
// recebe a o usuário digitada
// retorna 1 para usuário com a 6 digitos e 0 para não
int username_length(char *username)
{
    int username_size = strlen(username);

    if (username_size != USERNAME_LENGTH)
    {
        return 0;
    }

    return 1;
}

// valida se a senha digitada está de acordo com a politica de senhas estabelecida
// recebe a senha digitada
// retorna 1 para senha de acordo com a politica e 0 para não
int password_is_strong(char *password)
{
    int password_size = strlen(password);

    if (password_size != PASSWORD_LENGTH)
    {
        return 0;
    }

    return 1;
}

// Função responsável por retornar o hash de uma senha
// Recebe a senha e o hash como paramentro
// Retorna o poteiro de uma string com o hash gerado
char *password_hash(char *password)
{
    char *hashed_password = crypt(password, "$6$rounds=20000$$");
    return hashed_password;
}

// Função responsável por verificar se o hash da senha digitada corresponde ao hash salvo no arquivo
// Recebe a senha e o hash como paramentro
// Retorna 1 caso os hash sejam equivalentes e 0 caso não sejam
int password_hash_compare(char *password, char *hashed_password)
{
    if (strcasecmp(hashed_password, password_hash(password)) == 0)
    {
        return 1;
    }

    return 0;
};

// Função responsável por verificar se o username digitado corresponde ao username salvo no arquivo
// Recebe o username digitado no login e o username salvo no arquivo como paramentro
// Retorna 1 caso os usernames sejam equivalentes e 0 caso não sejam
int username_compare(char *input_username, char *file_username)
{
    if (strcasecmp(input_username, file_username) == 0)
    {
        return 1;
    }

    return 0;
};

// Função responsável por registrar um usuário em um arquivo
//  recebe os dados do usuário como parametro
void register_user(char *first_name, char *last_name, char *username, char *password)
{
    // ponteiro para a string que irá conter a linha que será salva no arquivo
    char *data_string = malloc(MAX_LENGTH_DATA_STRING);

    // é criado a linha que será salva no arquivo, seguindo o seguinte padrão
    // primeiro_nome ; sobrenome ; usuário ; hash_senha ;;
    strcat(data_string, first_name);
    strcat(data_string, " ; ");
    strcat(data_string, last_name);
    strcat(data_string, " ; ");
    strcat(data_string, username);
    strcat(data_string, " ; ");
    strcat(data_string, password_hash(password));
    strcat(data_string, " ;;\n");

    FILE *fp = fopen(FILENAME, "a+");

    // a linha com os dados é inserida ao final do arquivo e com uma quebra de linha no final
    fputs(data_string, fp);

    fclose(fp);

    free(data_string);

    printf("\nUsuário registrado com sucesso! \n");
}

// função responsável por realizar o login do usuário
// recebe o usuário e a senha como parametro
void login(char *input_username, char *input_password)
{

    FILE *fp = fopen(FILENAME, "r");

    // caso não exista nenhum arquivo de dados a função login não é continuada
    if (!fp)
    {
        printf("%s não encontrado no diretório. \n", FILENAME);
        return;
    }

    int authenticated = 0;

    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *username = malloc(USERNAME_LENGTH);
    char *hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    // percorre o arquivo de dados em busca do usuário digitado
    while (fscanf(fp, "%s ; %s ; %s ; %s ;;\n", first_name, last_name, username, hashed_password) != EOF)
    {
        // verifica se a linha em questão possui usuário e senha correspondentes aos que foram digitados
        if (username_compare(input_username, username) == 1 && password_hash_compare(input_password, hashed_password) == 1)
        {
            // caso encontre correspondencia, a variável responsável por identificar o usuário como autenticado recebe 1 e o loop é interrompido
            authenticated = 1;
            break;
        }
    }

    // caso o usuário esteja autenticado uma mensagem de boas vindas é exibido
    if (authenticated == 1)
    {
        printf("\nLogado!\n");
        printf("Seja bem vindo %s!\n", first_name);
    }
    else
    {
        // caso não seja encontrado nenhuma correspondencia no arquivo, uma mensagem é exibida para o usuário
        printf("\nErro ao tentar autenticar-se, verifique login e senha!\n");
    }

    fclose(fp);
    free(first_name);
    free(last_name);
    free(username);
    free(hashed_password);
}

void a() {
    printf("\n Executano função a");
}

void b() {
    printf("\n Executano função b");
}

void c() {
    printf("\n Executano função c");
}

void d() {
    printf("\n Executano função d");
}

int main(void)
{
    // variável responsável por coletar a opção do usuário ao entrar no sistema
    int choice;
    // variáveis responsáveis pelos dados digitados pelo usuário
    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *username = malloc(USERNAME_LENGTH);
    char *password = malloc(PASSWORD_LENGTH);

    // enquanto o usuário não digitar 0 para sair, o programa é executado
    while (choice != 0)
    {
        // exibe o menu e recolhe a opção desejada pelo usuário
        print_menu();
        scanf("%i", &choice);

        switch (choice)
        {
        case 1:
            printf("\nRegistre-se\n");

            // caso o usuário digite 1, os dados de registro são solicitados
            printf("Digite seu primeiro nome: \n");
            scanf("%s", first_name);

            printf("Digite seu segundo nome: \n");
            scanf("%s", last_name);

            printf("Digite seu usuário: \n");
            printf("O usuário deve conter 6 caracteres alfanuméricos: \n");
            scanf("%s", username);

            // verifica o usuário digitado até ele ser um usuário válido de 6 digitos
            while (username_length(username) != 1)
            {
                printf("O usuário deve conter 6 caracteres alfanuméricos: \n");
                scanf("%s\n", username);
            }

            printf("Digite sua senha: \n");
            printf("A senha deve conter 6 caracteres númericos: \n");
            scanf("%s", password);

            // verifica a senha digitada até ela ser uma válido de 6 digitos
            while (password_is_strong(password) != 1)
            {
                printf("A senha deve conter 6 caracteres númericos: \n");
                scanf("%s\n", password);
            }

            // chama a função responsável por registrar os dados do usuário no arquivo de dados
            register_user(first_name, last_name, username, password);

            break;
        case 2:
            printf("Login\n");

            // caso o usuário digite 2, os dados de login são solicitados
            printf("Digite seu usuário: \n");
            scanf("%s", username);

            printf("Digite sua senha: \n");
            printf("A senha deve conter 6 caracteres númericos: \n");
            scanf("%s", password);

            // chama a função responsável por realizar o login do usuário
            login(username, password);

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
    free(username);
    free(password);
}