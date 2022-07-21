# -*- coding: utf-8 -*-
"""Untitled0.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1rHb4Lc34qNFi_UMdZsqSpd7tu7t2XpBy
"""

import numpy as np
import sys
from scipy import signal
from scipy.io import wavfile
from scipy.io.wavfile import write
import soundfile
import matplotlib.pyplot as plt
from scipy.signal import spectrogram, lfilter, freqz, filtfilt, tf2zpk

######## 4.1. ########
sg, fs = soundfile.read('audio/xlogin00.wav')
signal_length = len(sg)
time_length = signal_length/fs

min_value = min(sg)
max_value = max(sg)

print("The sampling frequency of the signal xlogin00.wav is: ", fs)
print("The signal length of xlogin00.wav in seconds is:  ", time_length)
print("Min value is: ", min_value)
print("Max value is: ", max_value)

plt.figure(figsize=(6,3))
plt.plot(np.arange(0, time_length, time_length/signal_length), sg)
plt.xlabel('Time [s]')
plt.title('xhunda01.wav')
plt.tight_layout()
plt.show
plt.savefig('xlogin00.jpg')

######## 4.2. ########
sg = sg - np.mean(sg)
sg = sg / max(np.abs(sg))

frames = []
length = int(round(signal_length/1024))
framework = 1024
shift = 512
for i in range(length) :
  frames.append(sg[i*shift:i*shift+framework])

plt.figure(figsize=(6,3))
plt.plot(np.arange(0, 1024/fs, 1/fs), frames[21])
plt.xlabel('Time [s]')
plt.title('xlogin00.wav - segment 21')
plt.tight_layout()
plt.show
plt.savefig('xlogin00.wav - segment 21.jpg')

######## 4.3. ########
spec = np.fft.fft(frames[21])
spec_axis = np.arange(0, fs, fs/len(spec))

plt.plot(spec_axis[:len(spec)//2], np.abs(spec[:len(spec)//2]))
plt.xlabel('Frequency [Hz]')
plt.title('segment 21 - spectral analysis')
plt.tight_layout()
plt.savefig('dft_by_fft.jpg')

#my own
x = frames[21]
N = len(x)
dft = np.zeros((N, N), dtype=np.complex128)
for n in range(N):
  for k in range(N):
    dft[k, n] = np.exp(-2j * np.pi * k * n / N)

x = np.dot(dft, x)
spec_axis_x = np.arange(0, fs, fs/len(x))

plt.title('segment 21 - spectral analysis own implementation')
plt.plot(spec_axis_x[:len(x)//2], np.abs(x[:len(x)//2]), color = 'purple')
plt.xlabel('Frequency [Hz]')
plt.tight_layout()
plt.savefig('dft_own.jpg')

######## 4.4. ########
f, t, sgr = spectrogram(sg, fs)
sgr_log = 10 * np.log10(sgr+1e-20) 
plt.figure(figsize=(9,3))
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_xlabel('Time [s]')
plt.gca().set_ylabel('Frequency [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spectral power density[dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.savefig('spectogram.jpg')

######## 4.5. ########
f1 = 968.75 
f2 = f1 * 2
f3 = f1 * 3
f4 = f1 * 4

######## 4.6. ########
freq = [f1, f2, f3, f4]
x = np.arange(0, signal_length)
y = 0
for i in range(4) :
  y += np.cos(freq[i]/fs*2*np.pi*x)

f, t, sgr = spectrogram(y, fs, nperseg=1024, noverlap=512 )
sgr_log = 10 * np.log10(sgr+1e-20)

plt.figure(figsize=(9,3))
plt.pcolormesh(t, f, sgr_log)
plt.xlabel('Time [s]')
plt.ylabel('Frequency [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spectral power density[dB]', rotation=270, labelpad=15)
plt.tight_layout()

new = wavfile.write('audio/4cos.wav', fs, y)
plt.savefig('4cos.jpg')

######## 4.7. ########
N_imp = 32
imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls

#prvy signal
N, wn = signal.buttord([f1-50, f1+50], [f1-15, f1+15], 3, 40, False, fs)
b1, a1 = signal.butter(N, wn, 'bandstop', False, 'ba', fs)
w1, H1 = signal.freqz(b1, a1)

h = signal.lfilter(b1, a1, imp)

plt.figure(figsize=(10,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_title('Impulse response f1 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.savefig('Impulse_responsef1.jpg')

#druhy signal
N, wn = signal.buttord([f2-50, f2+50], [f2-15, f2+15], 3, 40, False, fs)
b2, a2 = signal.butter(N, wn, 'bandstop', False, 'ba', fs)
w2, H2 = signal.freqz(b2, a2)

h = signal.lfilter(b2, a2, imp)

plt.figure(figsize=(10,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_title('Impulse response f2 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.savefig('Impulse_responsef2.jpg')

#treti signal
N, wn = signal.buttord([f3-50, f3+50], [f3-15, f3+15], 3, 40, False, fs)
b3, a3 = signal.butter(N, wn, 'bandstop', False, 'ba', fs)
w3, H3 = signal.freqz(b3, a3)

h = signal.lfilter(b3, a3, imp)

plt.figure(figsize=(10,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_title('Impulse response f3 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.savefig('Impulse_responsef3.jpg')

#stvrty signal
N, wn = signal.buttord([f4-50, f4+50], [f4-15, f4+15], 3, 40, False, fs)
b4, a4 = signal.butter(N, wn, 'bandstop', False, 'ba', fs)
w4, H4 = signal.freqz(b4, a4)

h = signal.lfilter(b4, a4, imp)

plt.figure(figsize=(10,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_title('Impulse response f4 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.savefig('Impulse_responsef4.jpg')

######## 4.8. ########
#nuly, poly
convole_a = 1
convole_b = 1
a = [a1, a2, a3, a4]
b = [b1, b2, b3, b4]
for i in range(4):
  convole_a = np.convolve(convole_a, a[i])
  convole_b = np.convolve(convole_b, b[i])

z, p, k = signal.tf2zpk(convole_b, convole_a) 

plt.figure(figsize=(4,3.5))
#jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='zeroes')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='poles')

plt.gca().set_xlabel('Real part $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginary part $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig('poles_and_zeros.jpg')

######## 4.9. ########
#prvy filter
_, ax = plt.subplots(1, 2, figsize=(8, 3))

ax[0].plot(w1 / 2 / np.pi * fs, np.abs(H1))
ax[0].set_xlabel('Frequency [Hz]')
ax[0].set_title('Modulus of filter f1 response $|H(e^{j\omega})|$')

ax[1].plot(w1 / 2 / np.pi * fs, np.angle(H1))
ax[1].set_xlabel('Frequency [Hz]')
ax[1].set_title('Argument of filter f1 response $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig('freq_charf1.jpg')

#druhy filter
_, ax = plt.subplots(1, 2, figsize=(8, 3))

ax[0].plot(w2 / 2 / np.pi * fs, np.abs(H2))
ax[0].set_xlabel('Frequency [Hz]')
ax[0].set_title('Modulus of filter f2 response $|H(e^{j\omega})|$')

ax[1].plot(w2 / 2 / np.pi * fs, np.angle(H2))
ax[1].set_xlabel('Frequency [Hz]')
ax[1].set_title('Argument of filter f2 response $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig('freq_charf2.jpg')

#treti filter
_, ax = plt.subplots(1, 2, figsize=(8, 3))

ax[0].plot(w3 / 2 / np.pi * fs, np.abs(H3))
ax[0].set_xlabel('Frequency [Hz]')
ax[0].set_title('Modulus of filter f3 response $|H(e^{j\omega})|$')

ax[1].plot(w3 / 2 / np.pi * fs, np.angle(H3))
ax[1].set_xlabel('Frequency [Hz]')
ax[1].set_title('Argument of filter f3 response $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig('freq_charf3.jpg')

#stvrty filter
_, ax = plt.subplots(1, 2, figsize=(8, 3))

ax[0].plot(w4 / 2 / np.pi * fs, np.abs(H4))
ax[0].set_xlabel('Frequency [Hz]')
ax[0].set_title('Modulus of filter f4 response $|H(e^{j\omega})|$')

ax[1].plot(w4 / 2 / np.pi * fs, np.angle(H4))
ax[1].set_xlabel('Frequency [Hz]')
ax[1].set_title('Argument of filter f4 response $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig('freq_charf4.jpg')

######## 4.10. ########
filtered = signal.filtfilt(convole_b, convole_a, sg)
#normalizacia do dynamickeho rozsahu
filtered = filtered - np.mean(filtered)
filtered = filtered / max(np.abs(filtered))

f, t, spektrogram = spectrogram(filtered, fs)
X_log = 10 * np.log10(spektrogram+1e-20)
plt.figure(figsize=(15,5))
plt.pcolormesh(t,f,X_log)
plt.title('Filtred signal')
plt.gca().set_xlabel('Time [s]')
plt.gca().set_ylabel('Frequency [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()

new = write('audio/clean_bandstop.wav', fs, np.real(filtered))
plt.savefig('filtered_signal.jpg')