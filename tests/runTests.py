import numpy as np
import subprocess as subp

import matplotlib.pyplot as plt

import os
import platform
import sys

# Add filter_design_analysis to sys.path to access mvmfilter
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)

python_library_dir = os.path.join(parent_dir, 'notebooks', 'filter_design_analysis')

if python_library_dir not in sys.path:
    sys.path.append(python_library_dir)

import filter

class TestPipe:
    def __init__(self, cmd, dtype):
        self.cmd = cmd
        self.dtype = dtype
    def run(self, data):
        testProcess = subp.Popen(self.cmd, stdin=subp.PIPE, stdout=subp.PIPE)
        testOutput = testProcess.communicate(data.tobytes())
        result = np.frombuffer(testOutput[0], self.dtype)
        result = result[0:len(data)]
        return result

def signal_rms(signal):
    return np.sqrt(np.mean(signal * np.conj(signal)))

def signal_power(signal):
    rms = signal_rms(signal)
    if rms == 0:
        rms = np.finfo(np.float64).eps
    return 20 * np.log10(rms)
    
if __name__ == '__main__':
    # Define input impulse signal
    test_signal = np.zeros(128)
    test_signal[0] = 1

    # Run C++ implementation
    os_name = platform.system()
    if os_name == "Windows":
        executable_name = 'MVMFilterTests.exe'
    else:
        executable_name = os.path.join(current_dir, 'MVMFilterTests')

    test = TestPipe([executable_name], np.float32)
    result_cpp = test.run(test_signal.astype(np.float32))

    # Run Python implementation
    fs = 48000
    mvm_filter = filter.mvmfilter(1000, 0.01, fs)
    result_python = []
    for sample in test_signal:
        result_python.append(mvm_filter.process(sample))

    # Compare results from both implementation
    diff = result_cpp - result_python

    mean_level_error = signal_power(diff)

    if mean_level_error > -120:
        print("TEST FAIL with mean level error =" + str(mean_level_error) + " dB")
    else:
        print("TEST PASS with mean level error =" + str(mean_level_error) + " dB")

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 6), layout='constrained')

    ax1.plot(result_cpp)
    ax1.set_xlabel('Sample')
    ax1.set_ylabel('Magnitude')
    ax1.set_title('C++ implementation')
    ax1.grid()

    ax2.plot(result_python)
    ax2.set_xlabel('Sample')
    ax2.set_ylabel('Magnitude')
    ax2.set_title('Python implementation')
    ax2.grid()

    ax3.plot(diff)
    ax3.set_xlabel('Sample')
    ax3.set_ylabel('Magnitude')
    ax3.set_title('Difference')
    ax3.grid()

    plt.show()