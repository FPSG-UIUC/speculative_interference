import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.special as sc

def binary_entropy(x):
    return -(sc.xlogy(x, x) + sc.xlog1py(1 - x, -x))/np.log(2)

plt.rcParams.update({'font.size': 20})
with open('dcache', newline='\n') as csvfile:
    dc = list(csv.reader(csvfile))
    dcx = [float(x[0]) for x in dc]
    dcy = [float(x[1]) for x in dc]
with open('icache', newline='\n') as csvfile:
    ic = list(csv.reader(csvfile))
    icx = [float(x[0]) for x in ic]
    icy = [float(x[1]) for x in ic]

dcap = []
for i in range(len(dcx)):
    cap = dcx[i] * (1 - binary_entropy(dcy[i]))
    dcap.append(cap)
icap = []
for i in range(len(icx)):
    cap = icx[i] * (1 - binary_entropy(icy[i]))
    icap.append(cap)

type='B'
if type == 'H':
    fig, (ax1,ax2) = plt.subplots(2,1,figsize=(14,6))
    file = 'cap-h.pdf'
elif type == 'S':
    fig, (ax1,ax2) = plt.subplots(1,2,figsize=(15,5))
    file = 'cap.pdf'
elif type == 'B':
    fig, (ax1,ax2) = plt.subplots(1,2,figsize=(15,5))
    file = 'bitrate.pdf'
#fig.suptitle('Error rate - Throughput')
ax1.plot(dcx, dcy, 'o', color='black')
ax1.plot(np.unique(dcx), np.poly1d(np.polyfit(dcx, dcy, 6))(np.unique(dcx)), color='black')
ax1.set_title('(a) D-Cache', fontsize=30)
#ax11 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
if type == 'H':
    fig.text(-0.01, 0.5, 'Bit Error Probability', va='center', rotation='vertical', fontsize=30)
    fig.text(0.99, 0.5, 'Capacity (bps)', va='center', rotation='vertical', fontsize=30)
elif type == 'S':
    ax1.set_xlabel('bit rate (bps)', fontsize=30)
    ax1.set_ylabel('Bit Error Probability', fontsize=30)
    ax11.set_ylabel('Capacity (bps)', fontsize=30)
else:
    ax1.set_xlabel('bit rate (bps)', fontsize=30)
    ax1.set_ylabel('Bit Error Probability', fontsize=30)
    ax1.yaxis.grid()
#ax11.plot(dcx, dcap, 'o', color='red')
#ax11.plot(np.unique(dcx), np.poly1d(np.polyfit(dcx, dcap, 6))(np.unique(dcx)), color='red')
ax1.set_ylim(0,0.4)
ax1.yaxis.set_major_locator(plt.MaxNLocator(2))
ax2.plot(icx, icy, 'o', color='black')
ax2.plot(np.unique(icx), np.poly1d(np.polyfit(icx, icy, 6))(np.unique(icx)), color='black')
ax2.set_title('(b) I-Cache', fontsize=30)
ax2.set_xlabel('bit rate (bps)', fontsize=30)
#ax21 = ax2.twinx()  # instantiate a second axes that shares the same x-axis
if type == 'S':
    ax2.set_ylabel('Bit Error Probability', fontsize=30)
    ax21.set_ylabel('Capacity (bps)', fontsize=30)
#ax21.plot(icx, icap, 'o', color='red')
#ax21.plot(np.unique(icx), np.poly1d(np.polyfit(icx, icap, 6))(np.unique(icx)), color='red')
ax2.set_ylim(0,0.4)
ax2.set_yticklabels([])
ax2.yaxis.grid()
ax2.yaxis.set_major_locator(plt.MaxNLocator(2))
plt.tight_layout()
#plt.show()
plt.savefig(file, bbox_inches='tight')
