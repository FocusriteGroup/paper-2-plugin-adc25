#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <numbers>

#include <MVMFilter.h>

constexpr double CutoffFrequency = 440.0;
constexpr double SampleRate = 48000.0;
constexpr double ProcessingDurationInSeconds = 1.0;
constexpr size_t NumberOfSamples = ProcessingDurationInSeconds*SampleRate;
constexpr size_t NumberOfProcessingLoop = 100;

volatile double sink; // prevents optimization

MVMFilter mvmFilter{MVMFilter::Parameters{}};
std::array<double, NumberOfSamples> impulse{};

void mvmFilterProcess()
{
    for(size_t j=0; j<NumberOfProcessingLoop; j++)
    {
        for(size_t i=0; i<NumberOfSamples; i++)
        {
            sink = mvmFilter.process(impulse[i]);
        }
    }
}

void sineProcess()
{
    for(size_t j=0; j<NumberOfProcessingLoop; j++)
    {
        for(size_t i=0; i<NumberOfSamples; i++)
        {
            double t = static_cast<double>(i) / SampleRate;
            sink = std::sin(2.0 * std::numbers::pi * CutoffFrequency * t);
        }
    }
}

int main()
{
    std::cout << std::fixed << std::setprecision(9) << std::left;

    impulse[0] = 1.0f;
    auto start = std::chrono::steady_clock::now();
    mvmFilterProcess();
    auto end = std::chrono::steady_clock::now();
    std::cout << "MVM Filter processing time " << end - start << std::endl;

    start = std::chrono::steady_clock::now();
    sineProcess();
    end = std::chrono::steady_clock::now();
    std::cout << "std::sine  processing time " << end - start << std::endl;
}