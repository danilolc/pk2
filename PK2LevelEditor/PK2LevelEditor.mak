# Microsoft Developer Studio Generated NMAKE File, Based on PK2LevelEditor.dsp
!IF "$(CFG)" == ""
CFG=PK2LevelEditor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PK2LevelEditor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PK2LevelEditor - Win32 Release" && "$(CFG)" != "PK2LevelEditor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PK2LevelEditor.mak" CFG="PK2LevelEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PK2LevelEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PK2LevelEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PK2LevelEditor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PK2LevelEditor.exe"


CLEAN :
	-@erase "$(INTDIR)\LevelEditor.obj"
	-@erase "$(INTDIR)\PisteDraw.obj"
	-@erase "$(INTDIR)\PisteFont.obj"
	-@erase "$(INTDIR)\PisteInput.obj"
	-@erase "$(INTDIR)\PisteWait.obj"
	-@erase "$(INTDIR)\PK2Map.obj"
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PK2LevelEditor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PK2LevelEditor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PK2LevelEditor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\PK2LevelEditor.pdb" /machine:I386 /out:"$(OUTDIR)\PK2LevelEditor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\LevelEditor.obj" \
	"$(INTDIR)\PisteDraw.obj" \
	"$(INTDIR)\PisteFont.obj" \
	"$(INTDIR)\PisteInput.obj" \
	"$(INTDIR)\PisteWait.obj" \
	"$(INTDIR)\PK2Map.obj" \
	"$(INTDIR)\Resource.res" \
	"..\..\..\mssdk\DirectX7\lib\ddraw.lib" \
	"..\..\..\mssdk\DirectX7\lib\dinput.lib" \
	"..\..\Vc98\Lib\Winmm.lib"

"$(OUTDIR)\PK2LevelEditor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PK2LevelEditor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PK2LevelEditor.exe"


CLEAN :
	-@erase "$(INTDIR)\LevelEditor.obj"
	-@erase "$(INTDIR)\PisteDraw.obj"
	-@erase "$(INTDIR)\PisteFont.obj"
	-@erase "$(INTDIR)\PisteInput.obj"
	-@erase "$(INTDIR)\PisteWait.obj"
	-@erase "$(INTDIR)\PK2Map.obj"
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PK2LevelEditor.exe"
	-@erase "$(OUTDIR)\PK2LevelEditor.ilk"
	-@erase "$(OUTDIR)\PK2LevelEditor.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PK2LevelEditor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PK2LevelEditor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\PK2LevelEditor.pdb" /debug /machine:I386 /out:"$(OUTDIR)\PK2LevelEditor.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\LevelEditor.obj" \
	"$(INTDIR)\PisteDraw.obj" \
	"$(INTDIR)\PisteFont.obj" \
	"$(INTDIR)\PisteInput.obj" \
	"$(INTDIR)\PisteWait.obj" \
	"$(INTDIR)\PK2Map.obj" \
	"$(INTDIR)\Resource.res" \
	"..\..\..\mssdk\DirectX7\lib\ddraw.lib" \
	"..\..\..\mssdk\DirectX7\lib\dinput.lib" \
	"..\..\Vc98\Lib\Winmm.lib"

"$(OUTDIR)\PK2LevelEditor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PK2LevelEditor.dep")
!INCLUDE "PK2LevelEditor.dep"
!ELSE 
!MESSAGE Warning: cannot find "PK2LevelEditor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PK2LevelEditor - Win32 Release" || "$(CFG)" == "PK2LevelEditor - Win32 Debug"
SOURCE=.\LevelEditor.cpp

"$(INTDIR)\LevelEditor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\PisteEngine\PisteDraw.cpp

"$(INTDIR)\PisteDraw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PisteEngine\PisteFont.cpp

"$(INTDIR)\PisteFont.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PisteEngine\PisteInput.cpp

"$(INTDIR)\PisteInput.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PisteEngine\PisteWait.cpp

"$(INTDIR)\PisteWait.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Pk2\PK2Map.cpp

"$(INTDIR)\PK2Map.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Resource.rc

"$(INTDIR)\Resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

