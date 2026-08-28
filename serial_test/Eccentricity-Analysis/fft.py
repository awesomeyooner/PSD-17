import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import find_peaks

DATA_FILE = "data6"
FILE_EXT = ".csv"

data = pd.read_csv(DATA_FILE + FILE_EXT)

time = data['Normalized Time'].to_numpy()
residual = data['Residual'].to_numpy()

num_samples = len(time)
total_time = time[-1] - time[0]
period = total_time / num_samples
frequency = 1 / period

fft_output = np.fft.rfft(residual)
frequencies = np.fft.rfftfreq(num_samples, d=period)

phases = np.angle(fft_output)

amplitude = np.abs(fft_output) / num_samples
amplitude[1:] = amplitude[1:] * 2

# Output Amplitude and Frequency to CSV
# output_data = pd.DataFrame({
#     'frequency_hz': frequencies,
#     'amplitude': amplitude
# })

# output_data.to_csv("fft_results.csv", index=False)

peak_idxs, properties = find_peaks(amplitude, threshold=0.0001)

frequency_peaks = []
amplitude_peaks = []
phase_peaks = []

for peak_idx in peak_idxs:
    x = frequencies[peak_idx]
    y = amplitude[peak_idx]
    phase = phases[peak_idx]

    frequency_peaks.append(x)
    amplitude_peaks.append(y)
    phase_peaks.append(phase)

peak_csv = pd.DataFrame({
    "Frequency (Hz)": frequency_peaks,
    "Amplitude": amplitude_peaks,
    "Phase": phase_peaks
}).to_csv(DATA_FILE + "_peaks.csv", index=False)

print("Frequency data saved!")

constructed_x = []
constructed_y = []

for t in time:

    constructed_x.append(t)

    sum = 0

    for p_i in range(len(peak_idxs)):
        f = frequency_peaks[p_i]
        amp = amplitude_peaks[p_i]
        phase = phase_peaks[p_i]

        sum += amp * math.sin(2 * math.pi * f * t + phase)

    constructed_y.append(sum)


fig, axs = plt.subplots(1, 2, figsize=(10, 4))

axs[0].plot(frequencies, amplitude, color='blue', linewidth=1.5)

for peak_idx in peak_idxs:
    x = frequencies[peak_idx]
    y = amplitude[peak_idx]

    axs[0].plot(x, y, "x")

axs[0].set_title("Frequency Spectrum")
axs[0].set_xlabel("Frequency (Hz)")
axs[0].set_ylabel("Amplitude")
axs[0].grid(True, linestyle='--', alpha=0.6)

axs[1].plot(constructed_x, constructed_y, color='red', linewidth=1.5)
axs[1].set_title("Reconstructed Noise")
axs[1].set_xlabel("Time (seconds)")
axs[1].set_ylabel("Angle (rads)")
axs[1].grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show()

# plt.figure(figsize=(10, 4))
# plt.plot(frequencies, amplitude, color='blue', linewidth=1.5)

# for peak_idx in peak_idxs:
#     x = frequencies[peak_idx]
#     y = amplitude[peak_idx]

#     plt.plot(x, y, "x")

# plt.title("Frequency Spectrum")
# plt.xlabel("Frequency (Hz)")
# plt.ylabel("Amplitude")
# plt.grid(True, linestyle='--', alpha=0.6)
# plt.show()