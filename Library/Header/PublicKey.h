#pragma once

#include <string>
#include <cryptopp/integer.h>

class PublicKey
{
private:
    CryptoPP::Integer* n;
    CryptoPP::Integer* e;
public:
    /**
     * @brief Construct a new Public Key object
     * 
     * @param CryptoPP::Integer : Modulus of the RSA algorithm
     * @param CryptoPP::Integer : The public exponent
     */
    PublicKey(CryptoPP::Integer, CryptoPP::Integer);
    ~PublicKey();
    /**
     * @brief Getter of the Modulus of RSA
     * 
     * @return CryptoPP::Integer 
     */
    CryptoPP::Integer getN();
    /**
     * @brief Getter of the public exponent
     * 
     * @return CryptoPP::Integer
     */
    CryptoPP::Integer getE();
    /**
     * @brief Getter of the Modulus of RSA as a string
     * 
     * @return std::string 
     */
    std::string getNStr();
    /**
     * @brief Getter of the public Exponenet as a string
     * 
     * @return std::string 
     */
    std::string getEStr();
};