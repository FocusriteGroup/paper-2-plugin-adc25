import numpy as np
def impulse_signal(duration_in_samples):
    impulse = np.zeros(duration_in_samples)
    impulse[0] = 1
    return impulse