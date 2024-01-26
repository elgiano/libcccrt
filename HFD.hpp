// PluginCccHFD.cpp
// Gianluca Elia (elgiano@gmail.com)
#pragma once

#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
struct HFD {
    // Get slope of best fitting curve through (x,y) = (ln(1/k), ln(L(k))
    static double fitSlope(const Array<double, Dynamic, 1>& x) {
        auto kMax = x.size();
        MatrixXd A(kMax, 2);
        // use ln(k) instaead of ln(1/k) saves some divisions: 
        // just flip slope's sign in the end
        A << VectorXd::Ones(kMax), VectorXd::LinSpaced(kMax, 1, kMax).array().log();
        VectorXd b = x.log();
        auto At = A.transpose();
        VectorXd slope = (At * A).llt().solve(At * b);
        return -slope(1);
    }

#define IMPL1
    static double calc(const Array<double,Dynamic,1>& x, int kMax) {
        int N = x.size();
        Array<double,Dynamic,1> higuchiDim(kMax);

        for (int k = 1; k <= kMax; ++k) {
            double sum = 0.0;
#ifdef IMPL0
            double avgSize = (N - 1.0) / (k * (N - k) * k);
            for (int m = 0; m < k; ++m) {
                double subSum = 0.0;
                size_t iMax = (N - 1 - m) / k;
                for (int i = 1; i <= iMax; ++i)
                    subSum += std::abs(x(m + (i - 1) * k) - x(m + i * k));
                sum += subSum * avgSize;
            }
            higuchiDim(k - 1) = sum;
#else
            for (int m = 0; m < k; ++m) {
                sum += x(seq(m + k, N - 1, k))
                    .absolute_difference(
                       x(seq(m, N - 1 - k, k))
                    ).mean();
            }
            higuchiDim(k - 1) = sum * (N - 1) / (k*k*k);
#endif
        }

        return fitSlope(higuchiDim);
    }
};
