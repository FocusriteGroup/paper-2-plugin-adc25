#include "MVMFilter.h"

#include <cmath>
#include <numbers>

MVMFilter::MVMFilter(const Parameters& parameters) : m_fs(parameters.Fs)
{
}

double MVMFilter::process(double input)
{
    return 0;
}

void MVMFilter::set(double f, double tau)
{
}
