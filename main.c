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
#define MAX_LENGTH_PAPER 6
#define MAX_LENGTH_DATA_STRING 512
// constantes responsáveis por definir o nome padrão para o arquivo de dados
#define FILENAME "data.txt"
#define FILENAME1 "file1.txt"
#define FILENAME2 "file2.txt"

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
void register_user(char *first_name, char *last_name, char *username, char *paper, char *password)
{
    // ponteiro para a string que irá conter a linha que será salva no arquivo
    char *data_string = malloc(MAX_LENGTH_DATA_STRING);

    // é criado a linha que será salva no arquivo, seguindo o seguinte padrão
    // primeiro_nome ; sobrenome ; usuário ; papel ; hash_senha ;;
    strcat(data_string, first_name);
    strcat(data_string, " ; ");
    strcat(data_string, last_name);
    strcat(data_string, " ; ");
    strcat(data_string, username);
    strcat(data_string, " ; ");
    strcat(data_string, paper);
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

// funções genéricas para exemplificar o controle de acesso
void a()
{
    printf("\nExecutando função a\n");
}

void b()
{
    printf("\nExecutando função b\n");
}

void c()
{
    printf("\nExecutando função c\n");
}

void d()
{
    printf("\nExecutando função d\n");
}

// função responsável por iterar e ler o conteúdo de um arquivo
// recebe o ponteiro do arquivo como parametro
void read_file(FILE *file)
{
    if (!file)
    {
        printf("Arquivo não encontrado no diretório. \n");
        return;
    }

    // variável auxiliar que irá conter o conteúdo da linha do arquivo
    char *data = malloc(MAX_LENGTH_DATA_STRING);

    // o arquivo é percorrido
    while (fscanf(file, "%s ;;\n", data) != EOF)
    {
        // a linha é printada
        printf("%s \n", data);
    }

    free(data);
}

// função responsável por sobrescrever o conteúdo de um arquivo
// recebe o ponteiro do arquivo e o conteúdo a ser incrementado como parametro
void overwrite_file(FILE *file, char *content)
{
    if (!file)
    {
        printf("Arquivo não encontrado no diretório. \n");
        return;
    }

    // é adicionado o padrão com um espaço, dois pontos e vírgula e uma quebra de linha no arquivo
    strcat(content, " ;;\n");

    // o conteúdo é escrito no arquivo
    fputs(content, file);

    printf("Arquivo atualizado com sucesso. \n");
}

// função responsável por excluir o conteúdo de um arquivo
// recebe o ponteiro do arquivo como parametro
void delete_content(FILE *file)
{
    if (!file)
    {
        printf("Arquivo não encontrado no diretório. \n");
        return;
    }

    // o conteúdo é escrito no arquivo
    fputs(" ;;\n", file);

    printf("Arquivo atualizado com sucesso. \n");
}

// função responsável por incrementar o conteúdo de um arquivo
// recebe o ponteiro do arquivo e o conteúdo a ser incrementado como parametro
void append_file(FILE *file, char *content)
{
    if (!file)
    {
        printf("Arquivo não encontrado no diretório. \n");
        return;
    }

    // é adicionado o padrão com um espaço, dois pontos e vírgula e uma quebra de linha no arquivo
    strcat(content, " ;;\n");

    // o conteúdo é escrito no arquivo
    fputs(content, file);

    printf("Arquivo atualizado com sucesso. \n");
}

// função responsável por deletar um arquivo
// recebe o nome do arquivo como parametro
void delete_file(char *filename)
{
    remove(filename);
    printf("Arquivo deletada com sucesso. \n");
}

// função responsável por criar um arquivo "vazio"
// recebe o nome do arquivo como parametro
void create_file(char *filename)
{
    FILE *file = fopen(filename, "w");

    // o conteúdo é escrito no arquivo
    fputs(" ;;\n", file);

    fclose(file);
    printf("Arquivo criado com sucesso. \n");
}

// função responsável por printar o menu baseado no papel do usuário e nas regras de controle de acesso que foram
// estabelecidas
void print_menu_by_paper(char *paper)
{
    // verifica se o papel do usuário logado é o papel1 e mostra as opções para este papel
    if (strcasecmp(paper, "papel1") == 0)
    {
        printf("\nEscolha sua ação \n");
        printf("1 - Executar função a \n");
        printf("2 - Executar função b \n");
        printf("3 - Executar função c \n");
        printf("4 - Executar função d \n");
        printf("5 - Ler x\n");
        printf("6 - Ler y\n");
        printf("7 - Ler z\n");
        printf("11 - Ler arquivo 1\n");
        printf("13 - Sobrescrever arquivo 1\n");
        printf("15 - Adicionar conteúdo arquivo 1\n");
        printf("17 - Apagar conteúdo arquivo 1\n");
        printf("19 - Criar arquivo 1\n");
        printf("21 - Deletar arquivo 1\n");

        printf("0 - Sair \n");
    }

    // verifica se o papel do usuário logado é o papel2 e mostra as opções para este papel
    if (strcasecmp(paper, "papel2") == 0)
    {
        printf("\nEscolha sua ação \n");
        printf("1 - Executar função a \n");
        printf("2 - Executar função b \n");
        printf("3 - Executar função c \n");
        printf("4 - Executar função d \n");
        printf("5 - Ler x\n");
        printf("6 - Ler y\n");
        printf("7 - Ler z\n");
        printf("8 - Alterar x\n");
        printf("9 - Alterar y\n");
        printf("10 - Alterar z\n");

        printf("0 - Sair \n");
    }

    // verifica se o papel do usuário logado é o papel3 e mostra as opções para este papel
    if (strcasecmp(paper, "papel3") == 0)
    {
        printf("\nEscolha sua ação \n");
        printf("5 - Ler x\n");
        printf("6 - Ler y\n");
        printf("7 - Ler z\n");
        printf("12 - Ler arquivo 2\n");
        printf("14 - Sobrescrever arquivo 2\n");
        printf("16 - Adicionar conteúdo arquivo 2\n");
        printf("18 - Apagar conteúdo arquivo 2\n");
        printf("20 - Criar arquivo 2\n");
        printf("22 - Deletar arquivo 2\n");

        printf("0 - Sair \n");
    }
}

void restricted_area(char *paper)
{
    printf("\n\n");
    printf("Área autenticada\n\n");

    // váriavel responsável por recolher a opção do usuário
    int choice;
    // variáveis que serão manipuladas a partir de um papel
    int x = 0, y = 0, z = 0;
    // variável auxiliar para inserir conteúdo em arquivo
    char *content = malloc(MAX_LENGTH_DATA_STRING);
    // ponteiro de arquivo que será manipulado a partir de um papel
    FILE *file;

    while (choice != 0)
    {
        print_menu_by_paper(paper);
        scanf("%i", &choice);

        switch (choice)
        {
        case 1:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa a função a
            a();

            break;
        case 2:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa a função b
            b();

            break;
        case 3:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa a função c
            c();

            break;
        case 4:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa a função d
            d();

            break;
        case 5:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0 && !strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }
            // código responsável por exibir o valor de x
            printf("Valor de x: %d\n", x);

            break;
        case 6:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0 && !strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }
            // código responsável por exibir o valor de y
            printf("Valor de y: %d\n", y);

            break;
        case 7:
            if (!strcasecmp(paper, "papel1") == 0 && !strcasecmp(paper, "papel2") == 0 && !strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }
            // código responsável por exibir o valor de z
            printf("Valor de z: %d\n", z);

            break;

        case 8:
            if (!strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // código responsável por alterar o valor de x
            printf("Valor de x: %d\n", x);
            printf("Digite um novo valor para x: \n");
            scanf("%d", &x);

            printf("Valor de x: %d\n", x);

            break;
        case 9:
            if (!strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }
            // código responsável por alterar o valor de y
            printf("Valor de y: %d\n", y);
            printf("Digite um novo valor para y: \n");
            scanf("%d", &y);

            printf("Valor de x: %d\n", y);
            break;

        case 10:

            if (!strcasecmp(paper, "papel2") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // código responsável por alterar o valor de z
            printf("Valor de z: %d\n", z);
            printf("Digite um novo valor para z: \n");
            scanf("%d", &z);

            printf("Valor de x: %d\n", z);
            break;

        case 11:

            if ((!strcasecmp(paper, "papel1") == 0))
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME1, "r");

            // executa função para ler arquivo
            read_file(file);

            fclose(file);
            break;

        case 12:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME2, "r");

            // executa função para ler arquivo
            read_file(file);

            fclose(file);
            break;

        case 13:

            if (!strcasecmp(paper, "papel1") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME1, "w");

            printf("Digite o conteúdo desejado: \n");
            scanf("%s", content);

            // executa função para sobrescrever arquivo
            overwrite_file(file, content);

            fclose(file);
            break;

        case 14:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME2, "w");

            printf("Digite o conteúdo desejado: \n");
            scanf("%s", content);

            // executa função para sobrescrever arquivo
            overwrite_file(file, content);

            fclose(file);
            break;

        case 15:

            if (!strcasecmp(paper, "papel1") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME1, "a");

            printf("Digite o conteúdo desejado: \n");
            scanf("%s", content);

            // executa função para adicionar conteúdo ao final do arquivo
            append_file(file, content);

            fclose(file);
            break;

        case 16:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME2, "a");

            printf("Digite o conteúdo desejado: \n");
            scanf("%s", content);

            // executa função para adicionar conteúdo ao final do arquivo
            append_file(file, content);

            fclose(file);
            break;

        case 17:

            if (!strcasecmp(paper, "papel1") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }
            file = fopen(FILENAME1, "w");

            // executa função para apagar conteúdo de arquivo
            delete_content(file);

            fclose(file);
            break;

        case 18:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            file = fopen(FILENAME2, "w");

            // executa função para apagar conteúdo de arquivo
            delete_content(file);

            fclose(file);
            break;

        case 19:

            if (!strcasecmp(paper, "papel1") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa função para criar arquivo
            create_file(FILENAME1);
            break;

        case 20:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa função para criar arquivo
            create_file(FILENAME2);
            break;

        case 21:

            if (!strcasecmp(paper, "papel1") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa função para deletar arquivo
            delete_file(FILENAME1);
            break;

        case 22:

            if (!strcasecmp(paper, "papel3") == 0)
            {
                printf("\nVocê não tem acesso ao recurso solicitado.\n");
                break;
            }

            // executa função para deletar arquivo
            delete_file(FILENAME2);
            break;

        default:
            printf("\nDigite uma opção valida \n");

            break;
        }
    }

    free(content);
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
    char *paper = malloc(MAX_LENGTH_PAPER);
    char *hashed_password = malloc(MAX_LENGTH_DATA_STRING);

    // percorre o arquivo de dados em busca do usuário digitado
    while (fscanf(fp, "%s ; %s ; %s ; %s ; %s ;;\n", first_name, last_name, username, paper, hashed_password) != EOF)
    {
        // verifica se a linha em questão possui usuário e senha correspondentes aos que foram digitados
        if (username_compare(input_username, username) == 1 && password_hash_compare(input_password, hashed_password) == 1)
        {
            // caso encontre correspondencia, a variável responsável por identificar o usuário como autenticado recebe 1 e o loop é interrompido
            authenticated = 1;
            break;
        }
    }

    // caso o usuário esteja autenticado uma mensagem de boas vindas é exibido junto com a área autenticada
    if (authenticated == 1)
    {
        printf("\nLogado!\n");
        printf("Seja bem vindo %s!\n", first_name);

        // área autenticada com demais funcionalidades do sistema é exibida
        restricted_area(paper);
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
    free(paper);
    free(hashed_password);
}

int main(void)
{
    // variável responsável por coletar a opção do usuário ao entrar no sistema
    int choice, role;
    // variáveis responsáveis pelos dados digitados pelo usuário
    char *first_name = malloc(MAX_LENGTH_FIRST_NAME);
    char *last_name = malloc(MAX_LENGTH_LAST_NAME);
    char *username = malloc(USERNAME_LENGTH);
    char *paper = malloc(MAX_LENGTH_PAPER);
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

            // usuário escolhe qual é o seu papel no sistema
            printf("Escolha o seu papel: \n");
            printf("1) papel1 \n2) papel2 \n3) papel3\n");
            scanf("%i", &role);

            switch (role)
            {
            case 1:
                paper = "papel1";
                break;
            case 2:
                paper = "papel2";
                break;
            case 3:
                paper = "papel3";
                break;

            default:
                paper = "papel1";
                break;
            }

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
            register_user(first_name, last_name, username, paper, password);

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