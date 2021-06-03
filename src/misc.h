#ifndef MISC_H_
#define MISC_H_

#include <stdlib.h>
#include <limits>

typedef float scalar_t;
const scalar_t INF_LENGTH = std::numeric_limits<scalar_t>::max();

#ifdef DEBUG
#define ASSERT(x) if(!(x)) {                                            \
    std::cerr << "ASSERT FAILED IN " << __FILE__ << ":" << __LINE__ << endl; \
    abort();                                                            \
  }
#else
#define ASSERT(x)
#endif

template<class T>
T **allocate_array(int a, int b) {
  T *whole = new T[a * b];
  T **array = new T *[a];
  for(int k = 0; k < a; k++) {
    array[k] = whole;
    whole += b;
  }
  return array;
}

template<class T>
void deallocate_array(T **array) {
  if(array) {
    delete[] array[0];
    delete[] array;
  }
}


#endif // MISC_H_
