
LIB_SRC = ./Library/Source
LIB_INC = ./Library/Header
ODIR = ./ObjectDir

generateServer: Server.o
		g++ -o server $(ODIR)/PrivateKey.o $(ODIR)/PublicKey.o $(ODIR)/RSA.o $(ODIR)/Server.o -lcppcms -l:libcryptopp.a

Server.o: Server.cpp RSA.o PublicKey.o
		g++ -I $(LIB_SRC) -c Server.cpp -lcppcms -l:libcryptopp.a -o $(ODIR)/Server.o

RSA.o : $(LIB_INC)/RSA.h $(LIB_SRC)/RSA.cpp PrivateKey.o PublicKey.o
		g++ -I $(LIB_INC) -c $(LIB_SRC)/RSA.cpp -l:libcryptopp.a -o $(ODIR)/RSA.o

PrivateKey.o: $(LIB_SRC)/PrivateKey.cpp $(LIB_INC)/PrivateKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PrivateKey.cpp -l:libcryptopp.a -o $(ODIR)/PrivateKey.o

PublicKey.o: $(LIB_SRC)/PublicKey.cpp $(LIB_INC)/PublicKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PublicKey.cpp -l:libcryptopp.a -o $(ODIR)/PublicKey.o

clean:
		@echo "Removing Object Files and Executable"
		rm server
		rm ./ObjectDir/*