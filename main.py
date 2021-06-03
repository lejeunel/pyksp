#!/usr/bin/env python3

import pyksp
import numpy as np
import pyksp
from graphviz import Digraph
import matplotlib.pyplot as plt

nodes = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'z': 7}
inv_nodes = {v: k for k, v in nodes.items()}

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
A = np.array(A)

W = np.array([1, 1, 1, 1, 1, 3, 5, 1, 1, 2, 7, 2, 1])

tracker = pyksp.PyKsp(A[:, 0], A[:, 1], W, np.unique(A).size, 0, 7)
tracker.config(min_cost=False, verbose=True, l_max=3)

res = tracker.run()

# generate image of graphs
fnames = ['graph_{}'.format(k) for k in range(len(res))]
for fname, path in zip(fnames, res):
    f = Digraph(name=fname, format='png')
    f.attr('node', shape='circle')

    edges_of_path = [(path[i], path[i + 1]) for i in range(len(path) - 1)]

    import pdb
    pdb.set_trace()  ## DEBUG ##
    for e, w in zip(A, W):

        f.edge(inv_nodes[e[0]],
               inv_nodes[e[1]],
               label=str(w),
               color='green' if ((e[0], e[1]) in edges_of_path) else None)

    f.render()

# plots images
fig, axs = plt.subplots(1, len(fnames), figsize=(15, 6))
for i, fname in enumerate(fnames):
    im = plt.imread(fname + '.gv.png')
    axs[i].imshow(im)
    axs[i].set_title('path {}/{}'.format(i + 1, len(fnames)))
    axs[i].axis('off')

plt.show()
