#ifndef PATH_H_
#define PATH_H_

#include "misc.h"
#include <vector>

class Path{
    public:
        Path(int n);
        ~Path();
        int n_vertices;
        int *vertices;
        scalar_t length;
        void print();

};


#endif // PATH_H_
