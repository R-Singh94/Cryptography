
LIB_SRC = ./Library/Source
LIB_INC = ./Library/Header
ODIR = ./ObjectDir
SERV_FILE = ./server
CLI_FILE = ./client

generate: Server.o Client.o
		g++ -o server $(ODIR)/PrivateKey.o $(ODIR)/PublicKey.o $(ODIR)/RSA.o $(ODIR)/Server.o -lcppcms -l:libcryptopp.a
		g++ -o client $(ODIR)/PrivateKey.o $(ODIR)/PublicKey.o $(ODIR)/RSA.o $(ODIR)/Client.o -lboost_system -lcurl -l:libcryptopp.a

Server.o: Server.cpp RSA.o
		g++ -I $(LIB_INC) -c Server.cpp -lcppcms -l:libcryptopp.a -o $(ODIR)/Server.o

Client.o: Client.cpp RSA.o
		g++ -I $(LIB_INC) -c Client.cpp -lboost_system -lcurl -l:libcryptopp.a -o $(ODIR)/Client.o

RSA.o : $(LIB_INC)/RSA.h $(LIB_SRC)/RSA.cpp PrivateKey.o PublicKey.o
		g++ -I $(LIB_INC) -c $(LIB_SRC)/RSA.cpp -l:libcryptopp.a -o $(ODIR)/RSA.o

PrivateKey.o: $(LIB_SRC)/PrivateKey.cpp $(LIB_INC)/PrivateKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PrivateKey.cpp -l:libcryptopp.a -o $(ODIR)/PrivateKey.o

PublicKey.o: $(LIB_SRC)/PublicKey.cpp $(LIB_INC)/PublicKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PublicKey.cpp -l:libcryptopp.a -o $(ODIR)/PublicKey.o

clean:
		@echo "Removing Object Files and Executable"
		rm server
		rm client
		rm ./ObjectDir/*