import matplotlib.pyplot as plt
import numpy as np

class cqpc_histogram:
    def __init__(self):
        self._min: float = None
        self._max: float = None
        self._delta: float = None
        self._nbins: int = None
        self.values: list[float] = None
        self.count: int = None
    
    def get_x_array(self):
        return np.linspace(self.min, self.max, self.nbins)
    
    @property
    def nbins(self):
        return self._nbins

    @nbins.setter
    def nbins(self, value):
        self._nbins = int(value)
        self.values = np.zeros(self._nbins)
        if self.min is not None and self.max is not None:
            self.delta = (self.max - self.min) / self.nbins


    @property
    def max(self):
        return self._max
    
    @max.setter
    def max(self, value):
        self._max = float(value)
        if self.min is not None and self.nbins is not None:
            self.delta = (self.max - self.min) / self.nbins

    @property
    def min(self):
        return self._min
    
    @min.setter
    def min(self, value):
        self._min = float(value)
        if self.max is not None and self.nbins is not None:
            self.delta = (self.max - self.min) / self.nbins

    @property
    def delta(self):
        return self._delta
    
    @delta.setter
    def delta(self, value):
        self._delta = float(value)

    @classmethod
    def from_file(cls, f):
        ins = cls()
        with open(f, 'rt') as file:
            counter = 0
            for l in file:
                if l.startswith("//"):
                    k,v = l.strip("//").replace(" ", "").replace("\n", "").split("=")
                    setattr(ins, k, v)
                else:
                    ins.values[counter] = float(l)
                    counter += 1
        return ins  
