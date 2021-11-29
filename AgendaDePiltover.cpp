#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct reg
{
	char nome[20];
	char tel[10];
	char email[20];
	char endereco[30];
	char sintoma[20];
	char idade[10];
	char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void gerararqtxt(FILE *);
void excluir(FILE *);

int main()
{
	setlocale(LC_ALL,"");

	int op;
	FILE *arq;
	
	if ((arq = fopen("dados.dat", "rb+")) == NULL)
	{
		if ((arq = fopen("dados.dat", "wb+")) == NULL)
		{
			printf("\nFalha ao abrir o arquivo!\n");
			system("pause");
		}
	}

	do
	{
		system("CLS");
		printf("\n========== Clinica Piltover ==========");
		printf("\n1.Cadastrar Paciente\n");
		printf("\n2.Consultar Paciente\n");
		printf("\n3.Clientes Consultados\n");
		printf("\n4.Gerar arquivo\n");
		printf("\n5.Sair\n");
		printf("======== Pacientes: %d =======\n", tamanho(arq));
		printf("Opcao: "); scanf("%d", &op);
		
		switch (op)
		{
		case 1: 
			cadastrar(arq);
			break;
		case 2: 
			consultar(arq);
			break;
		case 3: 
			excluir(arq);
			break;			
		case 4: 
			gerararqtxt(arq);
			break;
		case 5: fclose(arq);
		}
	} while (op != 5);
}

void cadastrar(FILE *arq)
{ 
	system("CLS");
	
	reg contato; 
	char confirma;
	contato.status = ' ';

	fflush(stdin);
	printf("Cadastrando novo paciente!!\n");
	printf("\nNumero do paciente: %d\n", tamanho(arq) +1);
	printf("Nome............: ");
	gets(contato.nome);
	printf("Idade............: ");
	gets(contato.idade);
	printf("Telefone.........: ");
	gets(contato.tel);
	printf("e-mail...........: ");
	gets(contato.email);
	printf("Endereço........: ");
	gets(contato.endereco);
	printf("Sintomas............: ");
	gets(contato.sintoma);
	printf("\nConfirma <s/n>: "); scanf("%c", &confirma);	

	if (toupper(confirma) == 'S')
	{
		printf("\n gravando ....\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&contato, sizeof(reg), 1, arq);
	}
}


void consultar(FILE *arq)
{	
	system("CLS");
	reg contato;
	int nr;
	printf("\nConsultar pelo codigo!\n");
	printf("\nInforme o Codigo do paciente...: "); scanf("%d",&nr);
	
	if((nr <= tamanho(arq))&&(nr>0))
	{
		fseek(arq,(nr-1)*sizeof(reg),SEEK_SET);
		fread(&contato,sizeof(reg),1,arq);
		if (contato.status == ' ')
		{
			printf("\nNome...: %s",contato.nome);
			printf("\nIdade...: %s",contato.idade);
			printf("\nTelefone...: %s",contato.tel);
			printf("\nE-mail...: %s",contato.email);
			printf("\nEndereço...: %s",contato.endereco);
			printf("\nSintomas...: %s\n",contato.sintoma);
		}		
		
		else
		{
			printf("\nCliente já consultado!!\n");
		}
	}
	
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

void gerararqtxt (FILE *arq)
{
	system("CLS");
	
	char nomearq[20];
	printf("Nome do arquivo texto: "); scanf("%s", nomearq);	
	strcat(nomearq, ".txt");
	
	FILE *arqtxt = fopen(nomearq, "w");
	
	if(!arqtxt)
	{
		printf("\nNão foi encontrado o arquivo!!");
		system("pause");
	}
	
	fprintf(arqtxt, "Nome                Idade    Telefone     E-mail              Endereço            Sintomas           Status");
	fprintf(arqtxt, "\n===============================================================================================================\n");

	int nr;
	reg contato;
	
	for(nr = 0; nr < tamanho(arq);nr++)
	{		
		fseek(arq,nr * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1,arq);
		fprintf(arqtxt, "%-20s %-9s %-13s %-20s %-20s %-10s %-20s\n", contato.nome, contato.idade, contato.tel, contato.email, contato.endereco, contato.sintoma,contato.status);
	}
	fprintf(arqtxt, "\n==============================================================================================================\n");
	fclose(arqtxt);
}

void excluir(FILE *arq)
{
	system("CLS");
	
	reg contato;
	char confirma;
	int nr;

	printf("\nInforme o codigo do paciente que foi consultado: \n"); scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		if (contato.status == ' ')
		{
			printf("\nNome......: %s", contato.nome);
			printf("\nIdade..: %s", contato.idade);
			printf("\nTelefone..: %s", contato.tel);
			printf("\nE-mail....: %s", contato.email);
			printf("\nEndereço....:%s", contato.endereco);					
			printf("\nConfirmar paciente: <s/n>\n");
			getchar();
			scanf("%c", &confirma);

			if (toupper(confirma) == 'S')
			{
				printf("\nPaciente Consultado!!\n\n");
				fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
				contato.status = '*';
				fwrite(&contato, sizeof(reg), 1, arq);
			}
		}
	else
		printf("Paciente inexistente! \n");
 }
	else
	{
		printf("\nNumero do paciente invalido!\n");
	}
	system("pause");
}

int tamanho(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(reg);
}
