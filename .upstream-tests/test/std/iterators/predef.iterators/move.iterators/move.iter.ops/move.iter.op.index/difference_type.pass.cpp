//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <cuda/std/iterator>

// move_iterator

// requires RandomAccessIterator<Iter>
//   unspecified operator[](difference_type n) const;
//
//  constexpr in C++17

#include <cuda/std/iterator>
#include <cuda/std/cassert>
#include <cuda/std/memory>

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
void
test(It i, typename cuda::std::iterator_traits<It>::difference_type n,
     typename cuda::std::iterator_traits<It>::value_type x)
{
    typedef typename cuda::std::iterator_traits<It>::value_type value_type;
    const cuda::std::move_iterator<It> r(i);
    value_type rr = r[n];
    assert(rr == x);
}

struct do_nothing
{
    void operator()(void*) const {}
};

int main(int, char**)
{
    {
        char s[] = "1234567890";
        test(random_access_iterator<char*>(s+5), 4, '0');
        test(s+5, 4, '0');
    }
#if TEST_STD_VER >= 11
    {
        int i[5];
        typedef cuda::std::unique_ptr<int, do_nothing> Ptr;
        Ptr p[5];
        for (unsigned j = 0; j < 5; ++j)
            p[j].reset(i+j);
        test(p, 3, Ptr(i+3));
    }
#endif
#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    typedef cuda::std::move_iterator<const char *> MI;
    constexpr MI it1 = cuda::std::make_move_iterator(p);
    static_assert(it1[0] == '1', "");
    static_assert(it1[5] == '6', "");
    }
#endif

  return 0;
}
