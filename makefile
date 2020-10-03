CPPFLAGS = -w -Wfatal-errors -std=gnu++0x -O3 -mfpmath=sse -march=native -g -m64 -fexceptions -ftree-vectorize -I/opt/ibm/ILOG/CPLEX_Studio129/opl/include -I/usr/include/mysql-cppconn-8/jdbc -I ../BibGrafos -I ../BibValidador -DILOUSEMT -D_REENTRANT -DIL_STD
LNKFLAGS1 = -w -std=gnu++0x -O3 -mfpmath=sse -march=native -g -m64 -pthread -fexceptions -ftree-vectorize -I/opt/ibm/ILOG/CPLEX_Studio129/opl/include -I/usr/include/mysql-cppconn-8/jdbc -I ../BibGrafos -I ../BibValidador -DILOUSEMT -D_REENTRANT
LNKFLAGS2 = -L/opt/ibm/ILOG/CPLEX_Studio129/opl/lib/x86-64_linux/static_pic -lopl -liljs -lilocplex -lcp -lconcert -L/opt/ibm/ILOG/CPLEX_Studio129/opl/bin/x86-64_linux -lcplex1290 -loplnl1 -ldl -licuuc -licui18n -licuio -licudata -L/usr/include/mysql-cppconn-8/jdbc -lmysqlcppconn -L /usr/include/boost -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -L ../BibGrafos -lBibGrafos -L ../BibValidador -lBibValidador -DILOUSEMT -D_REENTRANT -DIL_STD
 
COMP = g++

TARGET = VDCordais

OBJ = main.o LocalLib.o VDGrafoSplitIndiferenca.o VDGrafo2Cliques.o VDDirectedPath.o

all: $(TARGET)

$(TARGET): $(OBJ) libBibGrafos.a libBibValidador.a
	$(COMP) $(LNKFLAGS1) $(OBJ) $(LNKFLAGS2) -o $@

main.o: main.cpp LocalLib.o
	$(COMP) $(CPPFLAGS) -c $< 

LocalLib.o: LocalLib.cpp LocalLib.h 
	$(COMP) $(CPPFLAGS) -c $< 

VDGrafo2Cliques.o: VDGrafo2Cliques.cpp VDGrafo2Cliques.h
	$(COMP) $(CPPFLAGS) -c $< 	

VDGrafoSplitIndiferenca.o: VDGrafoSplitIndiferenca.cpp VDGrafoSplitIndiferenca.h
	$(COMP) $(CPPFLAGS) -c $< 	

VDDirectedPath.o: VDDirectedPath.cpp VDDirectedPath.h
	$(COMP) $(CPPFLAGS) -c $<

.PHONY: libBibGrafos.a
libBibGrafos.a:
	$(MAKE) -C ../BibGrafos/

.PHONY: libBibValidador.a
libBibValidador.a:
	$(MAKE) -C ../BibValidador/

clean:
	rm -f $(TARGET) $(OBJ)
