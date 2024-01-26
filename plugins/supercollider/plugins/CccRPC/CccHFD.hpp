// PluginCccHFD.hpp
// Gianluca Elia (elgiano@gmail.com)

#pragma once

#include "SC_PlugIn.hpp"
#include "../common/RingBuf.hpp"


namespace Ccc {

class CccHFD : public SCUnit {
public:
    CccHFD();

    // Destructor
    // ~CccRPC();

private:
    void next(int nSamples);

    enum Inputs { IN_SIG, IN_WINSIZE, IN_HOPSIZE, IN_KMAX, IN_MAXWINSIZE};

    RingBuf<double>  ringBuf;
    double           hfd = 0;
    size_t           mMaxWindowSize;
    size_t           hopCounter = 0;
};

} // namespace Ccc
