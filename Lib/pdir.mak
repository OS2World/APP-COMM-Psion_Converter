# 
# Created by IBM WorkFrame/2 MakeMake at 20:21:46 on 19 Sept 1999
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

filename = pdir

.SUFFIXES:

.SUFFIXES: \
    .c .obj 

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tdc /Sp1 /Ss /Q /Gm /G4 /Gs /Ms /Gi /Gu /O /C %s

{}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tdc /Sp1 /Ss /Q /Gm /G4 /Gs /Ms /Gi /Gu /O /C %s

all: \
    .\$(filename).exe

.\$(filename).exe: \
    .\$(filename).obj \
    psilib.lib \
    {$(LIB)}$(filename).def
    @echo " Link::Linker "
    icc.exe @<<
     /Q /B" /nop /m"
     /Fe$(filename).exe 
     psilib.lib 
     $(filename).def
    .\$(filename).obj
<<

.\$(filename).obj: \
    $(filename).c
