import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import find_peaks

DATA_FILE = "data9"
FILE_EXT = ".csv"

data = pd.read_csv(DATA_FILE + FILE_EXT)

time = data['Normalized Time'].to_numpy()
residual = data['Residual'].to_numpy()
readings = data['Encoder Angle (Radians)'].to_numpy()

num_samples = len(readings)
total_time = readings[-1] - readings[0]
period = total_time / num_samples
frequency = 1 / period

fft_output = np.fft.rfft(residual)
frequencies = np.fft.rfftfreq(num_samples, d=period)

phases = np.angle(fft_output)

amplitude = np.abs(fft_output) / num_samples

if num_samples % 2 == 0:
    amplitude[1:-1] *= 2
else:
    amplitude[1:] *= 2

peak_idxs, properties = find_peaks(amplitude, threshold=0.000075)

frequency_peaks = []
amplitude_peaks = []
phase_peaks = []

for peak_idx in peak_idxs:
    f = frequencies[peak_idx]
    amp = amplitude[peak_idx]
    phase = phases[peak_idx]

    frequency_peaks.append(f)
    amplitude_peaks.append(amp)
    phase_peaks.append(phase)

peak_csv = pd.DataFrame({
    "Frequency (Hz)": frequency_peaks,
    "Amplitude": amplitude_peaks,
    "Phase": phase_peaks
}).to_csv(DATA_FILE + "_peaks.csv", index=False)

print("Frequency data saved!")

reconstructed = []

for i in range(len(time)):

    theta = readings[i]

    sum = 0

    for p_i in range(len(peak_idxs)):
        
        f = frequency_peaks[p_i]
        n = round(2 * math.pi * f)
        amp = amplitude_peaks[p_i]
        phase = phase_peaks[p_i]

        sum += amp * math.cos(n * (theta + readings[0]) + (phase))

    reconstructed.append(sum)

diff_y = []

for i in range(len(time)):
    raw = residual[i]
    predict = reconstructed[i]

    diff_y.append(raw - predict)


fig1, ax1 = plt.subplots(figsize=(10, 4))

ax1.plot(frequencies, amplitude, color='blue', linewidth=1.5)

for peak_idx in peak_idxs:
    x = frequencies[peak_idx]
    y = amplitude[peak_idx]

    ax1.plot(x, y, "x")

ax1.set_title("Frequency Spectrum")
ax1.set_xlabel("Frequency (Hz)")
ax1.set_ylabel("Amplitude")
ax1.grid(True, linestyle='--', alpha=0.6)

fig2, ax2 = plt.subplots(figsize=(10, 4))

ax2.plot(readings, reconstructed, color='red', linewidth=1.5)
ax2.plot(readings, residual, color='green', linewidth=1.5)
# ax2.plot(readings, diff_y, color='purple', linewidth=1.5)
ax2.set_title("Reconstructed Noise")
ax2.set_xlabel("Encoder Angle (Radians)")
ax2.set_ylabel("Error (Radians)")
ax2.grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show()