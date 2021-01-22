import matplotlib.pyplot as plt
import numpy as np
import csv
import scipy.special as sc

def binary_entropy(x):
    return -(sc.xlogy(x, x) + sc.xlog1py(1 - x, -x))/np.log(2)

plt.rcParams.update({'font.size': 20})
with open('dcache', newline='\n') as csvfile:
    ic = list(csv.reader(csvfile))
    icx = [float(x[0]) for x in ic]
    icy = [float(x[1]) for x in ic]

icap = []
for i in range(len(icx)):
    cap = icx[i] * (1 - binary_entropy(icy[i]))
    icap.append(cap)

#fig, ax2 = plt.subplots(1,1,figsize=(15,5))
file = 'bitrate.pdf'
#fig.suptitle('Error rate - Throughput')
#fig.text(0.99, 0.5, 'Capacity (bps)', va='center', rotation='vertical', fontsize=30)
plt.plot(icx, icy, 'o', color='black')
plt.plot(np.unique(icx), np.poly1d(np.polyfit(icx, icy, 6))(np.unique(icx)), color='black')
plt.title('(b) D-Cache', fontsize=30)
plt.xlabel('bit rate (bps)')
plt.ylabel('Bit Error Probability')
#plt.ylim(0,0.4)
#plt.yaxis.grid()
#plt.yaxis.set_major_locator(plt.MaxNLocator(2))
plt.tight_layout()
plt.show()
#plt.savefig(file, bbox_inches='tight')
