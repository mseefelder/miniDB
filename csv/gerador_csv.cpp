#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <cmath>
#include<iostream>
#include<string.h>
#include<cstdio>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <math.h> 

#include <tr1/functional>



int cep(int n){

int a=0;
a=(19333*n)%100000;

if (a<0){
a=-a;
}
return a;

}

int digitos(int n){

int contaDigitos = 0;
  
       if (n == 0) contaDigitos = 1;
       else
           while (n != 0)
           {
               contaDigitos = contaDigitos + 1;
               n = n/ 10;
           }
       
	 
       return contaDigitos;

}

char* gerar_palavra(){

int str_len;
char letras[27] = "abcdefghijklmnopqrstuvwxyz";
char *novastr;

//srand(time(NULL));

// tamanho da string
str_len = (rand() % 10 );

// se for menor que cinco, configura para ser maior que cinco
str_len += str_len < 5 ? 5 : 0;


// aloca memoria
novastr = (char*)malloc((str_len + 1)* sizeof(char));


for ( int i = 0; i < str_len; i++ ) {
    novastr[i] = letras[ rand() % strlen(letras) ];
    novastr[i + 1] = 0x0;
}

 return novastr; 
}




void r1(int n){
//CPF - CEP

int cpf=0;
int randNumber;	

char arquivo_csv[n];
sprintf(arquivo_csv,"r1.csv");
FILE *arq_csv=fopen(arquivo_csv,"w");


srand(time(NULL));

	for (int i=0; i<n; i++){
		randNumber = (rand()% 89)+10;// vai variar entre 10 e 99
		cpf= (randNumber*n)+i;
		
		fprintf(arq_csv,"%d,%d\r\n", cpf, cep(cpf));
	
  		//std::cout << cpf <<","<< cep(cpf)<< std::endl; 
	}

fclose(arq_csv);
}


void r2(int n){
// CPF - Nome - Sobrenome 

int cpf=0;
int number[10];
int randNumber;

char arquivo_csv[n];
sprintf(arquivo_csv,"r2.csv");
FILE *arq_csv=fopen(arquivo_csv,"w");
    				
 //std::size_t int_hash;
 //std::tr1::hash<int> hash_fn;

srand(time(NULL));

	
	for (int i=0; i<n; i++){
		randNumber = (rand()% 89)+10; // vai variar entre 10 e 99
		cpf= (randNumber*n)+i;

		fprintf(arq_csv,"%d, %s,%s\r\n", cpf,gerar_palavra(), gerar_palavra());
   		
  		//std::cout << cpf <<","<< gerar_palavra() << "," << gerar_palavra()<< std::endl; 
	}
fclose(arq_csv);
}

void r3(int n, int n_cpf){
// id - CPF - Preço - CEP

char arquivo_csv[n];
sprintf(arquivo_csv,"r3.csv");
FILE *arq_csv=fopen(arquivo_csv,"w");

int id=0;
double preco=0;
int cpf=0; // tem que ler no arquivo

int number[10];
int randNumber;
int digitos_cpf= digitos(n_cpf)+1;
 
				


srand(time(NULL));


	for (int i=0; i<n; i++){
		randNumber = (rand()% 89)+10;// vai variar entre 10 e 99
		id= (randNumber*n)+i;
		preco = (rand()% 250);

		
		cpf= fmod(rand(),(pow(10, digitos_cpf) - pow(10,(digitos_cpf-1))-1)) + pow(10,(digitos_cpf-1)) ;
				
		fprintf(arq_csv,"%d,%d,%f,%d\r\n", id, cpf,preco, cep(cpf));
		//std::cout << id << "," << cpf << ","<< preco << ","<< cep(cpf) << std::endl; 
	}
fclose(arq_csv);

}



int main()
{


r1(10); // número de registros da relação1 gerados
r2(10); // número de registros da relação2 gerados
r3(1000,10); // número de registros da relação3 gerados , número de registros gerados na r1


return 0;

}



