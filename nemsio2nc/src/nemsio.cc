#include <iostream>
#include <string>
#include <cstdlib>
#include "nemsio.h"
#include "nemsio_interface.h"

namespace nems2nc {

 int nemsio::open(std::string filenamein) {
   std::cout << "Opening " << filenamein << " for reading " << std::endl;
   filename = filenamein;
   // convert filename to fname for use in F90
   char fname[filename.length()];
   int i;
   int fname_len = filename.length();
   for (i = 0; i < sizeof(fname); i++) { 
      fname[i] = filename[i];
   }
   // open file using NEMSIO F90 library
   nemsio_open_f90(fname_len, fname);
   return 0;
 }

}
