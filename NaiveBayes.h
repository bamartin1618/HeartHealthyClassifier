#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "HelperFunctions.h"

using namespace std;

/*
* Sources:
* https://en.wikipedia.org/wiki/Naive_Bayes_classifier
* https://en.wikipedia.org/wiki/Kernel_density_estimation
*/

/* The implementation of the classifier class using the Naive Bayes algorithm. */

class NaiveBayes
{
public:
    double kernelDensity(double, vector<double>);
    double bandwidth(vector<double>);

    bool decisionRule(vector<double>);

    NaiveBayes(vector<vector<double>>, vector<vector<double>>);
private:
    vector<vector<double>> healthyData;
    vector<vector<double>> unhealthyData;
};

double NaiveBayes::kernelDensity(double x, vector<double> data) {
    /* A function to estimate the probability that a data-point is an element of a data set. */
    double total = 0;
    const double dataBandwidth = bandwidth(data);

    for (double value : data) {
        total += gaussianKernel((x - value) / dataBandwidth);
    }

    return (1 / (data.size() * dataBandwidth)) * total;
}

double NaiveBayes::bandwidth(vector<double> data) {
    /* Computes the bandwidth of a data-set to smooth out the data. */

    const double a = standardDeviation(data);
    const double b = interquartileRange(data) / 1.34;

    const double userBandwidth = 0.9 * min(a, b) * pow(data.size(), -0.2);

    if (userBandwidth == 0) { 
        /* If the bandwidth is 0, return 1 to prevent a zero-division error.*/
        return 1;
    }
    return userBandwidth;
}

bool NaiveBayes::decisionRule(vector<double> test) {
    /* The decision rule of the classifier. Classifies a food as heart-healthy or not.*/

    double heartHealthy = 0;
    double notHeartHealthy = 0;

    vector<double> nutrient;
    double probability;

    int testSize = test.size();
    for (int i = 0; i < testSize; i++) {
        nutrient = {};

        int healthyDataSize = healthyData.size();
        for (int j = 0; j < healthyDataSize; j++) {
            nutrient.push_back(healthyData[j][i]);
        }

        /* Using the property of logarithms that log(a*b) = log(a)+log(b), we can add the logarithm of a probability to prevent underflow errors.*/

        probability = log10(kernelDensity(test[i], nutrient));

        if (isinf(probability)) {
            /* Handles cases in which the probability is 0. Subtracts 20 as a penalty.*/
            heartHealthy -= 20;
        }
        else {
            heartHealthy += probability;
        }
    }

    for (int i = 0; i < testSize; i++) {
        nutrient = {};

        int unhealthyDataSize = unhealthyData.size();
        for (int j = 0; j < unhealthyDataSize; j++) {
            nutrient.push_back(unhealthyData[j][i]);
        }

        probability = log10(kernelDensity(test[i], nutrient));

        if (isinf(probability)) {
            notHeartHealthy -= 20;
        }
        else {
            notHeartHealthy += probability;
        }
    }

    /* The tallies estimate the probability that a food belongs to a certain class. */
    /* If heartHealthy is greater than notHeartHealthy, it is more likely that the food is heart healthy and vice versa.*/

    return heartHealthy > notHeartHealthy;
}

NaiveBayes::NaiveBayes(vector<vector<double>> healthy, vector<vector<double>> unhealthy) {
    healthyData = healthy;
    unhealthyData = unhealthy;
}