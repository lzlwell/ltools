CC = g++
BIN =${HOME}/develop/ltools/bin
#BIN = .
BIN_DIR=${HOME}/bin
BASIC = conv_pos.o func.o r_poscar.o w_poscar.o w_pw.o
OBJS = $(BASIC) coordinates.o kpaths.o arrangebands.o addclmn.o chgz.o \
       supercell.o eqpbnd.o spinplot.o decompwfc.o cut2D.o fermisurf.o \
	   arpes.o abs_spec.o optrans.o findgap.o pdos.o realxct.o \
	   projbands.o kplusq.o
BIN_ALL = c2d d2c vasp2pw postrans kpaths arrangebands addclmn chgz \
	  supercell eqpbnd spinplot decompwfc cut2D fermisurf arpes \
	  abs_spec optrans findgap pdos realxct projbands kplusq
SCR = reclatt.sh final_coord.sh

# Binaries
all: $(BIN_ALL) scripts

c2d: $(BASIC) coordinates.o
	$(CC) -c coordinates.cpp -DC2D
	$(CC) -o $(BIN)/c2d $(BASIC) coordinates.o
	rm -f coordinates.o
	rm -f $(BIN_DIR)/c2d
	ln -s $(BIN)/c2d $(BIN_DIR)/c2d

d2c: $(BASIC) coordinates.o
	$(CC) -c coordinates.cpp -DD2C
	$(CC) -o $(BIN)/d2c $(BASIC) coordinates.o
	rm -f coordinates.o
	rm -f $(BIN_DIR)/d2c
	ln -s $(BIN)/d2c $(BIN_DIR)/d2c

vasp2pw: $(BASIC) coordinates.o
	$(CC) -c coordinates.cpp -DVASP2PW
	$(CC) -o $(BIN)/vasp2pw $(BASIC) coordinates.o
	rm -f coordinates.o
	rm -f $(BIN_DIR)/vasp2pw
	ln -s $(BIN)/vasp2pw $(BIN_DIR)/vasp2pw

postrans: $(BASIC) coordinates.o
	$(CC) -c coordinates.cpp -DPOSTRANS
	$(CC) -o $(BIN)/postrans $(BASIC) coordinates.o
	rm -f coordinates.o
	rm -f $(BIN_DIR)/postrans
	ln -s $(BIN)/postrans $(BIN_DIR)/postrans

kpaths: kpaths.o
	$(CC) -o $(BIN)/kpaths kpaths.o
	rm -f $(BIN_DIR)/kpaths
	ln -s $(BIN)/kpaths $(BIN_DIR)/kpaths

arrangebands: arrangebands.o conv_pos.o func.o
	$(CC) -o $(BIN)/arrangebands arrangebands.o conv_pos.o func.o
	rm -f $(BIN_DIR)/arrangebands
	ln -s $(BIN)/arrangebands $(BIN_DIR)/arrangebands

addclmn: addclmn.o
	$(CC) -o $(BIN)/addclmn addclmn.o
	rm -f $(BIN_DIR)/addclmn
	ln -s $(BIN)/addclmn $(BIN_DIR)/addclmn

chgz: chgz.o
	$(CC) -o $(BIN)/chgz chgz.o
	rm -f $(BIN_DIR)/chgz
	ln -s $(BIN)/chgz $(BIN_DIR)/chgz

supercell: supercell.o
	$(CC) -o $(BIN)/supercell supercell.o
	rm -f $(BIN_DIR)/supercell
	ln -s $(BIN)/supercell $(BIN_DIR)/supercell

eqpbnd: eqpbnd.o conv_pos.o func.o
	$(CC) -o $(BIN)/eqpbnd eqpbnd.o conv_pos.o func.o
	rm -f $(BIN_DIR)/eqpbnd
	ln -s $(BIN)/eqpbnd $(BIN_DIR)/eqpbnd

spinplot: spinplot.o 
	$(CC) -o $(BIN)/spinplot spinplot.o 
	rm -f $(BIN_DIR)/spinplot
	ln -s $(BIN)/spinplot $(BIN_DIR)/spinplot

decompwfc: decompwfc.o conv_pos.o func.o
	$(CC) -o $(BIN)/decompwfc decompwfc.o conv_pos.o func.o
	rm -f $(BIN_DIR)/decompwfc
	ln -s $(BIN)/decompwfc $(BIN_DIR)/decompwfc

cut2D: cut2D.o
	$(CC) -o $(BIN)/cut2D cut2D.o
	rm -f $(BIN_DIR)/cut2D
	ln -s $(BIN)/cut2D $(BIN_DIR)/cut2D

fermisurf: fermisurf.o conv_pos.o func.o
	$(CC) -o $(BIN)/fermisurf fermisurf.o conv_pos.o func.o
	rm -f $(BIN_DIR)/fermisurf
	ln -s $(BIN)/fermisurf $(BIN_DIR)/fermisurf

arpes: arpes.o func.o
	$(CC) -o $(BIN)/arpes arpes.o func.o
	rm -f $(BIN_DIR)/arpes
	ln -s $(BIN)/arpes $(BIN_DIR)/arpes

abs_spec: abs_spec.o func.o
	$(CC) -o $(BIN)/abs_spec abs_spec.o func.o
	rm -f $(BIN_DIR)/abs_spec
	ln -s $(BIN)/abs_spec $(BIN_DIR)/abs_spec

optrans: optrans.o
	$(CC) -o $(BIN)/optrans optrans.o
	rm -f $(BIN_DIR)/optrans
	ln -s $(BIN)/optrans $(BIN_DIR)/optrans

findgap: findgap.o func.o
	$(CC) -o $(BIN)/findgap findgap.o func.o
	rm -f $(BIN_DIR)/findgap
	ln -s $(BIN)/findgap $(BIN_DIR)/findgap

pdos: pdos.o
	$(CC) -o $(BIN)/pdos pdos.o
	rm -f $(BIN_DIR)/pdos
	ln -s $(BIN)/pdos $(BIN_DIR)/pdos

realxct: realxct.o
	$(CC) -o $(BIN)/realxct realxct.o
	rm -f $(BIN_DIR)/realxct
	ln -s $(BIN)/realxct $(BIN_DIR)/realxct

projbands: projbands.o
	$(CC) -o $(BIN)/projbands projbands.o
	rm -f $(BIN_DIR)/projbands
	ln -s $(BIN)/projbands $(BIN_DIR)/projbands

kplusq: kplusq.o
	$(CC) -o $(BIN)/kplusq kplusq.o
	rm -f $(BIN_DIR)/kplusq
	ln -s $(BIN)/kplusq $(BIN_DIR)/kplusq

scripts:
	rm -f $(BIN_DIR)/reclatt.sh
	rm -f $(BIN_DIR)/final_coord.sh
	rm -f $(BIN)/*.sh
	ln -s ../src/*.sh $(BIN)/
	ln -s $(BIN)/*.sh $(BIN_DIR)/


# Objects
func.o: func.cpp func.h
	$(CC) -c func.cpp

conv_pos.o: conv_pos.cpp conv_pos.h func.h
	$(CC) -c conv_pos.cpp

r_poscar.o: r_poscar.cpp r_poscar.h conv_pos.h func.h
	$(CC) -c r_poscar.cpp

w_poscar.o: w_poscar.cpp w_poscar.h conv_pos.h func.h
	$(CC) -c w_poscar.cpp

w_pw.o: w_pw.cpp w_pw.h func.h
	$(CC) -c w_pw.cpp

coordinates.o: coordinates.cpp coordinates.h conv_pos.h func.h

kpaths.o: kpaths.cpp
	$(CC) -c kpaths.cpp

arrangebands.o: arrangebands.cpp arrangebands.h conv_pos.h func.h
	$(CC) -c arrangebands.cpp

addclmn.o: addclmn.cpp
	$(CC) -c addclmn.cpp

chgz.o: chgz.cpp
	$(CC) -c chgz.cpp

supercell.o: supercell.cpp
	$(CC) -c supercell.cpp

eqpbnd.o: eqpbnd.cpp eqpbnd.h conv_pos.h func.h
	$(CC) -c eqpbnd.cpp

spinplot.o: spinplot.cpp spinplot.h
	$(CC) -c spinplot.cpp

decompwfc.o: decompwfc.cpp decompwfc.h conv_pos.h func.h
	$(CC) -c decompwfc.cpp

cut2D.o: cut2D.cpp
	$(CC) -c cut2D.cpp

fermisurf.o: fermisurf.cpp fermisurf.h conv_pos.h func.h
	$(CC) -c fermisurf.cpp

arpes.o: arpes.cpp arpes.h func.h
	$(CC) -c arpes.cpp

abs_spec.o: abs_spec.cpp abs_spec.h func.h
	$(CC) -c abs_spec.cpp

optrans.o: optrans.cpp optrans.h
	$(CC) -c optrans.cpp

findgap.o: findgap.cpp findgap.h func.h
	$(CC) -c findgap.cpp

pdos.o: pdos.cpp pdos.h
	$(CC) -c pdos.cpp

realxct.o: realxct.cpp realxct.h
	$(CC) -c realxct.cpp

projbands.o: projbands.cpp projbands.h
	$(CC) -c projbands.cpp

kplusq.o: kplusq.cpp kplusq.h
	$(CC) -c kplusq.cpp

	
# The above line starting with a 'tab', is an empty command line


##
clean:
	rm -rf $(OBJS)

uninstall:
	cd $(BIN); rm -rf $(BIN_ALL); rm -rf $(SCR)
	cd $(BIN_DIR); rm -rf $(BIN_ALL); rm -rf $(SCR)

clean-all:
	make clean; make uninstall
