#ifndef NEMSIO2NC_GFSNC
#define NEMSIO2NC_GFSNC
#include <string>

namespace nems2nc {
 class gfsnc {
   public:
     std::string filename;
     int ncid;

     int create(std::string filenamein);

 };
}

#endif // NEMSIO2NC_GFSNC
