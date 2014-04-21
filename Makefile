CHARMC=$(CHARM_HOME)/bin/charmc $(OPTS)

OBJS = sssp.o

all: sssp

sssp: $(OBJS)
	$(CHARMC) -language charm++ -o sssp $(OBJS)

sssp.decl.h: sssp.ci
	$(CHARMC)  sssp.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o sssp charmrun *.log *.sum *.sts sssp.exe sssp.pdb sssp.ilk

sssp.o: sssp.C sssp.decl.h
	$(CHARMC) -c sssp.C

test: all
	./charmrun ./sssp +p4 10 $(TESTOPTS)

bgtest: all
	./charmrun ./sssp +p4 10 +x2 +y2 +z2 +cth1 +wth1 $(TESTOPTS)
