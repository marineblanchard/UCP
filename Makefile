# ---------------------------------------------------------------------
# Options de la plateforme
# ---------------------------------------------------------------------

#PLATEFORME := `/bin/uname -n`
on_etud = yes

ifeq ($(on_etud), yes)
  #options pour etud
  SYSTEM     = x86-64_osx
  LIBFORMAT  = static_pic
else
  # options pour lxr
  SYSTEM     = i86_linux2_glibc2.2_gcc3.0
  LIBFORMAT  = static_pic_mt
endif

# ---------------------------------------------------------------------
# Options du compilo (mon gcc/ g++ prefere)
# ---------------------------------------------------------------------

PARANO = -Wall -ansi -pedantic
CCC    = g++


# ---------------------------------------------------------------------
# Options pour le linker et les librairies
# ---------------------------------------------------------------------

CCOPT         = -O -fPIC -fexceptions -DNDEBUG $(PARANO) -DIL_STD -DILOSTRICTPOD -DILO_LINUX

ifeq ($(on_etud), yes)
  #options pour etud
  CPLEXDIR      = /Applications/CPLEX_Studio128/cplex/
  CONCERTDIR    = /Applications/CPLEX_Studio128/concert/
else
  # options pour lxr
  CPLEXDIR      = /home/ilog/cplex80
  CONCERTDIR    = /home/ilog/concert12
endif

#options communes
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCFLAGS       = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
CCLNFLAGS     = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl

# ------------------------------------------------------------
# Coincoin !
#------------------------------------------------------------

SRC1 = main.cpp probleme.cpp solution.cpp
OBJ1 = main.o probleme.o solution.o
EXE1 = main


# ------------------------------------------------------------
# The regle pour make tout court
#------------------------------------------------------------

all: $(EXE1)


# ------------------------------------------------------------
# The nettoyage par le vide
#------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~
	/bin/rm -rf *.sav *.net *.msg *.log

distclean : clean
	/bin/rm -rf $(EXE1) $(EXE2)


# ------------------------------------------------------------
# The regles pour la compilation
#------------------------------------------------------------

%.o : %.cpp
	@echo "   obj file : "$*.cpp
	@$(CCC) -c $(CCFLAGS) $*.cpp -o $*.o

$(EXE1): $(OBJ1)
	@echo "   exe file : "$(EXE1)
	@$(CCC) $(OBJ1) -o $(EXE1) $(CCLNFLAGS)


