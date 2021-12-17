#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std; 

const double pi = 3.1415926;

double gaussianKernel(double x) {
    return (1 / sqrt(2 * pi)) * exp(-pow(x, 2) / 2);
}

double mean(vector<double> data) {
    double sum = 0;

    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

double standardDeviation(vector<double> data) {
    double variance = 0;
    double average = mean(data);

    for (double value : data) {
        variance += pow(value - average, 2);
    }

    return sqrt(variance / (data.size() - 1));
}

void sort(vector<double>& data) {
    int dataSize = data.size();
    for (int i = 0; i < dataSize; i++) {
        for (int j = i + 1; j < dataSize; j++) {
            if (data.at(i) > data.at(j)) {
                swap(data.at(i), data.at(j));
            }
        }
    }
}

double median(vector<double> data) {
    const int length = data.size();
    sort(data);

    if (length % 2 == 1) {
        return data.at(length / 2);
    }
    return (data.at(length / 2) + data.at((length / 2) - 1)) / 2;
}


double interquartileRange(vector<double> data) {
    /* Computes the interquartile range of a dataset set using the formula: IQR = Q3-Q1.*/
    const int length = data.size();
    sort(data);

    vector<double> firstHalf;
    vector<double> secondHalf;

    if (length % 2 == 0) {
        for (int i = 0; i < (length / 2); i++) {
            firstHalf.push_back(data[i]);
        }
        for (int i = (length / 2); i < length; i++) {
            secondHalf.push_back(data[i]);
        }
    }
    else {
        for (int i = 0; i < (length / 2); i++) {
            firstHalf.push_back(data[i]);
        }
        for (int i = (length / 2) + 1; i < length; i++) {
            secondHalf.push_back(data[i]);
        }
    }
    return median(secondHalf) - median(firstHalf);
}

void loadData(string fileName, vector<vector<double>>& data) {
    /* Loads data from a text file to a 2D array.*/
    ifstream input;

    input.open(fileName);

    double datapoint;

    vector<double> value;

    while (input >> datapoint) {
        value = {};
        value.push_back(datapoint);

        for (int i = 0; i < 9; i++) {
            input >> datapoint;
            value.push_back(datapoint);
        }

        data.push_back(value);
    }
}
