#include <alloc/pool.hpp>
#include <cassert>
#include <iostream>
#include <vector>

typedef struct {
    double x;
    double y;
} test;

constexpr int no_elements_small   = 48;
constexpr int no_elements_invalid = -1;
constexpr int no_elements_large   = 200000;

int main(void)
{

    std::vector<test*> ptrs;

    Allocator::Pool<test>* p = new Allocator::Pool<test>(no_elements_small);

    // Try to allocate when there are no free elements left
    // Should return nullptr
    for (int i = 0; i < no_elements_small; i++) {
        ptrs.push_back(p->alloc());
    }

    assert(p->alloc() == nullptr);

    // Negative element size
    // Should fail
    bool failed = false;
    try {
        Allocator::Pool<test>* tmp = new Allocator::Pool<test>(no_elements_invalid);
    } catch (std::runtime_error& e) {
        failed = true;
    }

    assert(failed);

    // Trying to free a pointer which is not in the memory block
    // Should fail
    failed = false;
    test t {};

    try {
        p->free(&t);
    } catch (std::runtime_error& e) {
        failed = true;
    }
    assert(failed);

    for (int i = 0; i < no_elements_small; i++) {
        p->free(ptrs.back());
        ptrs.pop_back();
    }

    delete p;

    Allocator::Pool<test>* l = new Allocator::Pool<test>(no_elements_large);

    // Try to allocate when there are no free elements left
    // Should return nullptr
    for (int i = 0; i < no_elements_large; i++) {
        ptrs.push_back(l->alloc());
    }

    assert(l->alloc() == nullptr);

    for (int i = 0; i < no_elements_large; i++) {
        l->free(ptrs.back());
        ptrs.pop_back();
    }

    delete l;

    return 0;
}
