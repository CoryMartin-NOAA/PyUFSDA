#ifndef NEMSIO2NC_GFSNC
#define NEMSIO2NC_GFSNC
#include <string>
#include "nemsio.h"

namespace nems2nc {
 class gfsnc {
   public:
     std::string filename;
     int ncid, xdimid, ydimid, zdimid, zidimid, tdimid;

     int create(std::string filenamein, nems2nc::nemsio nemsfile);

     int def_vars(nems2nc::nemsio nemsfile);

 };
}

#endif // NEMSIO2NC_GFSNC
