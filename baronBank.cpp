#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <locale.h>

// Códigos de cor ANSI
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_BLUE "\033[44m"
#define BG_WHITE "\033[47m"

typedef struct {
    int codigo;
    char nome[50];
    char email[50];
    char cpf[20];
    char dataNascimento[20];
    char dataCadastro[20];
} Cliente;

typedef struct {
    int numero;
    Cliente cliente;
    float saldo;
    float limite;
    float saldoTotal;
} Conta;

// Funções
void menu();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
void fecharConta();
void salvarContas();
void carregarContas();
float atualizaSaldoTotal(Conta conta);
Conta* buscarContaPorNumero(int numero);
void sacar(Conta *conta, float valor);
void depositar(Conta *conta, float valor);
void transferir(Conta *conta_origem, Conta *conta_destino, float valor);
void limparTela();

static Conta contas[50];
static int contador_contas = 0;

int main() {
    setlocale(LC_ALL, "Portuguese");
    carregarContas();
    menu();
    return 0;
}

void menu() {
    int opcao = 0;
    while (1) {
        limparTela();

        // Exibe o menu com cores
        printf(BG_WHITE MAGENTA);
        printf("============================================\n");
        printf("================ TERMINAL ==================\n");
        printf("=============== BANCO BARÃO ================\n");
        printf("============================================\n");
        printf("================= /\\_/\\ ====================\n");
        printf("================ ( o.o ) ===================\n"); 
		printf("================= > $ < ====================\n");
		printf("============================================\n");

        printf(RESET);

        printf(GREEN "\n   ==== Selecione uma opção no menu: ====\n\n" RESET);
        printf(YELLOW "1 - Criar Conta\n" RESET);
        printf(YELLOW "2 - Efetuar Saque\n" RESET);
        printf(YELLOW "3 - Efetuar Depósito\n" RESET);
        printf(YELLOW "4 - Efetuar Transferência\n" RESET);
        printf(YELLOW "5 - Listar Contas\n" RESET);
        printf(YELLOW "6 - Fechar Conta\n" RESET);
        printf(YELLOW "7 - Limpar tela\n" RESET);
        printf(YELLOW "8 - Sair do programa\n\n" RESET);

        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                criarConta();
                break;
            case 2:
                efetuarSaque();
                break;
            case 3:
                efetuarDeposito();
                break;
            case 4:
                efetuarTransferencia();
                break;
            case 5:
                listarContas();
                break;
            case 6:
                fecharConta();
                break;
            case 7:
                limparTela();
                break;
            case 8:
                salvarContas();
                printf(RED "Até a próxima!\n" RESET);
                Sleep(2000);
                exit(0);
            default:
                printf(RED "Opção inválida.\n" RESET);
                Sleep(2000);
                break;
        }
    }
}

void limparTela() {
    system("cls");
}

void criarConta() {
    if (contador_contas >= 50) {
        printf(RED "Limite de contas atingido.\n" RESET);
        Sleep(2000);
        return;
    }

    Conta novaConta;
    novaConta.numero = contador_contas + 1;

    printf("Digite o nome do cliente: ");
    fgets(novaConta.cliente.nome, 50, stdin);
    novaConta.cliente.nome[strcspn(novaConta.cliente.nome, "\n")] = 0;

    printf("Digite o email do cliente: ");
    fgets(novaConta.cliente.email, 50, stdin);
    novaConta.cliente.email[strcspn(novaConta.cliente.email, "\n")] = 0;

    printf("Digite o CPF do cliente: ");
    fgets(novaConta.cliente.cpf, 20, stdin);
    novaConta.cliente.cpf[strcspn(novaConta.cliente.cpf, "\n")] = 0;

    printf("Digite a data de nascimento (dd/mm/aaaa): ");
    fgets(novaConta.cliente.dataNascimento, 20, stdin);
    novaConta.cliente.dataNascimento[strcspn(novaConta.cliente.dataNascimento, "\n")] = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(novaConta.cliente.dataCadastro, 20, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    novaConta.saldo = 0.0;
    novaConta.limite = 0.0;
    novaConta.saldoTotal = atualizaSaldoTotal(novaConta);

    contas[contador_contas] = novaConta;
    contador_contas++;

    salvarContas();
    printf(GREEN "Conta criada com sucesso!\n" RESET);
    Sleep(2000);
}

float atualizaSaldoTotal(Conta conta) {
    return conta.saldo + conta.limite;
}

void listarContas() {
    if (contador_contas == 0) {
        printf(RED "Nenhuma conta cadastrada.\n" RESET);
        Sleep(2000);
        return;
    }

    for (int i = 0; i < contador_contas; i++) {
        infoConta(contas[i]);
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar();
}

void infoConta(Conta conta) {
    printf("Número da conta: %d\n", conta.numero);
    printf("Nome do cliente: %s\n", conta.cliente.nome);
    printf("Email do cliente: %s\n", conta.cliente.email);
    printf("CPF do cliente: %s\n", conta.cliente.cpf);
    printf("Data de nascimento: %s\n", conta.cliente.dataNascimento);
    printf("Data de cadastro: %s\n", conta.cliente.dataCadastro);
    printf("Saldo: %.2f\n", conta.saldo);
    printf("Limite: %.2f\n", conta.limite);
    printf("Saldo total: %.2f\n", conta.saldoTotal);
    printf("-----------------------------\n");
}

Conta* buscarContaPorNumero(int numero) {
    for (int i = 0; i < contador_contas; i++) {
        if (contas[i].numero == numero) {
            return &contas[i];
        }
    }
    return NULL;
}

void efetuarSaque() {
    int numero;
    float valor;
    printf("Digite o número da conta: ");
    scanf("%d", &numero);
    Conta* conta = buscarContaPorNumero(numero);

    if (conta == NULL) {
        printf(RED "Conta não encontrada.\n" RESET);
        Sleep(2000);
        return;
    }

    printf("Digite o valor a ser sacado: ");
    scanf("%f", &valor);
    sacar(conta, valor);
}

void sacar(Conta *conta, float valor) {
    if (valor <= 0) {
        printf(RED "Valor inválido para saque.\n" RESET);
        Sleep(2000);
        return;
    }

    if (valor > conta->saldo + conta->limite) {
        printf(RED "Saldo insuficiente.\n" RESET);
        Sleep(2000);
        return;
    }

    conta->saldo -= valor;
    conta->saldoTotal = atualizaSaldoTotal(*conta);
    salvarContas();
    printf(GREEN "Saque realizado com sucesso! Novo saldo: %.2f\n" RESET, conta->saldo);
    Sleep(2000);
}

void efetuarDeposito() {
    int numero;
    float valor;
    printf("Digite o número da conta: ");
    scanf("%d", &numero);
    Conta* conta = buscarContaPorNumero(numero);

    if (conta == NULL) {
        printf(RED "Conta não encontrada.\n" RESET);
        Sleep(2000);
        return;
    }

    printf("Digite o valor a ser depositado: ");
    scanf("%f", &valor);
    depositar(conta, valor);
}

void depositar(Conta *conta, float valor) {
    if (valor <= 0) {
        printf(RED "Valor inválido para depósito.\n" RESET);
        Sleep(2000);
        return;
    }

    conta->saldo += valor;
    conta->saldoTotal = atualizaSaldoTotal(*conta);
    salvarContas();
    printf(GREEN "Depósito realizado com sucesso! Novo saldo: %.2f\n" RESET, conta->saldo);
    Sleep(2000);
}

void efetuarTransferencia() {
    int numero_origem, numero_destino;
    float valor;
    
    printf("Digite o número da conta de origem: ");
    scanf("%d", &numero_origem);
    Conta* conta_origem = buscarContaPorNumero(numero_origem);

    if (conta_origem == NULL) {
        printf(RED "Conta de origem não encontrada.\n" RESET);
        Sleep(2000);
        return;
    }

    printf("Digite o número da conta de destino: ");
    scanf("%d", &numero_destino);
    Conta* conta_destino = buscarContaPorNumero(numero_destino);

    if (conta_destino == NULL) {
        printf(RED "Conta de destino não encontrada.\n" RESET);
        Sleep(2000);
        return;
    }

    printf("Digite o valor a ser transferido: ");
    scanf("%f", &valor);
    transferir(conta_origem, conta_destino, valor);
}

void transferir(Conta *conta_origem, Conta *conta_destino, float valor) {
    if (valor <= 0) {
        printf(RED "Valor inválido para transferência.\n" RESET);
        Sleep(2000);
        return;
    }

    if (valor > conta_origem->saldo + conta_origem->limite) {
        printf(RED "Saldo insuficiente para transferência.\n" RESET);
        Sleep(2000);
        return;
    }

    conta_origem->saldo -= valor;
    conta_destino->saldo += valor;
    conta_origem->saldoTotal = atualizaSaldoTotal(*conta_origem);
    conta_destino->saldoTotal = atualizaSaldoTotal(*conta_destino);

    salvarContas();
    printf(GREEN "Transferência realizada com sucesso!\n" RESET);
    Sleep(2000);
}

void fecharConta() {
    int numero;
    printf("Digite o número da conta a ser fechada: ");
    scanf("%d", &numero);

    for (int i = 0; i < contador_contas; i++) {
        if (contas[i].numero == numero) {
            for (int j = i; j < contador_contas - 1; j++) {
                contas[j] = contas[j + 1];
            }
            contador_contas--;
            salvarContas();
            printf(GREEN "Conta fechada com sucesso!\n" RESET);
            Sleep(2000);
            return;
        }
    }

    printf(RED "Conta não encontrada.\n" RESET);
    Sleep(2000);
}

// Funções para salvar e carregar dados do banco
void salvarContas() {
    FILE *arquivo = fopen("contas.dat", "wb");
    if (arquivo == NULL) {
        printf(RED "Erro ao salvar os dados.\n" RESET);
        return;
    }
    fwrite(&contador_contas, sizeof(int), 1, arquivo);
    fwrite(contas, sizeof(Conta), contador_contas, arquivo);
    fclose(arquivo);
}

void carregarContas() {
    FILE *arquivo = fopen("contas.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo ainda não existe
    }
    fread(&contador_contas, sizeof(int), 1, arquivo);
    fread(contas, sizeof(Conta), contador_contas, arquivo);
    fclose(arquivo);
}

