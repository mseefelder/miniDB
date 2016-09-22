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

   selectAttribute("IES.bin", regFormat, regSize, 2360, 1);
   selectAttribute("IES.bin", regFormat, regSize, 2360, 2);
   selectAttribute("IES.bin", regFormat, regSize, 2360, 3);

   selectKey("IES.bin", regFormat, regSize, 0, 3);
   selectKey("IES.bin", regFormat, regSize, 1, 3);
   selectKey("IES.bin", regFormat, regSize, 2, 3);
   selectKey("IES.bin", regFormat, regSize, 3, 3);
   selectKey("IES.bin", regFormat, regSize, 4, 3);

   selectKeyByDenseIndex("IES.bin", regFormat, regSize, "0", 3);
   selectKeyByDenseIndex("IES.bin", regFormat, regSize, "1", 3);
   selectKeyByDenseIndex("IES.bin", regFormat, regSize, "2", 3);
   selectKeyByDenseIndex("IES.bin", regFormat, regSize, "3", 3);
   selectKeyByDenseIndex("IES.bin", regFormat, regSize, "4", 3);

   return 0;
}
