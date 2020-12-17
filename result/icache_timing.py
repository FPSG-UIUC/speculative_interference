import matplotlib.pyplot as plt
import numpy as np
import csv

data_s = []
count = 0
f = open('data', 'r')
x = f.readlines()
for l in x:
    entries = l.split(',')
    for entry in entries:
        if entry != '':
            data_s.append(int(entry))
#print(data)
data_s.sort()
data_s_p = [i for i in data_s if i < 350]
bs = data_s[-1]/5
plt.figure(figsize=(5, 2))
plt.hist(data_s_p, 70, normed=True, alpha=0.7, rwidth=0.5,
        color='limegreen', label='I-Cache')
plt.grid(axis='y', alpha=0.75)
plt.xlabel('Cycles')
plt.ylabel('Frequency')
#plt.xlim(xmin=0, xmax=350)
plt.legend(loc="upper right")
plt.title('I-Cache attack Reload Times')
#plt.show()
plt.savefig('icu-s.pdf')
