#include "PrivateKey.h"

PrivateKey::PrivateKey(CryptoPP::Integer n, CryptoPP::Integer d){
    this->n = new CryptoPP::Integer(n);
    this->d = new CryptoPP::Integer(d);
}

PrivateKey::~PrivateKey(){
    
}

CryptoPP::Integer PrivateKey::getN(){
    return this->n->AbsoluteValue();
}

CryptoPP::Integer PrivateKey::getD(){
    return this->d->AbsoluteValue();
}