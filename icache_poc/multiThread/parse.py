import sys
import os

f = open('out','r')
new_expt=False
loopln=False
it=0
loop=0
period=0
accuracy=0
run=3
data={}
for line in f.readlines():
    if 'Running with training loop' in line:
        new_expt=True
        continue
    if not new_expt:
        if not loopln:
            loop = int(line)
            loopln=True
            continue
        else:
            to = int(line)
            loopln=False
        continue
    if new_expt:
        if 'ones' in line:
            continue
        if 'Period' in line:
            period = int(line.split(': ')[1])
            continue
        if 'Sender finished' in line:
            continue
        if 'accuracy' in line:
            accuracy = int(line.split()[4].split('/')[0])
            bits = int(line.split()[4].split('/')[1])
            #print('iter:{},loop:{},run:{},period:{},accuracy:{}'.format(it,loop,run,period,accuracy))
            if loop not in data:
                data.update({loop:{run:{'period':period, 'accuracy':accuracy}, 'bits':bits}})
            else:
                data[loop].update({run:{'period':period, 'accuracy':accuracy}})
            continue
        if 'Error' in line:
            run == 3
            new_expt=False
            continue
        run = int(line)+1
        if run == 3:
            new_expt=False
        #print(run)
        #print(data)
for loop in data:
    p=[0,0,0]
    a=[0,0,0]
    bits = data[loop]['bits']
    freq = 4000000000
    if 1 in data[loop]:
        p[0] = data[loop][1]['period']
        a[0] = data[loop][1]['accuracy']
    if 2 in data[loop]:
        p[1] = data[loop][2]['period']
        a[1] = data[loop][2]['accuracy']
    if 3 in data[loop]:
        p[2] = data[loop][3]['period']
        a[2] = data[loop][3]['accuracy']
    print('{},{}'.format((3*freq)/(p[0]+p[1]+p[2]), (a[0]+a[1]+a[2])/(3*bits)))
