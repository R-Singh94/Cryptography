#include <iostream>

#include "Library/Header/RSA.h"

using namespace std;

int main(){
    RSA inst;
    string input("Hello World");
    string signature = inst.generateSignature(input);
    cout<<"Signature : "<<signature<<endl;
    PublicKey* publicKey = inst.getPublicKey();
    cout<<"Validation : "<<RSA::validateSignature(input, signature, publicKey)<<endl;
}