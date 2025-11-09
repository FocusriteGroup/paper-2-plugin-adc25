import numpy as np
import matplotlib.pyplot as plt

from matplotlib import patches
from matplotlib.figure import Figure
from matplotlib import rcParams

from scipy import signal

from .signal import impulse_signal

sample_rate = 48000

def get_impulse_response(filterFunction, L):
    impulse_response = np.zeros(L)
    for index, impulse_sample in enumerate(impulse_signal(L)):
        impulse_response[index] = filterFunction.process(impulse_sample)
    return impulse_response

def signal_dB(signal):
    magnitude = np.abs(signal)
    magnitude[magnitude == 0] = np.finfo(np.float64).eps
    return 20 * np.log10(magnitude)

def get_frequency_response(filterFunction, signal_length):
    impulse_response = np.zeros(signal_length)

    for index, impulse_sample in enumerate(impulse_signal(signal_length)):
        impulse_response[index] = filterFunction.process(impulse_sample)

    return signal_dB(np.fft.rfft(impulse_response))

def analyse_filter(filterFunction):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), layout='constrained')

    frequencyResponse = get_frequency_response(filterFunction, 4096)
    impulseReponse = get_impulse_response(filterFunction, 20)
    xaxis = np.pi * np.arange(0, frequencyResponse.size) / frequencyResponse.size
    
    ax1.stem(impulseReponse)
    ax1.set_xlabel('Sample')
    ax1.set_ylabel('Magnitude')
    ax1.set_title('Impulse Response')
    ax1.grid()
    
    ax2.set_xlabel('Frequency')
    ax2.set_ylabel('Magnitude')
    ax2.set_title('Frequency Response')
    ax2.grid()
    ax2.plot(xaxis, frequencyResponse)
    plt.show()

def get_complex_impulse_response(filterFunction, L):
    impulse_response = np.zeros(L, dtype='complex')
    for index, impulse_sample in enumerate(impulse_signal(L)):
        impulse_response[index] = filterFunction.process(impulse_sample)
    return impulse_response

def get_complex_frequency_response(filterFunction, signal_length):
    impulse_response = get_complex_impulse_response(filterFunction, signal_length)
    return signal_dB(np.fft.fft(impulse_response))

def analyse_filter_complex(filterFunction, length = 20):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), layout='constrained')

    frequencyResponse = get_complex_frequency_response(filterFunction, 4096)
    impulseReponse = get_complex_impulse_response(filterFunction, length)
    xaxis = -np.pi + 2 * np.pi * np.arange(0, frequencyResponse.size) / frequencyResponse.size
    
    ax1.plot(np.real(impulseReponse), np.imag(impulseReponse), marker='x')
    ax1.set_xlim([-2,2])
    ax1.set_ylim([-2, 2])
    ax1.set_xlabel('Real')
    ax1.set_ylabel('Imaginary')
    ax1.set_title('Impulse Response')
    ax1.grid()
    
    ax2.set_xlabel('Frequency')
    ax2.set_ylabel('Magnitude')
    ax2.set_title('Frequency Response')
    ax2.grid()
    ax2.plot(xaxis, np.fft.fftshift(frequencyResponse))
    plt.show()

def get_poles_zeros_from_coefs(numerator_coeffs, denominator_coeffs):
    '''
    return poles and zeros from the specified numerator and denominator coefficients of a filter transfer function.
    It does so by scaling the coefficients if necessary, and then finding the roots.
    '''

    if np.max(numerator_coeffs) > 1:
        kn = np.max(numerator_coeffs)
        numerator_coeffs = numerator_coeffs/float(kn)

    if np.max(denominator_coeffs) > 1:
        kd = np.max(denominator_coeffs)
        denominator_coeffs = denominator_coeffs/float(kd)

    poles = np.roots(denominator_coeffs)
    zeros = np.roots(numerator_coeffs)

    return poles, zeros

def plot_poles_zeros_from_coefs(poles, zeros):
    '''
    Plot the complex z-plane given the poles and zeros of a transfer function.
    '''

    # get a figure/plot
    ax = plt.subplot()

    # create the unit circle
    uc = patches.Circle((0, 0), radius=1, fill=True,
                        color='c', ls='dashed')
    ax.add_patch(uc)
    plt.text(0.1, 0.5, 'Stable region', fontsize=8)

    # Plot the zeros and set marker properties
    t1 = plt.plot(zeros.real, zeros.imag, 'go', ms=10, label='zero')
    plt.setp(t1, markersize=10.0, markeredgewidth=1.0,
             markeredgecolor='k', markerfacecolor='g')

    # Plot the poles and set marker properties
    t2 = plt.plot(poles.real, poles.imag, 'rx', ms=10, label='pole')
    plt.setp(t2, markersize=12.0, markeredgewidth=3.0,
             markeredgecolor='r', markerfacecolor='r')

    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    # set the ticks
    r = 1.5;
    plt.axis('scaled');
    plt.axis([-r, r, -r, r])
    ticks = [-1, -.5, .5, 1];
    plt.xticks(ticks);
    plt.yticks(ticks)
    plt.legend()

def plot_frequency_response(freq, freq_response):
    '''
    Plot the given frequency response.
    '''
    plt.plot(freq, freq_response)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.ylim(-50, 30)
    plt.xlim(1, int(np.floor(sample_rate / 2)))
    plt.title('Frequency response')
    plt.grid()

def plot_fft_frequency_response(filter, signal_length, sample_rate):
    '''
    Plot given filter's frequency response using the FFT method (FFT of the filter's impulse response)
    '''
    impulse_response = np.zeros(signal_length)

    for index, impulse_sample in enumerate(impulse_signal(signal_length)):
        impulse_response[index] = filter.process(impulse_sample)

    y = np.fft.rfft(impulse_response)
    y_log = 20 * np.log10(np.abs(y))
    x = np.linspace(0, sample_rate / 2, y_log.shape[0])

    plt.plot(x, y_log)
    plt.xlabel('Frequency (Hz)')
    plt.xscale('log')
    plt.ylabel('Magnitude (dB)')
    plt.title('Frequency response')
    plt.grid()
    plt.show()

def get_direct_frequency_response(b, a, sample_rate, num_points):
    '''
    Calculate the frequency response of the filter specified by the numerator and denominator coefficients,
    using the direct evaluation method (using the resulting transfer function from b, a coefficients, substituting z by
    e^{j*thetha} and calculating the magnitude for theta varying from 0 to Nyquist)
    '''
    transfer_function = lambda z: \
        sum([b * (z ** -i) for i, b in enumerate(b)]) / \
        sum([a * (z ** -i) for i, a in enumerate(a)])

    theta = np.linspace(0, np.pi, num_points)
    z = np.exp(1j * theta)

    freq_response = 20 * np.log10(np.abs(transfer_function(z)))
    f = theta * sample_rate / (2 * np.pi)
    return f, freq_response

def plot_filter_response_and_pole_zero(zeros, poles, k=0.05):
    '''
        Plot the pole-zero diagram from a list of zeros and poles.
    '''
    b, a = signal.zpk2tf(zeros, poles, k)

    print('Numerator coefficients:', b)
    print('Denominator coefficients:', a)

    # get a figure/plot
    ax = plt.subplot(121)

    # create the unit circle
    uc = patches.Circle((0, 0), radius=1, fill=True,
                        color='c', ls='dashed')
    ax.add_patch(uc)

    # Plot the zeros and set marker properties
    t1 = plt.plot(zeros.real, zeros.imag, 'go', ms=10, label='zero')
    plt.setp(t1, markersize=10.0, markeredgewidth=1.0,
             markeredgecolor='k', markerfacecolor='g')

    # Plot the poles and set marker properties
    t2 = plt.plot(poles.real, poles.imag, 'rx', ms=10, label='pole')
    plt.setp(t2, markersize=12.0, markeredgewidth=3.0,
             markeredgecolor='r', markerfacecolor='r')

    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    # set the ticks
    r = 1.5
    plt.axis('scaled')
    plt.axis([-r, r, -r, r])
    ticks = [-1, -.5, .5, 1]
    plt.xticks(ticks)
    plt.yticks(ticks)
    plt.legend()

    plt.subplot(1, 2, 2)
    f, freq_response = get_direct_frequency_response(b, a, sample_rate, 1024)
    plot_frequency_response(f, freq_response)