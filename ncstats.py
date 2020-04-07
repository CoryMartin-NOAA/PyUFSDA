#!/usr/bin/env python
import os
import sys 
import re
import numpy as np
##from __future__ import print_function
from netCDF4 import Dataset

nc1 = Dataset(sys.argv[1])
for dim in nc1.dimensions.keys():
    d = nc1[dim]
    print(d.name,d.size)
for attr in nc1.ncattrs():
    print(attr,nc1.getncattr(attr))
for varname in nc1.variables.keys():
    data1 = nc1[varname][:]
    if (data1.ndim == 4): # 3D var
        for z in range(0,data1.shape[1]):
            data1sub = data1[:,z,...]
            print('%s z=%s min=%s max =%s mean =%12.10f'%(varname, z+1, np.nanmin(data1sub), np.nanmax(data1sub), np.nanmean(data1sub)))
    else:
        print('%s min=%s max =%s mean =%12.10f'%(varname, data1.min(), data1.max(), np.nanmean(data1)))
