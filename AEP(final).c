#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int op;
int opmenu;
int y[100];
char cript[17];
char descript[17];
char nome[100][20]; 
char senhai[100][17];

//Funções de escolha do usuario

void cadastro();
void pesquisa();
void excluir();
void lista();
void creditos();
void adm();

// Funções de senha e criptografia

void senha(char senhai[]);
int tam(char s[]);
int num(char s[]);
int maiusc(char s[]);
int minusc(char s[]);
int sp(char s[]);
void cripto(char s[], char crip[], int lock);
void descripto(char cript[], char descript[], int lock);


int main(void) {
    do {
        srand(time(0));
        printf("\n|----- M  E  N  U -----|\n");
        printf("| 1 - Cadastrar        |\n");
        printf("| 2 - Pesquisar        |\n");
        printf("| 3 - Excluir          |\n");
        printf("| 4 - Listar           |\n");
        printf("| 5 - Creditos         |\n");
        printf("| 6 - Sair             |\n");
        printf("|----------------------|\n");
        printf("Selecione a sua opcao: ");
        scanf("%d", &opmenu);
        
        switch (opmenu) {
            case 1:
                cadastro();
                break;
            case 2:
                pesquisa();
                break;
            case 3:
                excluir();
                break;
            case 4:
                lista();
                break;
            case 5:
                creditos();
                break;
            case 6:
                printf("Encerrando o programa. \n");
                break;
            case 7:
                adm();
                break;
            default:
                printf("Opcao invalida. Tente novamente. \n");
        }
    } while (opmenu != 6);

    return 0;
}

// Função de cadastro de usuário

void cadastro() {
    char nome[20];
    char senhai[17];
    FILE *arquivo = fopen("listaUsuarios.txt", "a"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo. \n");
        return;
    }
    do {
        printf("\nDigite o nome do usuario: ");
        scanf("%s", nome);
        senha(senhai);
        int y = rand() % 3 + 1;  // Gera chave aleatória de criptografia
        cripto(senhai, cript, y);  // Criptografa a senha
        fprintf(arquivo, "%s\n", nome);  // Grava o nome do usuário
        fprintf(arquivo, "%s\n", cript);  // Grava a senha criptografada
        fprintf(arquivo, "%d\n", y);  // Grava a chave de criptografia
        printf("\nDigite 1 para continuar ou 2 para sair: ");
        scanf("%d", &op);
    } while (op == 1);

    fclose(arquivo);
}

// Função de pesquisa de usuário

void pesquisa() {
    char UsuarioPesquisa[20], linha[100];
    int encontrado = 0;
    FILE *arquivo = fopen("listaUsuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo. \n");
        return;
    }
    printf("\nDigite o usuario que deseja pesquisar: ");
    scanf("%s", UsuarioPesquisa);
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;
        if (strcmp(linha, UsuarioPesquisa) == 0) {
            printf("\nUsuario encontrado: %s \n", linha);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("\nUsuario nao encontrado. \n");
    }
    fclose(arquivo);
}

// Função para excluir usuário

void excluir() {
    char UsuarioExcluir[20], linha[100];
    int encontrado = 0;
    FILE *arquivo = fopen("listaUsuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos. \n");
        return;
    }
    printf("\nDigite o usuario que deseja excluir: ");
    scanf("%s", UsuarioExcluir);
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; 
        if (strcmp(linha, UsuarioExcluir) == 0) {
            encontrado = 1;
            fgets(linha, sizeof(linha), arquivo);
            fgets(linha, sizeof(linha), arquivo);
        } else {
            fprintf(temp, "%s\n", linha);
        }
    }
    fclose(arquivo);
    fclose(temp);
    remove("listaUsuarios.txt");
    rename("temp.txt", "listaUsuarios.txt");
    if (encontrado) {
        printf("\nUsuario excluido com sucesso.\n");
    } else {
        printf("\nUsuario nao encontrado.\n");
    }
    adm();
}

// Função para listar usuários

void lista() {
    char linha[50];
    int linha_num = 0; 
    int user_num = 1; 
    FILE *arquivo = fopen("listaUsuarios.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\n--- Lista de Usuarios Cadastrados ---\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';
        if (linha_num % 3 == 0) {
            printf("\nUsuario %d:\n", user_num);
            printf("Nome: %s\n", linha);
        } else if (linha_num % 3 == 1) {
            printf("Senha criptografada: %s\n", linha);
        } else if (linha_num % 3 == 2) {
            printf("Chave de criptografia: %s\n", linha);
            user_num++;
        }

        linha_num++;
    }

    fclose(arquivo);
}


void creditos() {
    printf("\nEste programa foi feito pelos alunos Pedro Alves Franco e Jose Antonio Kassius, ambos da turma A da Unicesumar! \n");
}

// Função de senha

void senha(char senhai[]) {
    int val = 0;
    char senha[17], senhac[17];
    do {
        printf("\nDigite sua senha: \n");
        scanf("%s", senha);
        if (tam(senha)) {
            printf("\nA senha deve ter entre 8 a 16 caracteres \n");
        } else if (!num(senha)) {
            printf("\nA senha deve conter pelo menos um numero \n");
        } else if (minusc(senha)) {
            printf("\nA senha deve conter pelo menos uma letra minuscula \n");
        } else if (maiusc(senha)) {
            printf("\nA senha deve conter pelo menos uma letra maiuscula \n");
        } else if (sp(senha)) {
            printf("\nA senha deve conter pelo menos um caracter especial \n");
        } else {
            val = 1;
        }
    } while (val != 1);
    strcpy(senhai, senha);
}
// Tamanho da senha

int tam(char s[]) {
	int x = strlen(s);
	if (x >= 8 && x <= 16){
		return 0;
	} else {
		return 1;
	}
}

// Presença de numero

int num(char s[]) {
   int v = 0;
   int x;
   for(x = 0; x < strlen(s); x++){
   	 if(isdigit(s[x])){
   	 	v = v + 1;
		}
	}
	if (v > 0){
	    return 1;	
	} else {
		return 0;
	}
}

// Presença se letras maiusculas

int maiusc(char s[]){
	int v = 0;
	int x;
   for(x = 0;x <= strlen(s); x++){
   	 if((s[x])>=65 && (s[x])<=90){
   	 	v = v + 1;
		}
	}
	if (v > 0){
	    return 0;	
	} else {
		return 1;
	}
}

// Presenaça de letras minusculas

int minusc(char s[]){
	int v = 0;
	int x;
   for(x = 0;x <= strlen(s); x++){
   	 if((s[x])>=97 && (s[x])<=122){
   	 	v = v + 1;
		}
	}
	if (v > 0){
	    return 0;	
	} else {
		return 1;
	}
}

// Presença de caracter especial

int sp(char s[]) {
    int v = 0;
    int x;
    for (x = 0;x < strlen(s); x++){
        if ((s[x] >= 33 && s[x] <= 47) || (s[x] >= 58 && s[x] <= 64) || (s[x] >= 91 && s[x] <= 96)) {
            v = v + 1;
        }
    }
    if (v > 0) {
        return 0;  
    } else {
        return 1;
    }
}

//Criptografia aleatória

void cripto(char s[],char crip[], int lock) {
    int x;
    if (lock == 1) {
        for (x = 0; x < 17 && s[x] != '\0'; x++) {
            if (x % 2 == 1) {
                crip[x] = s[x] + 10;
            } else {
                crip[x] = s[x] - 15;
            }
        }
    } else if (lock == 2) {
        for (x = 0; x < 17 && s[x] != '\0'; x++) {
            if (x % 2 == 1) {
                crip[x] = s[x] + 5;
            } else {
                crip[x] = s[x] - 15;
            }
        }
    } else if (lock == 3) {
        for (x = 0; x < 17 && s[x] != '\0'; x++) {
            if (x % 2 == 1) {
                crip[x] = s[x] + 10;
            } else {
                crip[x] = s[x] - 5;
            }
        }
    }
    crip[x] = '\0';
}

// Descriptografia

void descripto(char cript[], char descript[], int lock) {
    int x;
    if (lock == 1) {
        for (x = 0; x < 17 && cript[x] != '\0'; x++) {
            if (x % 2 == 1) {
                 cript[x] = cript[x] - 10;
            } else {
                descript[x] = cript[x] + 15;
            }
        }
    } else if (lock == 2) {
        for (x = 0; x < 17 && cript[x] != '\0'; x++) {
            if (x % 2 == 1) {
                descript[x] = cript[x] - 5;
            } else {
                descript[x] = cript[x] + 15;
            }
        }
    } else if (lock == 3) {
        for (x = 0; x < 17 && cript[x] != '\0'; x++) {
            if (x % 2 == 1) {
                descript[x] = cript[x] - 10;
            } else {
                descript[x] = cript[x] + 5;
            }
        }
    }
    descript[x] = '\0';
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void adm() {
    char nome[20], senhaCript[17], linha[100];
    int chave;
    FILE *arquivo = fopen("listaUsuarios.txt", "r");
    FILE *arquivoADM = fopen("lista_adm.txt", "w");

    if (arquivo == NULL || arquivoADM == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    while (fgets(nome, sizeof(nome), arquivo) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';

        fgets(senhaCript, sizeof(senhaCript), arquivo);
        senhaCript[strcspn(senhaCript, "\n")] = '\0';

        fgets(linha, sizeof(linha), arquivo);
        chave = atoi(linha);

        descripto(senhaCript, descript, chave);

        fprintf(arquivoADM, "Nome: %s\nSenha: %s\nChave: %d\n\n", nome, descript, chave);
    }

    fclose(arquivo);
    fclose(arquivoADM);
}
