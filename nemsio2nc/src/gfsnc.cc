#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include "gfsnc.h"
#include <netcdf.h>
#include "nemsio.h"
#include "nems2ncvars.h"

namespace nems2nc {

 int nc_err( int errval) {
    if (errval != 0) {
      std::cout << "netCDF Error! code=" << errval << std::endl;
    }
 }

 std::string get_timestr( int idate[5]) {
   std::stringstream buffer;
   buffer << "hours since " << idate[0] << "-";
   buffer << std::setw(2) << std::setfill('0') << idate[1];
   buffer << "-" ;
   buffer << std::setw(2) << std::setfill('0') << idate[2];
   buffer << " " ;
   buffer << std::setw(2) << std::setfill('0') << idate[3];
   buffer << ":" ;
   buffer << std::setw(2) << std::setfill('0') << idate[4];
   buffer << ":00" ;
   return buffer.str();
   }

 int gfsnc::create(std::string filenamein, nems2nc::nemsio nemsio) {
   int errval, varid_tmp;
   int dimids1[1], dimids2[2];
   std::cout << "Opening " << filenamein << " for writing " << std::endl;
   filename = filenamein;
   // create netCDF file for writing
   nc_err(nc_create(filename.c_str(), NC_CLOBBER|NC_NETCDF4, &ncid));
   // define dimensions
   nc_err(nc_def_dim( ncid, "grid_xt", nemsio.nx, &xdimid));
   nc_err(nc_def_dim( ncid, "grid_yt", nemsio.ny, &ydimid));
   nc_err(nc_def_dim( ncid, "pfull", nemsio.nz, &zdimid));
   nc_err(nc_def_dim( ncid, "phalf", nemsio.nz+1, &zidimid));
   nc_err(nc_def_dim( ncid, "time", NC_UNLIMITED, &tdimid));
   // grid_xt
   dimids1[0] = xdimid;
   nc_err(nc_def_var( ncid, "grid_xt", NC_DOUBLE, 1, dimids1, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "cartesian_axis", 1, "X"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 16, "T-cell longitude"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 9, "degrees_E"));
   // grid_yt
   dimids1[0] = ydimid;
   nc_err(nc_def_var( ncid, "grid_yt", NC_DOUBLE, 1, dimids1, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "cartesian_axis", 1, "Y"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 15, "T-cell latitude"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 9, "degrees_N"));
   // lon
   dimids2[0] = ydimid;
   dimids2[1] = xdimid;
   nc_err(nc_def_var( ncid, "lon", NC_DOUBLE, 2, dimids2, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 16, "T-cell longitude"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 9, "degrees_E"));
   // lat
   nc_err(nc_def_var( ncid, "lat", NC_DOUBLE, 2, dimids2, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 15, "T-cell latitude"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 9, "degrees_N"));
   // pfull
   dimids1[0] = zdimid;
   nc_err(nc_def_var( ncid, "pfull", NC_FLOAT, 1, dimids1, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "cartesian_axis", 1, "Z"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 24, "ref full pressure level"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 2, "mb"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "positive", 4, "down"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "edges", 5, "phalf"));
   // phalf
   dimids1[0] = zidimid;
   nc_err(nc_def_var( ncid, "phalf", NC_FLOAT, 1, dimids1, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "cartesian_axis", 1, "Z"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 24, "ref half pressure level"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 2, "mb"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "positive", 4, "down"));
   // time
   std::string timestr;
   timestr = get_timestr(nemsio.idate);
   dimids1[0] = tdimid;
   nc_err(nc_def_var( ncid, "time", NC_DOUBLE, 1, dimids1, &varid_tmp));
   nc_err(nc_put_att_text( ncid, varid_tmp, "cartesian_axis", 1, "T"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "units", 31, timestr.c_str()));
   nc_err(nc_put_att_text( ncid, varid_tmp, "calendar_type", 6, "JULIAN"));
   nc_err(nc_put_att_text( ncid, varid_tmp, "calendar", 6, "JULIAN"));
   // global attributes
   nc_err(nc_put_att_text( ncid, NC_GLOBAL, "hydrostatic", 15, "non-hydrostatic"));
   int intatt[1];
   intatt[0] = nemsio.ntrac;
   nc_err(nc_put_att_int( ncid, NC_GLOBAL, "ncnsto", NC_INT, 1, intatt));
   float nc_ak[nemsio.nz+1];
   for (int i = 0; i < nemsio.nz+1; i++) {
     nc_ak[i] = static_cast<float>(nemsio.ak[i]);
   }
   nc_err(nc_put_att_float( ncid, NC_GLOBAL, "ak", NC_FLOAT, nemsio.nz+1, nc_ak));
   for (int i = 0; i < nemsio.nz+1; i++) {
     nc_ak[i] = static_cast<float>(nemsio.bk[i]);
   }
   nc_err(nc_put_att_float( ncid, NC_GLOBAL, "bk", NC_FLOAT, nemsio.nz+1, nc_ak));
   nc_err(nc_put_att_text( ncid, NC_GLOBAL, "source", 6, "FV3GFS"));
   nc_err(nc_put_att_text( ncid, NC_GLOBAL, "grid", 8, "gaussian"));
   nc_err(nc_put_att_text( ncid, NC_GLOBAL, "converter", 66, "This file was converted from a NEMSIO file using nemsioatm2nc v1.0"));
   intatt[0] = nemsio.nx;
   nc_err(nc_put_att_int( ncid, NC_GLOBAL, "im", NC_INT, 1, intatt));
   intatt[0] = nemsio.ny;
   nc_err(nc_put_att_int( ncid, NC_GLOBAL, "jm", NC_INT, 1, intatt));

   // end define mode
   nc_err(nc_enddef(ncid));

   // write lat/lon/etc.
   double outx[nemsio.nx];
   double outy[nemsio.ny];
   int ii = 0;
   for (int j = 0; j < nemsio.ny; j++) {
     for (int i = 0; i < nemsio.nx; i++) {
       outy[j] = nemsio.lat[ii];
       outx[i] = nemsio.lon[ii];
       ii++;
     }
   }
   nc_err(nc_inq_varid( ncid, "grid_yt", &varid_tmp));
   nc_err(nc_put_var_double( ncid, varid_tmp, outy));
   nc_err(nc_inq_varid( ncid, "grid_xt", &varid_tmp));
   nc_err(nc_put_var_double( ncid, varid_tmp, outx));
   nc_err(nc_inq_varid( ncid, "lat", &varid_tmp));
   nc_err(nc_put_var_double( ncid, varid_tmp, nemsio.lat));
   nc_err(nc_inq_varid( ncid, "lon", &varid_tmp));
   nc_err(nc_put_var_double( ncid, varid_tmp, nemsio.lon));

   // need to convert pfull/phalf to float to write out
   float outpf[nemsio.nz];
   float outph[nemsio.nz+1];
   for (int i = 0; i < nemsio.nz; i++) {
     outpf[i] = static_cast<float>(nemsio.pfull[i]);
     outph[i] = static_cast<float>(nemsio.phalf[i]);
   }
   outph[nemsio.nz] = static_cast<float>(nemsio.phalf[nemsio.nz]);

   nc_err(nc_inq_varid( ncid, "pfull", &varid_tmp));
   nc_err(nc_put_var_float( ncid, varid_tmp, outpf));
   nc_err(nc_inq_varid( ncid, "phalf", &varid_tmp));
   nc_err(nc_put_var_float( ncid, varid_tmp, outph));
   nc_err(nc_inq_varid( ncid, "time", &varid_tmp));
   size_t start1[1], count1[1];
   start1[0] = 0;
   count1[0] = 1;
   double fhout = static_cast<double>(nemsio.fhour);
   nc_err(nc_put_vara_double( ncid, varid_tmp, start1, count1, &fhout));

   return 0;
 }

 int gfsnc::def_vars(nems2nc::nemsio nemsio) {
   // define variables based on what is in the input NEMSIO file
   // requires 'translation' of variable names
   int errval, varid_tmp;
   bool skip;
   int dimids2[2], dimids3[3];
   dimids2[0] = ydimid;
   dimids2[1] = xdimid;
   dimids3[0] = ydimid;
   dimids3[1] = xdimid;
   dimids3[2] = zdimid;
   nc_err(nc_redef(ncid));
   // loop through unique records
   for (auto & elem : nemsio.countRecs) {
     std::string vname_out;
     if ( varchanges.count(elem.first) > 0 ) {
       vname_out = varchanges.at(elem.first);
     } else {
       vname_out = elem.first;
     }
     skip=false;
     if ( elem.second == 1 ) {
        std::cout << "Defining 2D output variable " << vname_out << std::endl;
        nc_err(nc_def_var( ncid, vname_out.c_str(), NC_FLOAT, 2, dimids2, &varid_tmp));
     } else if ( elem.second == nemsio.nz ) {
        std::cout << "Defining 3D output variable "
                  << vname_out << ", levs=" << elem.second << std::endl;
        nc_err(nc_def_var( ncid, vname_out.c_str(), NC_FLOAT, 3, dimids3, &varid_tmp));
     } else {
        std::cout << "Error! " << vname_out << ", levs=" << elem.second
        << " != nlevs from NEMSIO header, this var will not be defined." << std::endl;
        skip=true;
     }
     if ( skip == false ) {
       if ( varlongnames.count(vname_out) > 0 ) {
       // define variable metadata if defined in the header file
       std::string long_name;
       long_name = varlongnames.at(vname_out);
       std::cout << long_name << " " << long_name.size() << std::endl;
 //      nc_err(nc_put_att_text( ncid, varid_tmp, "long_name", 16, "de"));
     }
   }
   nc_err(nc_enddef(ncid));
   nc_err(nc_close(ncid));
 }

}
