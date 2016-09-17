#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main()
{

    cout << "1° estudo dirigido COS480" << endl << endl;
    cout << "Teste de carregamento de arquivo de dados" << endl << endl;

    ifstream arquivo("world-universities.csv.txt");

    string linha, sigla;

    unsigned int totalLinhas = 0;

    map< string,int > mapaSiglas;
    map< string,int >::erator it;
    pair< map<string,int>::iterator, bool > siglaExiste;

    while (arquivo.good()) {

        getline(arquivo,linha);
        totalLinhas++;

        //cout << linha << endl;

        sigla = linha.substr(0, 2);

        //map.insert retorna um pair cujo primeiro elemento é uma referência iterator para a posição da chave no map,
        // e o segundo elemento é um boolean que revela a existência ou não da chave

        siglaExiste = mapaSiglas.insert( pair<string,int> (sigla, 1) );
        if (siglaExiste.second == false) siglaExiste.first->second++;
    }

    cout << "Total de linhas do arquivo: " << totalLinhas << endl;
    cout << "Total de universidades brasileiras: " << mapaSiglas.at("BR") << endl << endl;
    cout << "Número de universidades por país:" << endl << endl;

    it = mapaSiglas.begin(); it++;
    while (it != mapaSiglas.end()){
        cout << "Sigla: " << it->first << ", Quantidade: " << it->second << endl;
        it++;
    }

    return 0;
}
