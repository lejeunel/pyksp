#!/usr/bin/env python3

import pyksp
import numpy as np
from graphviz import Digraph
import matplotlib.pyplot as plt
import os
import glob

# Define nodes by strings and impose a correspondence with integers
nodes = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'z': 7}
inv_nodes = {v: k for k, v in nodes.items()}

# build adjacency list: each row contains two integers where the
# first is the input node, and the second is the output node of an edge
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

# define weights (lengths) of each edge
W = np.array([1, 1, 1, 1, 1, 3, 5, 1, 1, 2, 7, 2, 1])

# initialize graph object using arguments:
# - ndarray of shape [N] where each element u[i] is the index of the input node of edge i
# - ndarray of shape [N] where each element v[i] is the index of the output node of edge i
# - ndarray of shape [N] where each element w[i] is the weight (length) of edge i
# - integer: total number of nodes in the graph
# - integer: index of source node
# - integer: index of sink node
graph = pyksp.PyKsp(A[:, 0], A[:, 1], W, np.unique(A).size, 0, 7)

# Since all our edges have positive lengths, the obvious solution would be the empty set
# We set min_cost to False to force the algorithm to find some paths
# l_max governs the maximum number of path to find (ignored when min_cost is True)
# The verbose flag will log to stdout
graph.config(min_cost=False, verbose=True, l_max=2)

# run algorithm
res = graph.run()

#Below, we produce a plot of solutions

# delete graphviz files that could exist
files = glob.glob(os.path.join('*.gv.*'))
for f in files:
    os.remove(f)

# generate image of graphs
fnames = ['graph_{}'.format(k) for k in range(len(res))]
for fname, path in zip(fnames, res):
    f = Digraph(name=fname, format='png')
    f.attr('node', shape='circle')

    edges_of_path = [(path[i], path[i + 1]) for i in range(len(path) - 1)]

    for e, w in zip(A, W):

        f.edge(inv_nodes[e[0]],
               inv_nodes[e[1]],
               label=str(w),
               color='green' if ((e[0], e[1]) in edges_of_path) else None)

    f.render()

# plots images
fig, axs = plt.subplots(1, len(fnames), figsize=(10, 6))
if (len(fnames) == 1):
    axs = [axs]

for i, (fname, ax) in enumerate(zip(fnames, axs)):
    im = plt.imread(fname + '.gv.png')
    ax.imshow(im)
    ax.set_title('path {}/{}'.format(i + 1, len(fnames)))
    ax.axis('off')

plt.tight_layout()
fig.savefig('demo.png')
