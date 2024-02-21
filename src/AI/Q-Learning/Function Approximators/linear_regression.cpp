// linear_regression.cpp
// by Jake Charles Osborne III



#include "function_approximators.h"
#include <vector>
#include <stdexcept>

using std::vector;



namespace ai {

    float LinearRegression::predict(const vector<float>& features) const {
        float value = 0;

        for (size_t i = 0; i < features.size(); ++i) {
            value += weights[i] * features[i];
        }
        value += weights.back(); // bias term

        return value;
    }

    // Update the weights based on the error (target - predicted)
    void LinearRegression::updateWeights(const vector<float>& features, float target, float learningRate) {
        if (features.size() + 1 != weights.size()) throw std::logic_error("std::vector<float> of invalid size passed as features argument");

        float prediction = predict(features);
        float error = target - prediction;

        for (size_t i = 0; i < features.size(); ++i) {
            weights[i] += learningRate * error * features[i];
        }
        weights.back() += learningRate * error;
    }

}