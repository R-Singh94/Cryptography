#include <iostream>
#include <sstream>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/algparam.h>
#include <cryptopp/nbtheory.h>

#include "RSA.h"

RSA::RSA()
{
    //Generating the 1st Integer
    this->p = new CryptoPP::Integer();
    RSA::generatePrime(this->p, 1024);
    //Generating the 2nd Integer
    this->q = new CryptoPP::Integer();
    RSA::generatePrime(this->q, 1024);
    //Computing the Modulo function
    this->n = new CryptoPP::Integer(this->p->AbsoluteValue() * this->q->AbsoluteValue());
    //Generating the PHI
    this->z = new CryptoPP::Integer((this->p->AbsoluteValue()-1) * (this->q->AbsoluteValue()-1));
    //Public Modules
    this->e = new CryptoPP::Integer(65537);
    //Generating the Private Modules
    this->d = new CryptoPP::Integer(RSA::modularInverse(this->e->AbsoluteValue(), this->z->AbsoluteValue()));
    //Creating the Private Key
    this->privateKey = new PrivateKey(this->n->AbsoluteValue(), this->d->AbsoluteValue());
    //Creating the Public Key
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
    CryptoPP::byte msgDigest[CryptoPP::SHA256::DIGESTSIZE];

    CryptoPP::SHA256().CalculateDigest(msgDigest, binaryData, msgLength);

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