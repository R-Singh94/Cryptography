#include <iostream>

#include "Library/Header/RSA.h"

using namespace std;

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