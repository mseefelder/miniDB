#include <iostream>
#include "loader.h"

using namespace std;

int main()
{
   short regFormat[3] = {4, 256, 32};
   short regSize = 292;
   loadFile("CSVs_de_Teste/lista_ies.csv","test.bin", regFormat, 3);
   selectAttribute("test.bin", regFormat, regSize, 2369, 1);
   selectAttribute("test.bin", regFormat, regSize, 2369, 2);
   selectAttribute("test.bin", regFormat, regSize, 2369, 3);
   return 0;
}
