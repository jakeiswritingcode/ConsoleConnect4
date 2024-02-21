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

    void LinearRegression::updateWeights(const vector<float>& features, float learningRate, float tdTarget) {
        if (features.size() + 1 != weights.size()) throw std::logic_error("std::vector<float> of invalid size passed as features argument");

        float tdError = tdTarget - predict(features);

        for (size_t i = 0; i < features.size(); ++i) {
            weights[i] += learningRate * tdError * features[i];
        }
        weights.back() += learningRate * tdError;
    }

}