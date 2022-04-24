@echo off

set "diag_options= /nologo /FC /diagnostics:column /W4 /wd4201 /wd4100"

IF NOT EXIST .\build mkdir build
pushd .\build

set "compile_options= %diag_options% /Od /Zo /Zf /Z7"
set "link_options= /INCREMENTAL:NO /opt:ref"

cl %compile_options% ..\src\main.c /link %link_options% /PDB:builder.pdb /out:builder.exe

del main.obj

popd
