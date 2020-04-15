#ifndef NEMSIO2NC_NEMSIO
#define NEMSIO2NC_NEMSIO
#include <string>

namespace nems2nc {
 class nemsio {
   public:
     std::string filename;

     int open(std::string filenamein);

 };
}

#endif // NEMSIO2NC_NEMSIO
