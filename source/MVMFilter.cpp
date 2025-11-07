#include "MVMFilter.h"

#include <cmath>
#include <numbers>

MVMFilter::MVMFilter(const Parameters& parameters) : m_fs(parameters.Fs)
{
    set(parameters.f, parameters.tau);
}

double MVMFilter::process(double input)
{
    auto x_ = m_x1 * m_x - m_y1 * m_y + input;
    m_y = m_y1 * m_x + m_x1 * m_y;
    m_x = x_;
    return m_y;
}

void MVMFilter::set(double f, double tau)
{
    auto r = exp(-1/(tau * m_fs));
    auto theta = 2 * std::numbers::pi * f / m_fs;
    m_x1 = r * cos(theta);
    m_y1 = r * sin(theta);
}
