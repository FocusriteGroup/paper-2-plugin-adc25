#pragma once

class MVMFilter
{
public:

    struct Parameters
    {
        double f{1000.0};
        double tau{0.01};
        double Fs{48000.0};
    };

    MVMFilter()
    : MVMFilter(Parameters{})
    {
    }

    explicit MVMFilter(const Parameters& parameters);
    double process(double);
    void set(double f, double tau);

private:
    double m_x{};
    double m_y{};
    double m_fs;
    double m_x1{};
    double m_y1{};
    double m_scale{};
};
