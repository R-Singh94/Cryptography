#include <iostream>
#include <string>
#include <exception>
#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/json.h>

#include "Library/Header/RSA.h"
#include "Library/Header/PublicKey.h"

using namespace std;

class Server : public cppcms::application
{
    public:
        Server(cppcms::service &ser) 
            : cppcms::application(ser)
        {
            cout<<"Server is Setup \n";
            dispatcher().assign("/rsa", &Server::rsaInvoke, this);
            mapper().assign("rsaInvoke", "/rsa");

            mapper().root("/signature");
        }

        void rsaInvoke();
};

void Server::rsaInvoke(){

    cout<<"RSA Request \n";

    //Generating the signature using RSA
    RSA inst;
    string input("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    string signature = inst.generateSignature(input);
    PublicKey *publicKey = inst.getPublicKey();
    cppcms::json::value result;
    result["message"] = input;
    result["signature"] = signature;
    result["public_key"]["n"] = publicKey->getNStr();
    result["public_key"]["e"] = publicKey->getEStr();

    //Generating the HTTP response
    response().status(cppcms::http::response::status_type::ok);
    response().content_type("application/json");
    response().out() << result;
    
    cout<<"Response sent \n";
}

int main(int argc, char** argv){
    try{
        cout<<"Server is being Initalized \n";
        cppcms::service ser(argc, argv);
        ser.applications_pool().mount(
            cppcms::applications_factory<Server>()
        );
        ser.run();
    }
    catch(std::exception const &e){
        cerr << e.what() << '\n';
    }
}