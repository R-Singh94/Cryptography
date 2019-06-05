#include "PublicKey.h"

#include <sstream>

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

std::string PublicKey::getNStr(){
    //The Integer class does not have a string function, hence converting the value to string stream and then to string
    std::stringstream result;
    result<<this->n->AbsoluteValue();
    std::string result_str(result.str());
    
    //The Integer class appends a '.' to the end of the number, removing the character
    result_str.replace(result_str.size()-1, 1, "");
    return result_str;
}

std::string PublicKey::getEStr(){
    //The Integer class does not have a string function, hence converting the value to string stream and then to string
    std::stringstream result;
    result<<this->e->AbsoluteValue();
    std::string result_str(result.str());
    
    //The Integer class appends a '.' to the end of the number, removing the character
    result_str.replace(result_str.size()-1, 1, "");
    return result_str;
}