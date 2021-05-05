clc;clear
% A lowpass FIR filter with 25 taps
% The cutoff frequency of the filter is 2,000 Hz, assuming a sampling
% frequency of 8000 Hz. 
Fs=8000;
N=25; Ftype = 1; WnL=0.5*pi; WnH=0; Wtype=1;
Brec=firwd(N,Ftype,WnL,WnH,1);
Bham=firwd(N,Ftype,WnL,WnH,4);
B_black=firwd(N,Ftype,WnL,WnH,5);
[h_rec,f]=freqz(Brec,1,512,Fs);
[h_ham,f]=freqz(Bham,1,512,Fs);
[h_black,f]=freqz(B_black,1,512,Fs);
p_rec = 180*unwrap(angle(h_rec))/pi;
p_ham = 180*unwrap(angle(h_ham))/pi;
p_black = 180*unwrap(angle(h_black))/pi;
figure()
subplot(2,1,1)
plot(f,20*log10(abs(h_rec)),'k','LineWidth',1.2);grid;
hold on;plot(f,20*log10(abs(h_ham)),'r-.','LineWidth',1.2);
plot(f,20*log10(abs(h_black)),'LineWidth',1.2);
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
legend({'rectangular window','Hamming window','Blackman'},'FontSize',12,'location','southwest')
subplot(2,1,2)
plot(f,p_rec,'k','LineWidth',1.2);grid;
hold on;plot(f,p_ham,'r-.','LineWidth',1.2);
plot(f,p_black,'LineWidth',1.2);
xlabel('Frequency (Hz)');
ylabel('Phase (deg.)');
legend({'rectangular window','Hamming window','Blackman'},'FontSize',12,'location','southwest')