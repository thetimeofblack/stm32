fs = 44100 ;
T = 1/fs ;
time  = [3,3,3];
time1 = time(1);
time2 = time(2);
time3 = time(3);
freq = [261.62557,288,320];
freq1 = freq(1);
freq2 = freq(2);
freq3 = freq(3);
volume = [80,80,80];
vol1 = volume(1);
vol2 = volume(2);
vol3 = volume(3);
tone1 = sin(2*pi*freq1*(0:T:time1))*(db2mag(vol1));
tone2 = sin(2*pi*freq2*(0:T:time2))*(db2mag(vol2));
tone3 = sin(2*pi*freq3*(0:T:time3))*(db2mag(vol3));
Tone = [tone1,tone2,tone3];
sound(Tone,fs)
filename = ('test.wav');
audiowrite(filename,Tone,fs)
