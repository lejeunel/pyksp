#!/usr/bin/env python3

import pyksp
import numpy as np
import pyksp

nodes = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'z': 7}

A = []
A.append((nodes['a'], nodes['b']))
A.append((nodes['b'], nodes['c']))
A.append((nodes['c'], nodes['d']))
A.append((nodes['d'], nodes['z']))
A.append((nodes['a'], nodes['e']))
A.append((nodes['e'], nodes['f']))
A.append((nodes['f'], nodes['z']))
A.append((nodes['e'], nodes['b']))
A.append((nodes['b'], nodes['f']))
A.append((nodes['f'], nodes['d']))
A.append((nodes['a'], nodes['g']))
A.append((nodes['g'], nodes['z']))
A.append((nodes['c'], nodes['g']))
A = np.array(A).T

W = np.array([1, 1, 1, 1, 1, 3, 5, 1, 1, 2, 7, 2, 1])

tracker = pyksp.PyKsp(np.unique(A).size)
tracker.config(source_id=0, sink_id=7)

tracker.init_from_adj(A, W)
print(tracker)
tracker.run()
