#include <iostream>
#include <string>
#include <exception>
#include <sstream>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cryptopp/integer.h>

#include "Library/Header/RSA.h"
#include "Library/Header/PublicKey.h"

using namespace std;

void validateSignature(string &response_string) {
    try{
        stringstream response_sstream;
        response_sstream << response_string;
        //Reading the JSON as text into a property_tree
        boost::property_tree::ptree pTree;
        boost::property_tree::read_json(response_sstream, pTree);
        //Extracting content from JSON
        string message(pTree.get<string>("message"));
        string signature(pTree.get<string>("signature"));
        CryptoPP::Integer n(pTree.get<string>("public_key.n").c_str());
        CryptoPP::Integer e(pTree.get<string>("public_key.e").c_str());
        PublicKey *senderPublicKey = new PublicKey(n,e);
        RSA instance;
        //Validating the Signature
        if(instance.validateSignature(message, signature, senderPublicKey))
            cout<<"The Signature is Valid. The message is obtained from an authentic user.\n";
        else
            cout<<"The Signature is Invalid.\n";
    }
    catch(exception e){
        cerr << "Exception Raised : " << e.what() << '\n';
    }
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

int main(){
    CURL *curl;
    CURLcode res;

    string response_string;
    string header_string;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/signature/rsa");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
 
        // Perform the GET request, res will get the return code
        res = curl_easy_perform(curl);

        // Check for errors 
        if(res != CURLE_OK){
            std::cerr << "The response was NOK. Status : " << curl_easy_strerror(res) << '\n';
            exit(1);
        }

        cout<< "Response : "<< response_string << '\n';

        validateSignature(response_string);

        //Performs Cleanup
        curl_easy_cleanup(curl);
    }
}