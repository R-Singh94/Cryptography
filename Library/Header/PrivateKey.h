#pragma once

#include <cryptopp/integer.h>

class PrivateKey
{
private:
    CryptoPP::Integer* n;
    CryptoPP::Integer* d;
public:
    /**
     * @brief Construct a new Private Key object
     * 
     * @param CryptoPP::Integer : Modulus of RSA Algorithm
     * @param CryptoPP::Integer : Private Exponent
     */
    PrivateKey(CryptoPP::Integer, CryptoPP::Integer);
    ~PrivateKey();
    /**
     * @brief Getter for the Modulus
     * 
     * @return CryptoPP::Integer 
     */
    CryptoPP::Integer getN();
    /**
     * @brief Getter for the private exponent
     * 
     * @return CryptoPP::Integer
     */
    CryptoPP::Integer getD();
};
