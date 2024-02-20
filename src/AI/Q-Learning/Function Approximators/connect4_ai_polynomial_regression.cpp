// connect4_ai_polynomial_regression.cpp
// by Jake Charles Osborne III



#include <vector>
#include "connect4_ai_function_approximators.h"
#include <stdexcept>

using std::vector;



namespace {

    void validateFeaturesSize(const vector<float>& features, const vector<float>& weights) {
        if (features.size() + // linear terms
            features.size() + // squared terms
            features.size() * (features.size() - 1) / 2 + // unique pairs
            1 != // bias
            weights.size())
        {
            throw std::logic_error("incorrect number of features provided to PolynomialRegression instance");
        }
    }

}

namespace connect4::ai {

    float PolynomialRegression::predict(const vector<float>& features) const {
        validateFeaturesSize(features, weights);

        float value = 0;

        int weightIndex = 0;

        // linear terms
        for (int i = 0; i < features.size(); ++i) {
            value += weights[weightIndex++] * features[i];
        }

        // squared terms
        for (int i = 0; i < features.size(); ++i) {
            value += weights[weightIndex++] * features[i] * features[i];
        }

        // unique pairs
        for (int i = 0; i < features.size(); ++i) {
            for (int j = i + 1; j < features.size(); ++j) {
                value += weights[weightIndex++] * features[i] * features[j];
            }
        }

        // bias term
        value += weights.back();

        return value;
    }

    // Update the weights based on the error (target - predicted)
    void PolynomialRegression::updateWeights(const vector<float>& features, float target, float learningRate) {
        validateFeaturesSize(features, weights);

        float prediction = predict(features);
        float error = target - prediction;

        int weightIndex = 0;

        // Update weights for linear terms
        for (int i = 0; i < features.size(); ++i) {
            weights[weightIndex++] += learningRate * error * features[i];
        }

        // Update weights for squared terms
        for (int i = 0; i < features.size(); ++i) {
            weights[weightIndex++] += learningRate * error * features[i] * features[i];
        }

        // Update weights for interaction terms
        for (int i = 0; i < features.size(); ++i) {
            for (int j = i + 1; j < features.size(); ++j) {
                weights[weightIndex++] += learningRate * error * features[i] * features[j];
            }
        }

        // Update bias weight
        weights[weightIndex++] += learningRate * error;
    }

}