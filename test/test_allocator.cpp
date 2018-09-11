#include "allocator.h"
#include "typed_allocator.h"

#include "cassert"

using namespace std;
using namespace kcore;

template<class Alloc> requires allocator<Alloc> && extend_allocator<Alloc>
void test(Alloc &allocator) {
    int *a = allocator.allocate();
    assert( allocator.available_count() == 7 );
    assert( allocator.has(a) == true );
    assert( allocator.deallocate(a) == 0);
}

int main() {
    typed_allocator<int,8> allocator;
    test(allocator);
    return 0;
}
