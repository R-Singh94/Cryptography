#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <typeinfo>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/integer.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/algparam.h>
#include <cryptopp/nbtheory.h>

using namespace std;
#include "PrivateKey.h"
#include "PublicKey.h"

using CryptoPP::SHA256;

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
    CryptoPP::Integer getE(){
        return this->e->AbsoluteValue();
    }
    CryptoPP::Integer getZ(){
        return this->z->AbsoluteValue();
    }
    CryptoPP::Integer getD(){
        return this->d->AbsoluteValue();
    }
};

RSA::RSA()
{
    this->p = new CryptoPP::Integer();
    RSA::generatePrime(this->p, 1024);
    this->q = new CryptoPP::Integer();
    RSA::generatePrime(this->q, 1024);
    this->n = new CryptoPP::Integer(this->p->AbsoluteValue() * this->q->AbsoluteValue());
    this->z = new CryptoPP::Integer((this->p->AbsoluteValue()-1) * (this->q->AbsoluteValue()-1));
    this->e = new CryptoPP::Integer(65537);
    this->d = new CryptoPP::Integer(RSA::modularInverse(this->e->AbsoluteValue(), this->z->AbsoluteValue()));
    this->privateKey = new PrivateKey(this->n->AbsoluteValue(), this->d->AbsoluteValue());
    this->publicKey = new PublicKey(this->n->AbsoluteValue(), this->e->AbsoluteValue());
}

RSA::~RSA()
{
}

void RSA::generatePrime(CryptoPP::Integer* input_param, int bitLength){
    using namespace CryptoPP;

    AutoSeededRandomPool asrp;

    AlgorithmParameters params = MakeParameters("BitLength", bitLength)
        ("RandomNumberType", Integer::PRIME);
    input_param->GenerateRandom(asrp, params);

}

CryptoPP::Integer RSA::modularInverse(CryptoPP::Integer a, CryptoPP::Integer m){

    using namespace CryptoPP;

    //Check for realtive prime
    //TO-DO: Change to a Logger function and Exception Handling
    if(!RelativelyPrime(a, m)){
        cout<<"The numbers u("<<a<<"),v("<<m<<") are not relatively prime"<<endl;
        exit(1);
    }

    Integer mHolder = m;
    Integer x = 1, y = 0;

    //Trivial Check if PHI is 1
    if(m.Compare(1) == 0)
        return 0;

    //Implementation of Extended Euclidian Algorithm
    while( a.Compare(1) == 1 ){
        Integer q = a / m;
        Integer t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    //Negative values are not allowed, Hence converting the result to a positive number
    if(x.Compare(0) == -1)
        x += mHolder;

    return x;
}

PrivateKey* RSA::getPrivateKey(){
    return this->privateKey;
}

PublicKey* RSA::getPublicKey(){
    return this->publicKey;
}

string RSA::generateHash(const string& msg){
    const CryptoPP::byte* binaryData = (CryptoPP::byte*) msg.data();
    unsigned int msgLength = msg.size();
    CryptoPP::byte msgDigest[SHA256::DIGESTSIZE];

    SHA256().CalculateDigest(msgDigest, binaryData, msgLength);

    CryptoPP::HexEncoder encoder;
    string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(msgDigest, sizeof(msgDigest));
    encoder.MessageEnd();

    return output;
}

string RSA::generateSignature(const string& msg){
    //Generate the Hash and compute the signature
    string msgHash = RSA::generateHash(msg);
    CryptoPP::Integer integerMsgHash(msgHash.c_str());
    CryptoPP::Integer signature(a_exp_b_mod_c(integerMsgHash.AbsoluteValue(), this->privateKey->getD(), this->privateKey->getN()));

    //The Integer class does not have a string function, hence converting the value to string stream and then to string
    stringstream output;
    output<<signature;
    string output_str(output.str());
    
    //The Integer class appends a '.' to the end of the number, removing the character
    output_str.replace(output_str.size()-1, 1, "");
    return output_str;
}

bool RSA::validateSignature(const string& msg, const string& signature, PublicKey* publicKey){
    //Generating the Hash of the Message required for Comparision
    string msgHash = RSA::generateHash(msg);
    CryptoPP::Integer integerMsgHash(msgHash.c_str());

    //Computing the digest using public exponent
    CryptoPP::Integer integerSignature(signature.c_str());
    CryptoPP::Integer result(a_exp_b_mod_c(integerSignature.AbsoluteValue(), publicKey->getE(), publicKey->getN()));

    //Comparision of the Hash
    if(integerMsgHash.Compare(result) == 0)
        return true;
    else
        return false;
}

int main(){
    RSA inst;
    cout<<"Value of Z: "<<inst.getZ()<<endl;
    cout<<"Value of E: "<<inst.getE()<<endl;
    cout<<"Value of D: "<<inst.getD()<<endl;
    string input("Hello World");
    string signature = inst.generateSignature(input);
    cout<<"Signature : "<<signature<<endl;
    PublicKey* publicKey = inst.getPublicKey();
    cout<<"Validation : "<<RSA::validateSignature(input, signature, publicKey)<<endl;
}