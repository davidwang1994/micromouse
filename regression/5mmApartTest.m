close all; clc; clear;

SFH_4545 = 1;
VSLY_5940 = 2;
emit = [SFH_4545; VSLY_5940];

SFH_309FA4 = 1;
SFH_3100F = 2;
TEFT_4300 = 3;
rec = [SFH_309FA4; SFH_3100F; TEFT_4300]; 

dist = [ 1, 5, 10]; % Measured in cm
resRec = 1000;      % Receiver resistor (Ohms)
resEmit = 20;       % Emitter resistor (Ohms)

V = 1;   % resRec voltage drop (V)
I = 2;   % resEmit current (mA)
On = 1;
Off = 2;


readings = zeros(length(emit), length(rec), length(dist), 2,2);

% 5 cm tests

readings(VSLY_5940,SFH_309FA4,2,On,:) = [3.05, 100];
readings(VSLY_5940,SFH_309FA4,2,Off,:) = [361e-6, 0];

readings(VSLY_5940,TEFT_4300,2,On,:) = [3.21, 100];
readings(VSLY_5940,TEFT_4300,2,Off,:) = [2.4e-3, 0];

readings(VSLY_5940,SFH_3100F,2,On,:) = [0.35, 100];
readings(VSLY_5940,SFH_3100F,2,Off,:) = [2.4e-3, 0];



readings(SFH_4545,SFH_309FA4,2,On,:) = [3.16, 112];
readings(SFH_4545,SFH_309FA4,2,Off,:) = [342e-6, 0];

readings(SFH_4545,TEFT_4300,2,On,:) = [3.22, 112];
readings(SFH_4545,TEFT_4300,2,Off,:) = [2.5e-3, 0];

readings(SFH_4545,SFH_3100F,2,On,:) = [0.76, 112];
readings(SFH_4545,SFH_3100F,2,Off,:) = [0, 0];
