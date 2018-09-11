#include "typed_allocator.h"
#include "cassert"
#include "iostream"

using namespace std;
using namespace kcore;

int main() {
    typed_allocator<int,8> allocator;
    int *a = allocator.allocate();
    assert( allocator.available_count() == 7 );
    assert( allocator.has(a) == true );
    assert( allocator.deallocate(a) == 0);
    return 0;
}
