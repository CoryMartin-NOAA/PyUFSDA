#ifndef NEMSIO2NC_VARS
#define NEMSIO2NC_VARS
#include <string>
#include <map>

namespace nems2nc {
  // map of variable names to change between NEMSIO and netCDF
  // these are for only when the input name != output name
  const std::map<std::string, std::string> varchanges = {
    { "hgt", "hgtsfc" },
    { "pres", "pressfc" },
  };
  const std::map<std::string, std::string> varlongnames = {
    { "hgtsfc", "surface geopotential height" },
    { "pressfc", "surface pressure"},
  };
}

#endif // NEMSIO2NC_VARS
