// linear_regression.cpp
// by Jake Charles Osborne III



#include "function_approximators.h"
#include <vector>
#include <stdexcept>

using std::vector;



namespace ai {

    float LinearRegression::predict(const vector<float>& features) const {
        if (features.size() != weights.size() - 1) throw std::logic_error("std::vector<float> of invalid size passed as features argument");
        
        float value = 0;

        for (size_t i = 0; i < features.size(); ++i) {
            value += weights[i] * features[i];
        }
        value += weights.back(); // bias term

        return value;
    }

    void LinearRegression::updateWeights(const vector<float>& features, float learningRate, float tdTarget) {
        if (features.size()!= weights.size() - 1) throw std::logic_error("std::vector<float> of invalid size passed as features argument");

        float tdUpdate = learningRate * (tdTarget - predict(features));

        for (size_t i = 0; i < features.size(); ++i) {
            weights[i] += tdUpdate * features[i];
        }
        weights.back() += tdUpdate; // bias term
    }

}