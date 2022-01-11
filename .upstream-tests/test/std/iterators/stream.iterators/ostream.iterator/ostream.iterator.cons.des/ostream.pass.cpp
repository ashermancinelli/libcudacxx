//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <cuda/std/iterator>

// class ostream_iterator

// ostream_iterator(ostream_type& s);

#include <cuda/std/iterator>
#include <cuda/std/sstream>
#include <cuda/std/cassert>

#include "test_macros.h"

struct MyTraits : cuda::std::char_traits<char> {};

typedef cuda::std::basic_ostringstream<char, MyTraits> StringStream;
typedef cuda::std::basic_ostream<char, MyTraits> BasicStream;

void operator&(BasicStream const&) {}

int main(int, char**)
{
    {
        cuda::std::ostringstream outf;
        cuda::std::ostream_iterator<int> i(outf);
        assert(outf.good());
    }
    {
        StringStream outf;
        cuda::std::ostream_iterator<int, char, MyTraits> i(outf);
        assert(outf.good());
    }

  return 0;
}
