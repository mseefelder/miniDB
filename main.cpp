#include <iostream>
#include <string>
#include "loader.h"

using namespace std;

int main()
{
   short regFormat[3] = {4, 256, 32};
   string schemaName = "IES";
   short regSize = 292;
   loadFile("CSVs_de_Teste/lista_ies.csv","IES.bin", schemaName, regFormat, 3, 1,regSize);

   selectAttribute("IES.bin", regFormat, regSize, 2368, 1);
   selectAttribute("IES.bin", regFormat, regSize, 2368, 2);
   selectAttribute("IES.bin", regFormat, regSize, 2368, 3);

   selectKey("IES.bin", regFormat, regSize, 2360, 3);

   //selectKeyByDenseIndex("IES.bin", regFormat, regSize, "2", 3);

   return 0;
}
