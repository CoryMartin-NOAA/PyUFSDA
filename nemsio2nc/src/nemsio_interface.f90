module nemsio_interface_c
 use iso_c_binding
 use nemsio_module
 implicit none

contains
 subroutine nemsio_open_c(fname_len2, fname_in, fname_len) bind(c,name='nemsio_open_f90')
   integer(nemsio_intkind)         :: iret, ntrac
   type(nemsio_gfile) :: nemsfile
   integer(c_int), intent(in   ), value :: fname_len, fname_len2
   character(len=1,kind=c_char), dimension(fname_len), intent(in    ) :: fname_in
   character(len=255) :: fname
   integer :: i
   ! convert str from c++ to f90
   do i=1,fname_len2
      fname(i:i) = fname_in(i)
   end do
   do i=fname_len2+1,255
      fname(i:i) = ' '
   end do
   print *, 'Annyong from Fortran'
   print *, 'opening:', trim(fname)
   call nemsio_init(iret)
   call nemsio_open(nemsfile, trim(fname), "read", iret=iret)
   print *, iret
 end subroutine nemsio_open_c

end module nemsio_interface_c
