/*! \file vulneravel.c
    \brief Codigo fonte de demonstracao de buffer overflow - substituindo endereco de funcao local
	\author Anderson dos Santos Silva
	\version 1.0
*/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <process.h>

// verifica se o tipo boolean esta definido, caso nao esteja define
#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


#if _DEBUG // verifica se esta rodando em modo debug
#define INTERNET_BANK_FUNCTION_ADDRESS		"\\x2D\\x10\\x40\\x00"
#else // rodando em modo release
#define INTERNET_BANK_FUNCTION_ADDRESS		"\\xA0\\x10\\x40\\x00"
#endif

// mascaras de bits para as opcoes
#define OPTIONS_INVALID						0x00	//00000000
#define OPTIONS_RUN_NORMAL_MODE				0x01	//00000001
#define OPTIONS_SHOW_FUCTION_ADDRESS		0x02	//00000010
#define OPTIONS_SHOW_DEBUG_MESSAGES			0x04	//00000100
#define OPTIONS_SHOW_STACK_REGISTERS		0x08	//00001000
#define OPTIONS_SHOW_SHELL_CODE				0x10	//00010000
#define OPTIONS_RUN_HACKER_MODE				0x20	//00100000

#define RUN_IN_NORMAL_MODE					0x01
#define RUN_IN_HACKER_MODE					0x02

bool	g_bShowStack;
bool	g_bShowDebug;
int		g_iRunMode;		

/*! \fn void Debug(char *Msg)
    \brief Funcao para exibir mensagem de debug na tela.
    \param Msg Ponteiro para char contendo string para ser exibida na tela.
    \return void - Sem retorno.
*/
void Debug(char* Msg)
{
	// verifica se deve exibir a mensagem de debug na tela
	if(g_bShowDebug)
	{
		printf("\n====================== DEBUG ======================");
		printf("\n => %s", Msg);
		printf("\n===================================================");
	}
}

/*! \fn void ExibeStack()
    \brief Funcao para exibir stack - registradores.
    \return void - Sem retorno.
*/
void ExibeStack()
{
	// declaracao de variaveis para armazenar o conteudo dos registradores
	unsigned int	uiEAX;
	unsigned int	uiEBX;
	unsigned int	uiECX;
	unsigned int	uiEDX;
	unsigned int	uiESI;
	unsigned int	uiEDI;
	unsigned int	uiESP;
	unsigned int	uiEBP;

	// verifica se deve exibir as informacoes da pilha
	if(g_bShowStack)
	{
		// armazena o conteudo dos registradores nas variaveis
		__asm mov uiEAX, eax
		__asm mov uiEBX, ebx
		__asm mov uiECX, ecx
		__asm mov uiEDX, edx
		__asm mov uiESI, esi
		__asm mov uiEDI, edi
		__asm mov uiESP, esp
		__asm mov uiEBP, ebp

		// formata e exibe as informacoes na tela
		printf("\n====================== STACK TRACE =========================");
		printf("\nEAX:%08X\tEBX:%08X\tECX:%08X\tEDX:%08X"
			   "\nESI:%08X\tEDI:%08X\tESP:%08X\tEBP:%08X", 
			   uiEAX,uiEBX,uiECX,uiEDX,uiESI,uiEDI,uiESP,uiEBP);
		printf("\n============================================================");
	}
}

/*! \fn void InternetBank()
    \brief Funcao simular internet bank.
    \return void - Sem retorno.
*/
void InternetBank()
{
	// exibe as informacoes da pilha
	ExibeStack();

	// mensagem de debug notificando acesso permitido ao internet bank
	Debug("acesso permitido ao internet bank");

	// internet bank
	printf("\n======== INTERNET BANK ========");
	printf("\n1 - Saque");
	printf("\n2 - Extrato");
	printf("\n3 - Poupanca");
	printf("\n4 - Emprestimos");
	printf("\n5 - Sair");
	printf("\n===============================");
	getch();


}

/*! \fn int Logon(char* usuario, char* senha)
    \brief Funcao para autenticar usuario.
	\param usuario Ponteiro para char contendo o usuario.
	\param senha Ponteiro para char contendo a senha.
    \return int - Retorna 1 para usuario autenticado com sucesso ou 0 caso contrario.
*/
int Logon(char* usuario, char* senha)
{
	// declaracao de variaveis locais
	char szUsuario[10];
	char szSenha[10];
	int	 iRet=0;

	// exibe a pilha
	ExibeStack();

	// copia o usuario passado como parametro para a variavel local
	Debug("copiando usuario para a variavel local");
	strcpy(szUsuario, usuario);

	
	// copia a senha passada como parametro para a variavel local
	Debug("copiando senha para a variavel local");
	strcpy(szSenha, senha);

	// valida o usuario e senha
	Debug("validando usuario e senha digitados");
	iRet = ((!stricmp(szUsuario, "PED")) &&
		   (!stricmp(szSenha,   "UOLDIVEO")));

	// exibe informacao debug notificando se o usuario foi ou nao autenticado
	Debug((iRet == 1) ? 
		"usuario autenticado com sucesso (retorno = 1)" :
	    "usuario ou senha invalidos (retorno = 0)");

	// exibe a pilha
	ExibeStack();

	// retorna se autenticou com sucesso ou erro
	return(iRet);
}

/*! \fn void ExibeShellCode()
    \brief Funcao para exibir o shellcode utilizado para o ataque.
    \return void - Sem retorno.
*/
void ExibeShellCode()
{
	ExibeStack();
	printf("\n============ SHELLCODE ==============\n");
	printf("AAAAAAAAAAAAAAAAAAAAAAAABBBB%s",INTERNET_BANK_FUNCTION_ADDRESS);
	printf("\n======================================");
	ExibeStack();
}

/*! \fn void ExibeParametros()
    \brief Funcao para exibir os parametros validos para o programa.
    \return void - Sem retorno.
*/
void ExibeParametros()
{
	// limpa a tela
	system("cls");

	// exibe os parametros
	printf("vulneravel - Demonstracao 1 de software vulneravel a buffer overflow");
	printf("\n(C) Copyright 2012 Anderson");
	printf("\n\t-n: Executar no modo normal");
	printf("\n\t-h: Executar no modo H4CK3R");
	printf("\n\t-f: Exibir os enderecos das funcoes");
	printf("\n\t-d: Exibir informacoes debug");
	printf("\n\t-s: Exibir stack");
	printf("\n\t-e: Exibir shellcode para explorar vulnerabilidade");


}

/*! \fn int VerificaParametrosEntrada(int NumPar, char* Param[])
    \brief Funcao para exibir os parametros validos para o programa.
	\param NumPar Inteiro contendo o numero de parametros.
	\param Param[] Ponteiro para vetor de char contendo os parametros.
    \return int - Retorna mascara de bits contendo os parametros validos.
*/
int VerificaParametrosEntrada(int NumPar, char* Param[])
{
	int iRet=0;
	int	iCount;

	// valida os parametros
	for(iCount=1; iCount<=(NumPar-1); iCount++)
	{
		if(!stricmp(Param[iCount],"-n"))		iRet |= OPTIONS_RUN_NORMAL_MODE;
		else if(!stricmp(Param[iCount],"-f"))	iRet |= OPTIONS_SHOW_FUCTION_ADDRESS;
		else if(!stricmp(Param[iCount],"-d"))	iRet |= OPTIONS_SHOW_DEBUG_MESSAGES;
		else if(!stricmp(Param[iCount],"-s"))	iRet |= OPTIONS_SHOW_STACK_REGISTERS;
		else if(!stricmp(Param[iCount],"-e"))	iRet |= OPTIONS_SHOW_SHELL_CODE;
		else if(!stricmp(Param[iCount],"-h"))	iRet |= OPTIONS_RUN_HACKER_MODE;
		
	}

	// retorna mascara de bits de parametros
	return(iRet);
}

/*! \fn void ExibeEnderecosDeFuncoes()
    \brief Funcao para exibir os ponteiros das funcoes para montar o shellcode.
    \return void - Sem retorno.
*/
void ExibeEnderecosDeFuncoes()
{
	// exibe a pilha
	ExibeStack();

	// exibe os ponteiros de funcao
	printf("\n========= ENDERECOS DE FUNCOES =========");
	printf("\nVerificaParametrosEntrada:\t%p",VerificaParametrosEntrada);
	printf("\nExibeParametros:\t\t%p",ExibeParametros);
	printf("\nExibeEnderecosDeFuncoes:\t%p",ExibeEnderecosDeFuncoes);
	printf("\nLogon:\t\t\t\t%p",Logon);
	printf("\nInternetBank:\t\t\t0x%p",InternetBank);
	printf("\n========================================");

}

int main(int argc, char *argv[])
{
	// definicao de variaveis
	char	szVar1[100];
	char	szVar2[100];
	char	szTmp[2048];
	//char	szshellcode[]= {"AAAAAAAAAAAAAAAAAAABBBB\\x90\\x18\\x41\\x00"};
	char	szshellcode[] = { "AAAAAAAAAAAAAAAAAAABBBB\\x90\\x18\\x41\\x00" };
	int		iParam;
#if _DEBUG
	// 00411890	
	char shellcode[] = { "AAAAAAAAAAAAAAAAAAAAAAAABBBB\x90\x18\x41\x00" };	
#else
	char shellcode[] = { "AAAAAAAAAAAAAAAAAAAAAAAABBBB\xA0\x10\x40\x00" };
#endif

	// incializa variavel que define o modo que ira executar
	g_iRunMode = 0;

	// inicializa variaveis globais de controle para exibir stack e debug
	g_bShowDebug = g_bShowStack = false;

	g_bShowStack = true;

	// verifica se executou o programa sem parametros
	if(argc == 1)
	{
		// exibe os parametros validos para o programa
		ExibeParametros();
		getch();
		return(0);
	}
	else
	{
		// valida os parametros de entrada
		iParam = VerificaParametrosEntrada(argc, argv);

		// verifica se os parametros de entrada sao validos
		if(iParam == OPTIONS_INVALID)
		{
			// se nao forem exibe mensagem de erro e finaliza o programa
			printf("\nParametro invalido, por favor, verifique o modo correto de utilizacao.");
			return(0);
		}
		else // se os parametros de entrada sao validos!
		{
			// valida se deve exibir os ponteiros de funcao
			if(iParam & OPTIONS_SHOW_FUCTION_ADDRESS)
				ExibeEnderecosDeFuncoes();

			// valida se deve exibir o stack
			if(iParam & OPTIONS_SHOW_DEBUG_MESSAGES)
				g_bShowDebug = true;

			// valida se deve exibir o stack
			if(iParam & OPTIONS_SHOW_STACK_REGISTERS)
				g_bShowStack = true;

			// valida se deve exibir o shellcode
			if(iParam & OPTIONS_SHOW_SHELL_CODE)
				ExibeShellCode();

			// valida se deve executar em modo normal
			if(iParam & OPTIONS_RUN_NORMAL_MODE)
				g_iRunMode = RUN_IN_NORMAL_MODE;

			// valida se deve executar em modo hacker
			if(iParam & OPTIONS_RUN_HACKER_MODE)
				g_iRunMode = RUN_IN_HACKER_MODE;
			
		}	
	}

	// valida se deve executar em algum modo
	if(g_iRunMode)
	{
		Debug((g_iRunMode == RUN_IN_HACKER_MODE) ? 
			  "iniciando programa de demonstracao em modo H4CK3R":
		      "iniciando programa de demonstracao em modo normal");

		// exibe informacoes de autenticacao
		printf("\n\n===== AUTENTICACAO DO INTERNET BANK TABAJARA =====");
		printf("\nUsuario: ");		
		// preenche variavel szVar1 (usuario) com o valor digitado pelo usuario 
		gets(szVar1);


		printf("\nSenha: ");
		// preenche variavel szVar2 (senha) com o valor digitado pelo usuario 
		gets(szVar2);

		sprintf(szTmp,
			    "Usuario = %s | Senha: %s - chamando a funcao (Logon) para autenticacao ",
			    szVar1,
				(g_iRunMode == RUN_IN_HACKER_MODE) ? szshellcode : szVar2);
		Debug(szTmp);

		// cria o shellcode...
		/*
		memset(szTmp, 0, sizeof(szTmp));
		sprintf(szTmp, "%p",InternetBank);
		shellcode[34]=szTmp[6];
		shellcode[35]=szTmp[7];
		shellcode[32]=szTmp[4];
		shellcode[33]=szTmp[5];
		shellcode[30]=szTmp[2];
		shellcode[31]=szTmp[3];
		shellcode[28]=szTmp[0];
		shellcode[29]=szTmp[1];
		*/
		

		// realiza processo de autenticacao no logon
		if(Logon(szVar1,(g_iRunMode == RUN_IN_HACKER_MODE) ? shellcode : szVar2))
		{
			Debug("processo de autenticacao realizado com sucesso");
			Debug("acessando o internet bank");
			// se o logon foi realizado com sucesso acessa o internet bank
			InternetBank();
		}
		else
		{
			Debug("processo de autenticacao retornou erro para as informacoes digitadas");
			// se ocorreu falha na autenticacao do usuario exibe acesso negado
			printf("\nACESSO NEGADO!");
		}
	}

	// aguarda uma tecla ser pressionada
	getch();

	return(0);
}