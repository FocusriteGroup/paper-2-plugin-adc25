# Part 1: Warm Up

```python
# Draw a sine wave given
# - a sample rate
# - a frequency
# - a duration

sample_rate = 48000 # samples/second
frequency = 4.0 # Hz
duration = 1.0 # Seconds

# the number of samples will be sample_rate * duration
N = int(np.ceil(sample_rate * duration))

# You can create an empty array in numpy with numpy.zeros()
signal = np.zeros(N)

# The phase increment every sample is going to be
increment = frequency / sample_rate

for ndx in range(0, N):
    signal[ndx] = np.cos(2 * np.pi * increment * ndx)

plt.figure(1)
plt.clf()
plt.plot(signal)
plt.grid()
plt.show()
```

# Building a filter function
## Filters 1

Pass-through filter.

```python
class FilterClass:
        
    def process(self, inputValue):
        outputValue = inputValue
        return outputValue
```

## Filters 2

Delay by one sample.

```python
class FilterClass:

    def __init__(self):
        self.state = 0
        
    def process(self, inputValue):
        outputValue = self.state
        self.state = inputValue
        return outputValue

zm1 = FilterClass()
```

## Filters 3

Delay and add to dry signal.

```Python
class FilterClass:

    def __init__(self):
        self.state = 0
        
    def process(self, inputValue):
        outputValue = self.state + inputValue
        self.state = inputValue
        return outputValue

```

## Filters 4

```python
class FilterClass:

    def __init__(self, b1):
        self.state = 0
        self.b1 = b1
        
    def process(self, inputValue):
        outputValue = self.b1 * self.state + inputValue
        self.state = inputValue
        return outputValue
```

## Filters 5

```python
class FilterClass:

    def __init__(self, b0, b1):
        self.state = 0
        self.b0 = b0
        self.b1 = b1
        
    def process(self, inputValue):
        outputValue = self.b1 * self.state + self.b0 * inputValue
        self.state = inputValue
        return outputValue
```

## Filters 6

```python
class FilterClass:

    def __init__(self, b0, b1, b2):
        self.xzm1 = 0
        self.xzm2 = 0
        self.b0 = b0
        self.b1 = b1
        self.b2 = b2
        
    def process(self, inputValue):
        outputValue = self.b2 * self.xzm2 + self.b1 * self.xzm1 + self.b0 * inputValue
        self.xzm2 = self.xzm1
        self.xzm1 = inputValue
        return outputValue
```

## Complex Coefficients

```python
amplitude = 1.0
frequency = np.pi / 4
pole = -amplitude * np.exp(1j * frequency)

# Filters 4
class FilterClass:

    def __init__(self, b1):
        self.state = 0
        self.b1 = b1
        
    def process(self, inputValue):
        outputValue = self.b1 * self.state + inputValue
        self.state = inputValue
        return outputValue

analyse_filter_complex(FilterClass(pole))
```

```python
b0 = 1;
b1 = 2 * np.real(pole)
b2 = pole * np.conj(pole)
```

## Filters 7

```python
class FilterClass:

    def __init__(self):
        self.yzm1 = 0

    def process(self, inputValue):
        outputValue = inputValue - self.yzm1
        self.yzm1 = outputValue
        return outputValue
```

## Filters 8

```python
class FilterClass:

    def __init__(self, a1):
        self.yzm1 = 0
        self.a1 = a1

    def process(self, inputValue):
        outputValue = inputValue - self.a1 * self.yzm1
        self.yzm1 = outputValue
        return outputValue
```

## Filters 9

```python
class FilterClass:

    def __init__(self, b0, a1, a2):
        self.yzm1 = 0
        self.yzm2 = 0
        self.b0 = b0
        self.a1 = a1
        self.a2 = a2

    def process(self, inputValue):
        outputValue = self.b0 * inputValue - self.a1 * self.yzm1 - self.a2* self.yzm2
        self.yzm2 = self.yzm1
        self.yzm1 = outputValue
        return outputValue
```

## Filters 10

The full Biquad filter. 

```python
class FilterClass:

    def __init__(self, b0, b1, b2, a1, a2):
        self.yzm1 = 0
        self.yzm2 = 0
        self.xzm1 = 0
        self.xzm2 = 0
        self.b0 = b0
        self.b1 = b1
        self.b2 = b2
        self.a1 = a1
        self.a2 = a2

    def process(self, inputValue):
        outputValue = self.b2 * self.xzm2 + self.b1 * self.xzm1 + self.b0 * inputValue  - self.a1 * self.yzm1 - self.a2 * self.yzm2
        self.yzm2 = self.yzm1
        self.yzm1 = outputValue
        self.xzm2 = self.xzm1
        self.xzm1 = inputValue
        return outputValue
```

## The paper filter

```python
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
```


## Further filters

https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

This is the Bandpass Filter (constant skirt gain, peak gain = Q) filter

```python
class FilterClass:

    def __init__(self, omega, Q):
        self.yzm1 = 0
        self.yzm2 = 0
        self.xzm1 = 0
        self.xzm2 = 0

        self.b0 = 0
        self.b1 = 0
        self.b2 = 0
        self.a1 = 0
        self.a2 = 0

        self.set(omega, Q)

    def set(self, omega, Q):
        Q_alpha = np.sin(omega / 2)
        alpha = Q_alpha / Q
        
        self.b0 = Q_alpha
        self.b1 = 0
        self.b2 = -Q_alpha
        a0 = 1 + alpha
        self.a1 = -2 * np.cos(omega) / a0
        self.a2 = 1 - alpha / a0

    def process(self, inputValue):
        outputValue = self.b2 * self.xzm2 + self.b1 * self.xzm1 + self.b0 * inputValue  - self.a1 * self.yzm1 - self.a2 * self.yzm2
        self.yzm2 = self.yzm1
        self.yzm1 = outputValue
        self.xzm2 = self.xzm1
        self.xzm1 = inputValue
        return outputValue
```

# C++ implementation of the filter class
```C++
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
```
