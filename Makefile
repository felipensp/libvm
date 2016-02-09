CC?=gcc
CFLAGS=-fPIC -g -c -Wall -std=c99
SRC=src
BLD=build
LIB=lib
OBJS=$(BLD)/*.o

INSTALLINC=/usr/include/libvm
INSTALLLIB=/lib
INSTALLBIN=/usr/bin

vmtest:	libvm
	$(CC) -ovmtest -I$(SRC)/ test/main.c -L$(LIB)/ -lvm

libvm:
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -I$(SRC)/ -o$(BLD)/vm.o $(SRC)/vm.c
	@mkdir -p $(LIB)
	$(CC) -shared -Wl,-soname,$(LIB)/libvm.so.0 -o$(LIB)/libvm.so.0.1.0 $(OBJS)
	ln -sf libvm.so.0.1.0 $(LIB)/libvm.so.0
	ln -sf libvm.so.0.1.0 $(LIB)/libvm.so

clean:
	rm -rf vmtest $(LIB) $(BLD)/*.o $(LIB)/*

install:
	cp $(LIB)/* $(INSTALLLIB)
	ln -sf libvm.so.0.1.0 $(INSTALLLIB)/libvm.so.0
	ln -sf libvm.so.0.1.0 $(INSTALLLIB)/libvm.so
	cp $(SRC)/*.h $(INSTALLINC)

	$(CC) -ovmtest -I$(INSTALLINC)/ test/main.c -L$(INSTALLLIB)/ -lvm
