% Calculate the filter coefficients for a 5-tap lowpass filter with a
% cutoff frequency of 800Hz and a sampling rate of 8000 Hz using the
% Fourier transform method and hamming window function
clear;clc
Fc = 800; % cutoff frequency (Hz)
Fs = 8000; % sampling frequency (Hz)
cutoff_freq = 2*pi*Fc/Fs;
tap = 15;
b=zeros(1,tap);
b(round(tap/2))=cutoff_freq/pi;
for k=round(tap/2)+1:tap
    n=k-round(tap/2);
    b(k) = sin(n*cutoff_freq)/(n*pi);
end
for k=1:round(tap/2)-1
    b(k) = b(2*round(tap/2)-k)  %shift h(n) to the right by M samples
end
bw=b'.*hamming(tap);
[hz,w]=freqz(bw,1,512);
phi = 180*unwrap(angle(hz))/pi;
subplot(2,1,1);plot(w,20*log10(abs(hz))),grid;
xlabel('Frequency (radians)');
ylabel('Magnitude (dB)');
subplot(2,1,2);
plot(w,phi);grid
xlabel('Frequency (radians)');
ylabel('Phase (degrees)');
dataLength = 40;
x1=sin(0.05*pi*[0:1:dataLength]);
x2=0.8*sin(0.15*pi*[0:1:dataLength])+0.2*sin(0.93*pi*[0:1:dataLength])+0.1*sin(0.77*pi*[0:1:dataLength]);
filtered_x1 = filter(bw,1,x1);
filtered_x2 = filter(bw,1,x2);
filtered_x2 =filtered_x2';
figure()
subplot(4,1,1);plot(x1,'ko');grid;axis([0 50 -1.04 1.04])
subplot(4,1,2);plot(filtered_x1,'mo');grid;axis([0 50 -1.04 1.04])
subplot(4,1,3);plot(x2,'ko');grid;axis([0 50 -1.04 1.04])
subplot(4,1,4);plot(filtered_x2,'mo');grid;axis([0 50 -1.04 1.04])
fixedPointBw = round( bw * 2^14)  % then copy the coefficients t fixed-point version in C 

