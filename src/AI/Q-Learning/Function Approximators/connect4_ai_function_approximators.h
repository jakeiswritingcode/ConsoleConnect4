// connect4_ai_linear_regression.h
// by Jake Charles Osborne III
#pragma once



#include <vector>



namespace connect4::ai {

    class RegressionModel {
    protected:
        std::vector<float> weights;

    public:
        virtual void updateWeights(const std::vector<float>& features, float target, float learningRate) = 0;
        virtual float predict(const std::vector<float>& features) const = 0;
    };

    class LinearRegression : public RegressionModel {
    public:
        LinearRegression(int featureCount) {
            weights.resize(featureCount + 1, 0.0f);  // +1 for bias term
        }

        float predict(const std::vector<float>& features) const override;

        void updateWeights(const std::vector<float>& features, float target, float learningRate) override;
    };

    class PolynomialRegression : public RegressionModel {
    public:
        PolynomialRegression(int featureCount) {
            weights.resize(
                featureCount + // each factor
                featureCount + // each factor squared
                featureCount * (featureCount - 1) / 2 + // each unique combination
                1, // bias term
                0.0f
            );
        }

        float predict(const std::vector<float>& features) const override;

        void updateWeights(const std::vector<float>& features, float target, float learningRate) override;
    };

}