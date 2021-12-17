#include <iostream>
#include <string>

using namespace std;

/* A class to handle exceptions due to invalid input. */

class InvalidInput
{
public:
    InvalidInput(string userMessage) {
        message = userMessage;
    }

    string display() {
        return message;
    }
private:
    string message;
};