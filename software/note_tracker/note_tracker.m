% Tracks notes by performing note estimation then
% comparing notes to sheet music
clear;

C4 = 1;
CS4 = 2;
D4 = 3;
Eb4 = 4;
E4 = 5;
F4 = 6;
FS4 = 7;
G4 = 8;
GS4 = 9;
A4 = 10;
Bb4 = 11;
B4 = 12;
C5 = 13;
CS5 = 14;
D5 = 15;
Eb5 = 16;
E5 = 17;

notes = {'C4' 'CS4' 'D4' 'Eb4' 'E4' 'F4' 'FS4' 'G4' 'GS4' 'A4' 'Bb4' 'B4', 'C5', 'CS5', 'D5', 'Eb6', 'E6'};
f = [261.6 277.2 293.7 311.1 328.6 349.2 370.0 392.0 415.4 440.0 466.2 493.9 523.3 554.4 587.3 622.3 659.3];
N_Notes = length(f);

N = 2048; % Order of filter
M = 256; % Keep track of last 256 samples
a = 0.999;
%%

Fs = 16000;
nBits = 8;
nChannels = 1;
duration = 30;
period = 0.1;

%%

rec = audiorecorder(Fs, nBits, nChannels);
time = 0;
sample_no = 0; % Current sample

L = duration * Fs;

N_Window = 10;
window_no = 0;

comb = zeros(1, L);
res = zeros(N_Notes, L);
powers = zeros(N_Notes, L);
c_power = zeros(1, N_Notes);

%%
% Clocks- Coldplay
disp('Clocks- Coldplay');
song = [Eb5 Bb4 G4 Eb5 Bb4 G4 Eb5 Bb4 CS5 Bb4 F4 CS5 Bb4 F4 CS5 Bb4 CS5 Bb4 F4 CS5 Bb4 F4 CS5 Bb4 C5 GS4 F4 C5 GS4 F4 C5 GS4];
% disp('Twinkle Twinkle Little Star');
% song = [D4 D4 A4 A4 B4 B4 A4 G4 G4 FS4 FS4 E4 E4 D4];
s = 16;
n = 1;

%disp(notes(song));

t = min(n + s - 1, length(song));
str = char(zeros(1, 5 * (t - n + 1)));
for i = n:t
   str((5 * (i - 1) + 1):5 * (i - 1) + length(char(notes(song(i))))) = char(notes(song(i)));
end

%%
point = char(zeros(1, 5 * (t - n + 1)));
point1 = point;
point1(5 * mod(n - 1, s) + 1) = '^';

%%
% figure;

disp('Begin playing');
disp(str);
disp(point1);

record(rec, duration);

while sample_no < L && n <= length(song)
    pause(period);
    y = getaudiodata(rec);
    
    for k = (sample_no + 1): length(y)
        comb(k) = y(k);
        if k > N
            comb(k) = comb(k) - a^N * y(k-N);
        end
        comb(k) = comb(k) / N;
    end
    
    for i = 1:N_Notes
        ni = round(f(i) / Fs * N);
        for k = (sample_no + 1): length(y)
            if k > 1
                res(i, k) = -2*comb(k-1) + 2*a*cos(2*pi*ni/N) * res(i, k-1);
                if k > 2
                    res(i, k) = res(i, k) - 2*a*cos(2*pi*ni/N) * comb(k-2) - a^2 * res(i, k-2);
                end
            end
            c_power(i) = c_power(i) + res(i, k)^2;
            if (k > M)
                c_power(i) = c_power(i) - res(i, k - M) ^ 2;
                powers(i, k) = c_power(i);
            else
                powers(i, k) = M / k * c_power(i);
            end
            powers(i, k) = powers(i, k) * (f(A4)/f(i));
        end
    end
            
    sample_no = length(y);
    
    for k = (window_no + 1):floor(sample_no/N_Window)
        p = mean(powers(:,(window_no * N_Window + 1):((window_no + 1) * N_Window)), 2);
        [m, i] = max(p);
        if (m > 1)
            % disp(notes(i));
            % disp(song(n));
            while n <= length(song) && (mod(song(n),12) == mod(i, 12))
                n = n + 1;
                if (n <= t)
                    disp(str);
                    point1 = point;
                    point1(5 * mod(n - 1, s) + 1) = '^';
                    disp(point1);
                elseif (n <= length(song))
                    t = min(n + s - 1, length(song));
                    str = char(zeros(1, 5 * (t - n + 1)));
                    for i = n:t
                       str((5 * mod((i - 1), s) + 1):5 * mod((i - 1), s) + length(char(notes(song(i))))) = char(notes(song(i)));
                    end
                    disp(str);
                    point = char(zeros(1, 5 * (t - n + 1)));
                    point1 = point;
                    point1(5 * mod(n - 1, s) + 1) = '^';
                    disp(point1);
                end
            end
        else
            last = -1;
        end
    end
    window_no = floor(sample_no/N_Window);
    
    
%     for i = 1:N_Notes
%        subplot(N_Notes, 1, i);
%        plot(powers(i, 1:sample_no));
% 
%        title(notes(i));
%     end
    
end

stop(rec);
disp('Recording has ended');

%plot(y);
