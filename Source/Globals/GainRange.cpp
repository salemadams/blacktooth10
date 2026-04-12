#include "GainRange.h"
#include <string>

const float MinGain{-12.f};
const float MaxGain{12.f};

const float GainInterval{0.5f};
const float GainSkew{1.f};

const float GainDefault{0.f};

const std::string PreGainId{"preGain"};
const std::string PostGainId{"postGain"};

const std::string PreGainName{"Gain"};
const std::string PostGainName{"Volume"};
