#include "graph.h"

class Edge {
public:
  int occupied;
  scalar_t length, positivized_length;
  Vertex *origin_vertex, *terminal_vertex;

  // These fields are used for the linked list of a vertex's leaving
  // edge list. We have to do insertions / deletions.
  Edge *next_leaving_edge, *pred_leaving_edge;

  inline void invert();
};

class Vertex {
public:
  scalar_t distance_from_source;
  Edge *pred_edge_toward_source;

  Edge *leaving_edge_list_root;
  Vertex **heap_slot;

  Vertex();

  inline void add_leaving_edge(Edge *e);
  inline void del_leaving_edge(Edge *e);
  inline void decrease_distance_in_heap(Vertex **heap);
  inline void increase_distance_in_heap(Vertex **heap, Vertex **heap_bottom);
};


//////////////////////////////////////////////////////////////////////

void Edge::invert() {
  length = - length;
  positivized_length = - positivized_length;
  origin_vertex->del_leaving_edge(this);
  terminal_vertex->add_leaving_edge(this);
  swap(terminal_vertex, origin_vertex);
}

//////////////////////////////////////////////////////////////////////

Vertex::Vertex() {
  leaving_edge_list_root = 0;
}

void Vertex::add_leaving_edge(Edge *e) {
  e->next_leaving_edge = leaving_edge_list_root;
  e->pred_leaving_edge = 0;
  if(leaving_edge_list_root) {
    leaving_edge_list_root->pred_leaving_edge = e;
  }
  leaving_edge_list_root = e;
}

void Vertex::del_leaving_edge(Edge *e) {
  if(e == leaving_edge_list_root) {
    leaving_edge_list_root = e->next_leaving_edge;
  }
  if(e->pred_leaving_edge) {
    e->pred_leaving_edge->next_leaving_edge = e->next_leaving_edge;
  }
  if(e->next_leaving_edge) {
    e->next_leaving_edge->pred_leaving_edge = e->pred_leaving_edge;
  }
}

void Vertex::decrease_distance_in_heap(Vertex **heap) {
  Vertex **p, **h;
  h = heap_slot;
  while(1) {
    if(h <= heap) break;
    p = heap + ((h - heap + 1) >> 1) - 1;
    if((*p)->distance_from_source <= distance_from_source) break;
    swap((*p)->heap_slot, heap_slot);
    swap(*p, *h);
    h = p;
  }
}

void Vertex::increase_distance_in_heap(Vertex **heap, Vertex **heap_bottom) {
  Vertex **c1, **c2, **h;
  h = heap_slot;
  while(1) {
    c1 = heap + 2 * (h - heap) + 1;
    if(c1 >= heap_bottom) break;
    c2 = c1 + 1;
    if((*c1)->distance_from_source < distance_from_source) {
      if(c2 < heap_bottom && (*c2)->distance_from_source < (*c1)->distance_from_source) {
        swap((*c2)->heap_slot, heap_slot);
        swap(*c2, *h);
        h = c2;
      } else {
        swap((*c1)->heap_slot, heap_slot);
        swap(*c1, *h);
        h = c1;
      }
    } else {
      if(c2 < heap_bottom && (*c2)->distance_from_source < distance_from_source) {
        swap((*c2)->heap_slot, heap_slot);
        swap(*c2, *h);
        h = c2;
      } else break;
    }
  }
}

Graph::Graph(int n_vertices, int n_edges,
             int * vertex_from, int *vertex_to,
             scalar_t * weights,
             int source, int sink)
{
    _n_vertices = n_vertices;
    _n_edges = n_edges;

    _edges = new Edge[_n_edges];
    _vertices = new Vertex[_n_vertices];
    _heap = new Vertex *[_n_vertices];
    _dp_order = new Vertex *[_n_vertices];

    _source = &_vertices[source];
    _sink = &_vertices[sink];

    for(int e = 0; e < n_edges; e++) {
        _vertices[vertex_from[e]].add_leaving_edge(&_edges[e]);
        _edges[e].occupied = 0;
        _edges[e].length = weights[e];
        _edges[e].origin_vertex = &_vertices[vertex_from[e]];
        _edges[e].terminal_vertex = &_vertices[vertex_to[e]];
    }

  for(int v = 0; v < _n_vertices; v++) {
    _heap[v] = &_vertices[v];
    _vertices[v].heap_slot = &_heap[v];
  }

  paths = 0;
  n_paths = 0;

  compute_dp_ordering();
}

Graph::~Graph() {
    delete[] _vertices;
    delete[] _edges;
    if(n_paths > 0){
        delete[] _dp_order;
        delete[] _heap;
        for(int p = 0; p < n_paths; p++) delete paths[p];
        delete[] paths;
    }
}


void Graph::dp_compute_distances(){
  Vertex *v, *tv;
  Edge *e;
  scalar_t d;

  for(int k = 0; k < _n_vertices; k++) {
    _vertices[k].distance_from_source = FLT_MAX;
    _vertices[k].pred_edge_toward_source = 0;
  }

  _source->distance_from_source = 0;

  for(int k = 0; k < _n_vertices; k++) {
    v = _dp_order[k];
    for(e = v->leaving_edge_list_root; e; e = e->next_leaving_edge) {
      d = v->distance_from_source + e->positivized_length;
      tv = e->terminal_vertex;
      if(d < tv->distance_from_source) {
        tv->distance_from_source = d;
        tv->pred_edge_toward_source = e;
      }
    }
  }

}


void Graph::compute_dp_ordering() {
  Vertex *v;
  Edge *e;
  int ntv;

  // This method orders the nodes by putting first the ones with no
  // predecessors, then going on adding nodes whose predecessors have
  // all been already added. Computing the distances from the source
  // by visiting nodes in that order is equivalent to DP.

  int *nb_predecessors = new int[_n_vertices];

  Vertex **already_processed = _dp_order, **front = _dp_order, **new_front = _dp_order;

  for(int k = 0; k < _n_vertices; k++) {
    nb_predecessors[k] = 0;
  }

  for(int k = 0; k < _n_vertices; k++) {
    v = &_vertices[k];
    for(e = v->leaving_edge_list_root; e; e = e->next_leaving_edge) {
      ntv = int(e->terminal_vertex - _vertices);
      nb_predecessors[ntv]++;
    }
  }

  for(int k = 0; k < _n_vertices; k++) {
    if(nb_predecessors[k] == 0) {
      *(front++) = _vertices + k;
    }
  }

  while(already_processed < front) {
    // Here, nodes before already_processed can be ignored, nodes
    // before front were set to 0 predecessors during the previous
    // iteration. During this new iteration, we have to visit the
    // successors of these ones only, since they are the only ones
    // which may end up with no predecessors.
    new_front = front;
    while(already_processed < front) {
      v = *(already_processed++);
      for(e = v->leaving_edge_list_root; e; e = e->next_leaving_edge) {
        ntv = int(e->terminal_vertex - _vertices);
        nb_predecessors[ntv]--;
        ASSERT(nb_predecessors[ntv] >= 0);
        if(nb_predecessors[ntv] == 0) {
          *(new_front++) = e->terminal_vertex;
        }
      }
    }
    front = new_front;
  }

  if(already_processed < _dp_order + _n_vertices) {
    cerr << __FILE__ << ": The graph is not a DAG." << endl;
    abort();
  }

  delete[] nb_predecessors;
}

void Graph::Dijkstra()
{
  int heap_size;
  Vertex *v, *tv, **last_slot;
  Edge *e;
  scalar_t d;

  for(int k = 0; k < _n_vertices; k++) {
    _vertices[k].distance_from_source = FLT_MAX;
    _vertices[k].pred_edge_toward_source = 0;
  }

  heap_size = _n_vertices;
  _source->distance_from_source = 0;
  _source->decrease_distance_in_heap(_heap);

  while(heap_size > 1) {
    // Get the closest to the source
    v = _heap[0];

    // Remove it from the heap (swap it with the last_slot in the heap, and
    // update the distance of that one)
    heap_size--;
    last_slot = _heap + heap_size;
    swap(*_heap, *last_slot); swap((*_heap)->heap_slot, (*last_slot)->heap_slot);
    (*_heap)->increase_distance_in_heap(_heap, last_slot);

    // Now update the neighbors of the node currently closest to the
    // source
    for(e = v->leaving_edge_list_root; e; e = e->next_leaving_edge) {
      d = v->distance_from_source + e->positivized_length;
      tv = e->terminal_vertex;
      if(d < tv->distance_from_source) {
        ASSERT(tv->heap_slot < last_slot);
        tv->distance_from_source = d;
        tv->pred_edge_toward_source = e;
        tv->decrease_distance_in_heap(_heap);
      }
    }
  }
}

int Graph::retrieve_one_path(Edge *e, Path *path, int *used_edges) {
  Edge *f, *next = 0;
  int l = 0, nb_occupied_next;

  if(path) {
    path->vertices[l++] = int(e->origin_vertex - _vertices);
    // path->length = e->length;
    path->length = 0;
  } else l++;

  while(e->terminal_vertex != _sink) {
    if(path) {
      path->vertices[l++] = int(e->terminal_vertex - _vertices);
      path->length += e->length;
    } else l++;

    nb_occupied_next = 0;
    for(f = e->terminal_vertex->leaving_edge_list_root; f; f = f->next_leaving_edge) {
      if(f->occupied && !used_edges[f - _edges]) {
        nb_occupied_next++; next = f;
      }
    }

// #ifdef DEBUG
//     if(nb_occupied_next == 0) {
//       cerr << __FILE__ << ": retrieve_one_path: Non-sink end point." << endl;
//       abort();
//     }
// #endif

    if(path) { used_edges[next - _edges] = 1; }

    e = next;
  }

  if(path) {
    path->vertices[l++] = int(e->terminal_vertex - _vertices);
    path->length += e->length;
  } else l++;

  return l;
}

void Graph::retrieve_disjoint_paths() {
  Edge *e;
  int p, l;
  int *used_edges;

  for(int p = 0; p < n_paths; p++) delete paths[p];
    delete[] paths;

  n_paths = 0;
  for(e = _source->leaving_edge_list_root; e; e = e->next_leaving_edge) {
    if(e->occupied) { n_paths++; }
  }

  paths = new Path *[n_paths];
  used_edges = new int[_n_edges];
  for(int e = 0; e < _n_edges; e++) {
    used_edges[e] = 0;
  }

  p = 0;
  for(e = _source->leaving_edge_list_root; e; e = e->next_leaving_edge) {
    if(e->occupied && !used_edges[e - _edges]) {
      l = retrieve_one_path(e, 0, used_edges);
      paths[p] = new Path(l);
      retrieve_one_path(e, paths[p], used_edges);
      used_edges[e - _edges] = 1;
      p++;
    }
  }

  delete[] used_edges;
}

void Graph::update_positivized_lengths() {
  for(int k = 0; k < _n_edges; k++) {
    Edge *e = &_edges[k];
    e->positivized_length +=
      e->origin_vertex->distance_from_source - e->terminal_vertex->distance_from_source;
  }
}

void Graph::force_positivized_lengths() {
  for(int k = 0; k < _n_edges; k++) {
    Edge *e = &_edges[k];

    if(e->positivized_length < 0) {
      e->positivized_length = 0.0;
    }
  }
}

void Graph::find_best_paths(){

  scalar_t shortest_path_length, total_length;
  int iter = 0;
  Vertex *v;
  Edge *e;

  // set all edges as non-occupied
  for(int e = 0; e < _n_edges; e++) {
    // _edges[e].length = lengths[e];
    _edges[e].occupied = 0;
    _edges[e].positivized_length = _edges[e].length;
  }


  // Compute the distance of all the nodes from the source by just
  // visiting them in the DAG ordering we computed when
  // building the graph
  dp_compute_distances();

  while(true) {
    // Use the current distance from the source to make all edge
    // lengths positive
    update_positivized_lengths();
    // Fix numerical errors
    force_positivized_lengths();

    Dijkstra();

    shortest_path_length = 0.0;
    total_length = 0.0;

    // Do we reach the sink?
    if(_sink->pred_edge_toward_source) {
      // If yes, compute the length of the best path according to the
      // original edge lengths
      v = _sink;
      while(v->pred_edge_toward_source) {
        shortest_path_length += v->pred_edge_toward_source->length;
        v = v->pred_edge_toward_source->origin_vertex;
      }
      if((shortest_path_length < 0.0) || (min_cost == false)) {
          total_length += shortest_path_length;
          if(verbose)
              std::cout << "k: " << iter << " cost: " << total_length << std::endl;
        // Invert all the edges along the best path
        v = _sink;
        while(v->pred_edge_toward_source) {
          e = v->pred_edge_toward_source;
          v = e->origin_vertex;
          e->invert();
          // This is the only place where we change the occupations of
          // edges
          e->occupied = 1 - e->occupied;
        }
      }
    }
    ++iter;

    if(min_cost & (shortest_path_length < 0.0) &
       (std::abs(shortest_path_length) / total_length < tolerance))
        break;
    if((min_cost==false) && iter == l_max)
        break;

  }


  // Put back the graph in its original state (i.e. invert edges which
  // have been inverted in the process)
  for(int k = 0; k < _n_edges; k++) {
    e = &_edges[k];
    if(e->occupied) { e->invert(); }
  }

}

void Graph::set_min_cost(bool flag){
    min_cost = flag;
}

void Graph::set_verbose(bool flag){
    verbose = flag;
}

void Graph::set_tolerance(float val){
    tolerance = val;
}

void Graph::set_l_max(int val){
    l_max = val;
}
