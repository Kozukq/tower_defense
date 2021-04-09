# /!\ J'ai mis à jour le makefile /!\
# Avant de compiler, il suffit d'ajouter dans la configuration générale :
# - EXEC : les noms sans préciser de suffixe des fichiers C avec un main().
# - OBJETS : les noms avec le suffixe .o des fichiers C n'ayant pas de main().
# Puis de générer les dépendances en exécutant "make depend", elles seront visibles en fin du makefile (ne pas oublier cette étape).
# Et enfin "make" pour compiler.

#
# CONFIGURATION GENERALE
#

EXEC = server client
OBJETS = config.o fenetre.o game.o interface.o ncurses.o
NOM_PROJET = tower_defense

#
# SUFFIXES
#

.SUFFIXES: .c .o

#
# OBJETS
#

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

#
# ARGUMENTS ET COMPILATEUR
#

CC = gcc
CCFLAGS_STD = -Wall -O3 -Werror -ansi -pedantic
CCFLAGS_MACOS = -I/usr/local/opt/ncurses/include
CCFLAGS_DEBUG = -D _DEBUG_
CCFLAGS = $(CCFLAGS_STD)
CCLIBS = -lncurses -lpthread
CCLIBS_MACOS = -L/usr/local/opt/ncurses/lib

#
# REGLES
#

all: msg $(OBJETS) $(EXEC_O)
	@echo "Creation des executables..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o $(OBJETS) $(CCLIBS); \
	done
	@rm -f $(EXEC_O)
	@echo "Termine."

mac: CCFLAGS = -Wall -O3 -Werror -std=c99 -pedantic $(CCFLAGS_MACOS)
mac: CCLIBS = -lncurses $(CCLIBS_MACOS)
mac: all

msg:
	@echo "Creation des objets..."

debug: CCFLAGS = $(CCFLAGS_STD) $(CCFLAGS_DEBUG)
debug: all

#
# REGLES PAR DEFAUT
#

.c.o: .h
	@cd $(dir $<) && ${CC} ${CCFLAGS} -c $(notdir $<) -o $(notdir $@)

#
# REGLES GENERALES
#

clean:
	@echo "Suppresion des objets, des fichiers temporaires..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@rm -f dependances
	@echo "Termine."

depend:
	@echo "Creation des dependances..."
	@sed -e "/^# DEPENDANCES/,$$ d" makefile > dependances
	@echo "# DEPENDANCES" >> dependances
	@for i in $(OBJETS_O); do \
	$(CC) -MM -MT $$i $(CCFLAGS) `echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` >> dependances; \
	done
	@cat dependances > makefile
	@rm dependances
	@echo "Termine."

#
# CREATION ARCHIVE
#

ARCHIVE_FILES = *

archive: clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename "$$PWD"`; cd .. && tar zcf $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz') $(addprefix "$$REP"/,$(ARCHIVE_FILES))
	@echo "Termine."

# DEPENDANCES
config.o: config.c config.h
fenetre.o: fenetre.c fenetre.h
game.o: game.c game.h
interface.o: interface.c interface.h fenetre.h game.h ncurses.h
ncurses.o: ncurses.c ncurses.h
server.o: server.c network.h config.h game.h
client.o: client.c network.h config.h interface.h fenetre.h game.h
