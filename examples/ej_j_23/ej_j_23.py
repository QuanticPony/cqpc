import subprocess

import matplotlib.pyplot as plt

from cqppy import cqpc_histogram

n_agents = 1000
n_bins = 100
max_histogram = 400
t_max = 10000
total_money = 40000

CMD = lambda i: [str(c) for c in 
    ["./ej_j_23.o", n_agents, n_bins, t_max, total_money, i, max_histogram]
]

# Start 100 processes
ITERS = 100
processes: list[subprocess.Popen] = []
for i in range(ITERS):
    processes.append(subprocess.Popen(CMD(i)))

# Wait for them to finish
for p in processes:
    p.wait()

Hist = cqpc_histogram()
Hist.max = max_histogram
Hist.min = 0
Hist.nbins = n_bins

plt.subplot()
for i in range(ITERS):
    hist = cqpc_histogram.from_file(str(i))
    Hist.values[:] += hist.values

plt.bar(Hist.get_x_array(), Hist.values, Hist.delta)

plt.show()
