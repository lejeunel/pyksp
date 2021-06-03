#include "path.h"
#include <iostream>

Path::Path(int n) {
  n_vertices = n;
  vertices = new int[n_vertices];
}

Path::~Path() {
  delete[] vertices;
}

void Path::print(){

    std::cout << "length: " << length << std::endl;
    for(int i=0; i<n_vertices;++i){
        if(i<n_vertices - 1)
            std::cout << "->";
   }
    std::cout << std::endl;
}

