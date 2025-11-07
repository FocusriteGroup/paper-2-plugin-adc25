import numpy as np
class mvmfilter:
    def __init__(self, f, tau, Fs):
        self.x = 0
        self.y = 0
        self.Fs = Fs
        self.x1 = 0
        self.y1 = 0
        self.set(f ,tau)

    def set(self, f, tau):
        r = np.exp(- 1 /(tau * self.Fs))
        theta = 2 * np.pi * f / self.Fs
        self.x1 = r * np.cos(theta)
        self.y1 = r * np.sin(theta)

    def process(self, u):
        x_ = self.x1 * self.x - self.y1 * self.y + u
        self.y = self.y1 * self.x + self.x1 * self.y
        self.x = x_
        return self.y