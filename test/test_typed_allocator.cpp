#include "typed_allocator.h"

using namespace kcore;

int main() {
    TypedAllocator<int,8> allocator;
    int *a = allocator.alloc();
    return 0;
}