import matplotlib.pyplot as plt

import numpy as np
import csv

contention2 = np.loadtxt('raw.csv', delimiter=',', unpack=True, skiprows=1, max_rows=1)
baseline2 = np.loadtxt('raw.csv', delimiter=',', unpack=True, skiprows=0, max_rows=1)

bs = np.arange(contention2.min(),contention2.max()+2)
br = np.arange(baseline2.min(),baseline2.max()+2)

#n, bins, patches = plt.hist(x=baseline1[baseline1 != 0], bins='auto', alpha=0.7, rwidth=0.85, label='baseline1', color='darkgreen')
#plt.hist(x=contention1[contention1 != 0], bins='auto', alpha=0.7, rwidth=0.85, label='contention1', color='firebrick')
n, bins, patches = plt.hist(x=contention2[contention2 != 0], bins=bs, alpha=0.7, rwidth=0.5, label='contention', color='maroon', normed=True)
plt.hist(x=baseline2[baseline2 != 0], bins=br, alpha=0.7, rwidth=0.5, label='baseline', color='limegreen', normed=True)

plt.grid(axis='y', alpha=0.75)
plt.xlabel('Clock Thread Cycles')
plt.ylabel('Frequency')
plt.title('Contention Histogram')
plt.text(23, 45, r'$\mu=15, b=3$')
maxfreq = n.max()
# Set a clean upper y-axis limit.
#plt.ylim(ymax=np.ceil(maxfreq / 10) * 10 if maxfreq % 10 else maxfreq + 10)

rmin = min(contention2.min(),  baseline2.min())
rmax = max(contention2.max(),  baseline2.max())

con_min_percentile = np.percentile(contention2, 0.5)
con_max_percentile = np.percentile(contention2, 99.95)
base_min_percentile = np.percentile(baseline2, 0.5)
base_max_percentile = np.percentile(baseline2, 99.95)

axis_min = min(con_min_percentile, base_min_percentile)
axis_max = max(con_max_percentile, base_max_percentile)

plt.xlim(xmin=axis_min, xmax=axis_max)
plt.legend(loc='upper right')
plt.savefig("hist.png")
