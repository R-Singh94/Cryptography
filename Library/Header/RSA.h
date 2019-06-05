#pragma once

#include <string>
#include <cryptopp/integer.h>

#include "PrivateKey.h"
#include "PublicKey.h"

using namespace std;

class RSA
{
private:
    /**
     * @brief Private - 1st Prime number
     * 
     */
    CryptoPP::Integer* p;
    /**
     * @brief Private - 2nd Prime number
     * 
     */
    CryptoPP::Integer* q;
    /**
     * @brief Private - Holds the product of the two large prime numbers
     *          Modulus of RSA algorithm
     * 
     */
    CryptoPP::Integer* n;
    /**
     * @brief Private - PHI that holds (p-1*q-1)
     * 
     */
    CryptoPP::Integer* z;
    /**
     * @brief Private - Public exponent
     * 
     */
    CryptoPP::Integer* e;
    /**
     * @brief Private - Private exponent
     * 
     */
    CryptoPP::Integer* d;
    /**
     * @brief Private - Private Key
     * 
     */
    PrivateKey* privateKey;
    /**
     * @brief Private - Public key
     * 
     */
    PublicKey* publicKey;
    /**
     * @brief Private - Other User's Public Key
     * 
     */
    PublicKey* externalPublicKey;
public:
    /**
     * @brief Constructor for RSA:
     *  Initiates the process of generating the public and private key
     * 
     */
    RSA();
    ~RSA();
    /**
     * @brief Returns modulo inverse of 'a' with respect to 'm' using extended Euclid Algorithm
     *          Used to retrieve the private exponenet
     * 
     * @param CryptoPP::Integer* : a 
     * @param CryptoPP::Integer* : m 
     * @return CryptoPP::Integer
     */
    static CryptoPP::Integer modularInverse(CryptoPP::Integer, CryptoPP::Integer);
    /**
     * @brief Get the Private Key object
     * 
     * @return PrivateKey* 
     */
    PrivateKey* getPrivateKey();
    /**
     * @brief Get the Public Key object
     * 
     * @return PublicKey* 
     */
    PublicKey* getPublicKey();
    /**
     * @brief Generates the Hash using SHA256
     * 
     * @param string : Text to be hashed
     * @return string 
     */
    static string generateHash(const string&);
    /**
     * @brief Generates the signature using RSA
     * 
     * @param string& : The message to be sent to the recepient
     * @return string : The RSA signature
     */
    string generateSignature(const string&);
    /**
     * @brief Validates the RSA signature at the recepient
     * 
     * @return true 
     * @return false 
     */
    static bool validateSignature(const string&, const string&, PublicKey*);
    /**
     * @brief Generates a 128 byte prime number
     * 
     * @param CryptoPP::Integer*: input_param
     * @param int : bitLenght 
     */
    static void generatePrime(CryptoPP::Integer*, int);
};