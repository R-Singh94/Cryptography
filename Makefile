
LIB_SRC = ./Library/Source
LIB_INC = ./Library/Header

execute: RSA.o
		g++ -o rsa PrivateKey.o PublicKey.o RSA.o -l:libcryptopp.a

RSA.o : RSA.cpp PrivateKey.o PublicKey.o
		g++ -I $(LIB_INC) -c RSA.cpp -l:libcryptopp.a

PrivateKey.o: $(LIB_SRC)/PrivateKey.cpp $(LIB_INC)/PrivateKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PrivateKey.cpp -l:libcryptopp.a

PublicKey.o: $(LIB_SRC)/PublicKey.cpp $(LIB_INC)/PublicKey.h
		g++ -I $(LIB_INC) -c $(LIB_SRC)/PublicKey.cpp -l:libcryptopp.a

clean:
		@echo "Removing Object Files"
		rm *.o