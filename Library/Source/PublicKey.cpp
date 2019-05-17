#include "PublicKey.h"

PublicKey::PublicKey(CryptoPP::Integer n, CryptoPP::Integer e){
    this->n = new CryptoPP::Integer(n);
    this->e = new CryptoPP::Integer(e);
}

PublicKey::~PublicKey(){
    
}

CryptoPP::Integer PublicKey::getN(){
    return this->n->AbsoluteValue();
}

CryptoPP::Integer PublicKey::getE(){
    return this->e->AbsoluteValue();
}