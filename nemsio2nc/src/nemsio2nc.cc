/*
 * nemsio2nc
 * convert NCEP NEMSIO formatted file
 * to FV3GFS NCIO netCDF formatted file
 * supports GFSv15 to GFSv16 conversion
 */
#include "nems2nc.h"

 int main(int argc,  char ** argv) {
  int status;
  nems2nc::Convert nemsio2nc;
  status = nemsio2nc.init(argc, argv);
  status = nemsio2nc.run();
  status = nemsio2nc.cleanup();
  return status;
 }
