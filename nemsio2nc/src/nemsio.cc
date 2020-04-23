#include <iostream>
#include <iomanip>
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
   // get header info
   nemsio_get_header_f90( idate, fhour, nx, ny, nz, nrec);
   std::cout << "Input NEMSIO file:" << std::endl;
   std::cout << "File info: " << std::endl;
   // print out time M/D/YYYY HH:MM UTC
   std::cout << "Initial time: " << idate[1] << "/" << idate[2] << "/" << idate[0] << " ";
   std::cout << std::setw(2) << std::setfill('0') << idate[3];
   std::cout << ":";
   std::cout << std::setw(2) << std::setfill('0') << idate[4];
   std::cout << " UTC" << std::endl;
   // print out dimension info
   std::cout << "Forecast hour = " << fhour << std::endl;
   std::cout << "nx = " << nx << std::endl;
   std::cout << "ny = " << ny << std::endl;
   std::cout << "nz = " << nz << std::endl;
   // get ak,bk,ntrac
   ak = new double[nz+1];
   bk = new double[nz+1];
   phalf = new double[nz+1];
   pfull = new double[nz];
   lat = new double[nx*ny];
   lon = new double[nx*ny];
   nemsio_get_akbk_latlon_f90(nx, ny, nz, ak, bk, lat, lon, pfull, phalf, ntrac);
   // get list of records
   reclev = new int[nrec];
   char recname_tmp[10*nrec], reclevtyp_tmp[10*nrec];
   nemsio_get_recinfo_f90(nrec, recname_tmp, reclevtyp_tmp, reclev);
   for (i = 0; i < nrec; i++) {
     std::string tmpstr="";
     for (int j = 0; j<10; j++) {
       int k = i*10 + j;
       tmpstr = tmpstr + recname_tmp[k];
     }
     tmpstr.erase(tmpstr.find_last_not_of(" \n\r\t")+1);
     recname.push_back(tmpstr);
     std::cout << tmpstr << " " << tmpstr.size();
     std::cout << std::endl;
   }
   return 0;
 }

}
