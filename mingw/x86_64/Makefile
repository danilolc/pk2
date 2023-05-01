
PLAT = x86_64

OPT = -O2

CXX = $(PLAT)-w64-mingw32-g++
WINRES = $(PLAT)-w64-mingw32-windres
BIN_SRC = /usr/local/$(PLAT)-w64-mingw32/bin/
INC_SRC = /usr/local/$(PLAT)-w64-mingw32/include/
LIB_SRC = /usr/local/$(PLAT)-w64-mingw32/lib/


# checking for msys2 with official snippet not possible in github-actions
# check if file exists, and substitute with msys2-path if needed
ifeq ("$(wildcard $($(PLAT)-w64-mingw32-windres))","")
  WINRES = /opt/bin/$(PLAT)-w64-mingw32-windres
endif

ifeq ("$(wildcard $(/usr/local/$(PLAT)-w64-mingw32/bin/))","")
  BIN_SRC = /mingw64/bin/
endif

ifeq ("$(wildcard $(/usr/local/$(PLAT)-w64-mingw32/include/))","")
  INC_SRC = /mingw64/include/
endif

ifeq ("$(wildcard $(/usr/local/$(PLAT)-w64-mingw32/lib/))","")
  LIB_SRC = /mingw64/lib/
endif


-include ../mingw.mk
