#include <gcd.h>

intmax_t gcd( intmax_t a, intmax_t b ) {
  while( a * b != 0 ) {
    if( a >= b ) {
      a %= b;
    } else {
      b %= a;
    }
  }
  return a + b;
}

intmax_t lcm( intmax_t a, intmax_t b ) {
  return ( a * b ) / gcd( a, b );
}
