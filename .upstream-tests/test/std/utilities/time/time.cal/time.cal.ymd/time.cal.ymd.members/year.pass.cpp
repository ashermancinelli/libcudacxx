//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++98, c++03, c++11

// <chrono>
// class year_month_day;

// constexpr chrono::day day() const noexcept;
//  Returns: d_

#include <cuda/std/chrono>
#include <cuda/std/type_traits>
#include <cuda/std/cassert>

#include "test_macros.h"

int main(int, char**)
{
    using year           = cuda::std::chrono::year;
    using month          = cuda::std::chrono::month;
    using day            = cuda::std::chrono::day;
    using year_month_day = cuda::std::chrono::year_month_day;

    ASSERT_NOEXCEPT(                std::declval<const year_month_day>().year());
    ASSERT_SAME_TYPE(year, decltype(cuda::std::declval<const year_month_day>().year()));

    static_assert( year_month_day{}.year() == year{}, "");

    for (int i = 1; i <= 50; ++i)
    {
        year_month_day ym(year{i}, month{}, day{});
        assert( static_cast<int>(ym.year()) == i);
    }

  return 0;
}
