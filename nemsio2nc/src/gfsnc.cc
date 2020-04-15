#include <iostream>
#include <string>
#include <cstdlib>
#include "gfsnc.h"
#include <netcdf.h>

namespace nems2nc {

 int gfsnc::create(std::string filenamein) {
   std::cout << "Opening " << filenamein << " for writing " << std::endl;
   filename = filenamein;
   nc_create(filename.c_str(), NC_CLOBBER|NC_NETCDF4, &ncid);
   return 0;
 }

}
