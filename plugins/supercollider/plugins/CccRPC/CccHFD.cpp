// PluginCccHFD.cpp
// Gianluca Elia (elgiano@gmail.com)

#include "SC_PlugIn.hpp"
#include "CccHFD.hpp"
#include "../../../../HFD.hpp"
#include <Eigen/Dense>
#include <iostream>

static InterfaceTable* ft;

namespace Ccc {

CccHFD::CccHFD() {
    double maxWindowSize = in0(IN_MAXWINSIZE); 
    mMaxWindowSize = static_cast<size_t>(maxWindowSize / 1000.0 * sampleRate());
    ringBuf.setSize(mMaxWindowSize + 1);
    mCalcFunc = make_calc_function<CccHFD, &CccHFD::next>();
    next(1);
}

void CccHFD::next(int nSamples) {
    const float* input = in(IN_SIG);
    float* outbuf = out(0);
    double windowSize = in0(IN_WINSIZE); 
    double hopSize = in0(IN_HOPSIZE) * windowSize; 
    int kMax = static_cast<int>(in0(IN_KMAX));
    size_t windowSizeInSamples = static_cast<size_t>(windowSize / 1000.0 * sampleRate());
    size_t hopSizeInSamples = static_cast<size_t>(hopSize / 1000.0 * sampleRate());
    windowSize = sc_min(windowSizeInSamples, mMaxWindowSize);
    hopSizeInSamples = sc_min(hopSizeInSamples, mMaxWindowSize);

    for (int i = 0; i < nSamples; ++i) {
        ringBuf.push(input[i]);
        hopCounter++;

        if (hopCounter >= hopSizeInSamples) {
            hopCounter = 0;
            auto window = ringBuf.getBuffer(windowSize);
            hfd = HFD::calc(window, kMax);
        }
        outbuf[i] = hfd;
    }
}


} // namespace Ccc
