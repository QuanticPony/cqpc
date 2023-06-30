import subprocess
import os
import pathlib
PATH = pathlib.Path(__file__).parent.resolve()

import numpy as np
import matplotlib.pyplot as plt

from cqppy import cqpc_histogram

n_agents = 1000
n_bins = 100
max_histogram = 400
t_max = 10000
total_money = 40000

OUT_DIR = str(PATH) + "/ej_s_22_out"

try:
    os.mkdir(OUT_DIR)
except FileExistsError:
    pass

CMD = lambda i: [str(c) for c in 
    [f"{PATH}/ej_s_22.o", n_agents, n_bins, t_max, total_money, f"{OUT_DIR}/{i}", max_histogram]
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
    hist = cqpc_histogram.from_file(f"{OUT_DIR}/{i}")
    Hist.values[:] += hist.values
Hist.values /= ITERS

plt.bar(Hist.get_x_array(), Hist.values, Hist.delta)

def theoric(x):
    T = total_money / n_agents
    C = 1 / T
    return C * np.exp(-x/T)

plt.plot(Hist.get_x_array(), theoric(Hist.get_x_array()), color="red")

plt.savefig("Result.png")
plt.show()
