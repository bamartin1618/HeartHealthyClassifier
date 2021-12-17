#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "InvalidInput.h"
#include "NaiveBayes.h"

using namespace std;

/* A struct to represent the user. */
struct userFood
{
    vector<double> nutrients;

    void getRating(bool response) { /* Converts the prediction of the classifier to a message. */
        if (response) {
            cout << "Your food is heart healthy!" << endl;
        }
        else {
            cout << "Your food is not heart healthy." << endl;
        }
    }
};

bool sequentialSearch(char element, vector<char> characterArray) {
    for (char c : characterArray) {
        if (c == element) {
            return true;
        }
    }
    return false;
}


bool isADigit(char element) {
    /* Searches an array of valid digits to determine if a character is a digit or not.*/
    return sequentialSearch(element, { '1','2','3','4','5','6','7','8','9','0','.','-'});
}

bool isreal(string number) {
    /* Determines if a string is a valid number or not.*/
    int decimalPoints = 0;

    int size = number.size();

    for (int i = 0; i < size; i++) {
        if (number[i] == '.') {
            decimalPoints += 1;
            /* Makes sure that strings such as 11.1 are accepted, but 11.1.1 are not. */
        }
        if (number[i] == '-' && i != 0) {
            /* Makes sure that strings such as -1.2 are accepted, but 1-.2 are not. */
            return false;
        }
        if (!isADigit(number[i]) || decimalPoints > 1) {
            /* Checks if the character isn't a valid digit or we have too many decimal points. */
            return false;
        }
    }
    return true;
}

void getVector(vector<double>& food) {
    /* Builds a vector for the user's food item. */
    double quantity;
    string input;

    double element;
    string nutrients[10] = { "calorie", "total fat", "saturated fat", "trans fat", "cholesterol", "sodium","carbohydrate", "dietary fiber", "sugar", "protein" };

    while (true) {
        /* Prompt the user to enter the quantity of their food item. */
        cout << "Enter the number of grams in a serving: ";

        try {
            cin >> input;

            /* Throw exceptions if the compiler receives invalid input. Otherwise, convert to a float.*/
            if (!isreal(input)) {
                throw InvalidInput("The value must be a decimal number.");
            }
            else {
                quantity = stof(input);
            }

            if (quantity < 0) {
                throw InvalidInput("The value must be non-negative.");
            }
            else {
                break;
            }
        }
        catch (InvalidInput error) {
            cout << error.display() << endl;
        }
    }
    cout << endl;

    for (int i = 0; i < 10; i++) {

        /* Use a while loop to continue to prompt the user for input until valid input is entered. */
        while (true) { 

            switch (i) {
            case 0:
                cout << "Enter the " << nutrients[i] << " content: ";
                break;
            case 4:
                cout << "Enter the " << nutrients[i] << " content (in milligrams): ";
                break;
            case 5:
                cout << "Enter the " << nutrients[i] << " content (in milligrams): ";
                break;
            default:
                cout << "Enter the " << nutrients[i] << " content (in grams): ";
            }

            try {
                cin >> input;

                /* Throw an exception in case of invalid input. */

                if (!isreal(input)) {
                    throw InvalidInput("The value must be a decimal number.");
                }
                else {
                    element = stof(input);
                }

                if (element < 0) {
                    throw InvalidInput("The value must be non-negative.");
                }
                else {
                    break;
                }
            }
            catch (InvalidInput error) {
                cout << error.display() << endl;
            }
        }

        food.push_back(element * (100 / quantity)); 
        /* All of the foods in the dataset are measured per 100 grams. This standardizes the values.*/
    }
}
int main() {
    vector<vector<double>> trainingX;
    vector<vector<double>> trainingY;

    /* Load data into training sets. */
    loadData("heartHealthyData.txt", trainingX);
    loadData("notHeartHealthyData.txt", trainingY);

    cout << fixed << showpoint << setprecision(3);

    NaiveBayes classifier(trainingX, trainingY); /* Initialize a classifier. */

    userFood food; /* Initialize the user's food.*/

    bool repeat = false; /* Flag variable to track if the user wants to enter another food.*/
    string input;

    cout << "Welcome to HeartSmart!" << endl;
    cout << "HeartSmart is a classifier that uses machine learning to determine if your food is heart healthy." << endl;
    cout << "Just enter some nutritional facts about your food and HeartSmart does the rest." << endl;

    cout << endl;

    do {
        food.nutrients = {}; /* Empties the food's nutrient vector. */

        getVector(food.nutrients); /* Fills the vector. */

        /* The classifier predicts the class of the food, then passes this to the getRating method. */
        food.getRating(classifier.decisionRule(food.nutrients));

        /* Prompt the user to know if they want to enter another food or not. */

        while (true) {

            /* Prompt the user to enter input until valid input is entered. */

            cout << "Do you want to test another food? (1 or 0): ";

            try {
                cin >> input;

                /* Throw an exception if there is invalid input. */
                if (input == "1" || input == "0") {
                    if (input == "1") {
                        repeat = true;
                    }
                    else {
                        repeat = false;
                    }
                    break;
                    cout << endl;
                }
                else {
                    throw InvalidInput("You have entered invalid input.");
                }
            }
            catch (InvalidInput error) {
                cout << error.display() << endl;
            }
        }
       
    } while (repeat);
}
