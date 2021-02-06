//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads, pre-sm-60
// UNSUPPORTED: windows && pre-sm-70
//  ... test crashes clang

// <cuda/std/atomic>

// template <class Integral>
//     Integral
//     atomic_fetch_add(volatile atomic<Integral>* obj, Integral op);
//
// template <class Integral>
//     Integral
//     atomic_fetch_add(atomic<Integral>* obj, Integral op);
//
// template <class T>
//     T*
//     atomic_fetch_add(volatile atomic<T*>* obj, ptrdiff_t op);
//
// template <class T>
//     T*
//     atomic_fetch_add(atomic<T*>* obj, ptrdiff_t op);

#include <cuda/std/atomic>
#include <cuda/std/type_traits>
#include <cuda/std/cassert>

#include "test_macros.h"
#include "atomic_helpers.h"
#include "cuda_space_selector.h"

template <class T, template<typename, typename> typename Selector, cuda::thread_scope>
struct TestFn {
  __host__ __device__
  void operator()() const {
    {
        typedef cuda::std::atomic<T> A;
        Selector<A, constructor_initializer> sel;
        A & t = *sel.construct();
        cuda::std::atomic_init(&t, T(1));
        assert(cuda::std::atomic_fetch_add(&t, T(2)) == T(1));
        assert(t == T(3));
    }
    {
        typedef cuda::std::atomic<T> A;
        Selector<volatile A, constructor_initializer> sel;
        volatile A & t = *sel.construct();
        cuda::std::atomic_init(&t, T(1));
        assert(cuda::std::atomic_fetch_add(&t, T(2)) == T(1));
        assert(t == T(3));
    }
  }
};

template <class T, template<typename, typename> typename Selector>
__host__ __device__
void testp()
{
    {
        typedef cuda::std::atomic<T> A;
        typedef typename cuda::std::remove_pointer<T>::type X;
        Selector<A, constructor_initializer> sel;
        A & t = *sel.construct();
        cuda::std::atomic_init(&t, T(1*sizeof(X)));
        assert(cuda::std::atomic_fetch_add(&t, 2) == T(1*sizeof(X)));
        assert(t == T(3*sizeof(X)));
    }
    {
        typedef cuda::std::atomic<T> A;
        typedef typename cuda::std::remove_pointer<T>::type X;
        Selector<volatile A, constructor_initializer> sel;
        volatile A & t = *sel.construct();
        cuda::std::atomic_init(&t, T(1*sizeof(X)));
        assert(cuda::std::atomic_fetch_add(&t, 2) == T(1*sizeof(X)));
        assert(t == T(3*sizeof(X)));
    }
}

int main(int, char**)
{
    _LIBCUDACXX_CUDA_DISPATCH(
        GREATER_THAN_SM62, _LIBCUDACXX_ARCH_BLOCK(
            TestEachIntegralType<TestFn, local_memory_selector>()();
            testp<int*, local_memory_selector>();
            testp<const int*, local_memory_selector>();
        ),
        HOST, _LIBCUDACXX_ARCH_BLOCK(
            TestEachIntegralType<TestFn, local_memory_selector>()();
            testp<int*, local_memory_selector>();
            testp<const int*, local_memory_selector>();
        ),
        DEVICE, _LIBCUDACXX_ARCH_BLOCK(
            TestEachIntegralType<TestFn, shared_memory_selector>()();
            testp<int*, shared_memory_selector>();
            testp<const int*, shared_memory_selector>();
            TestEachIntegralType<TestFn, global_memory_selector>()();
            testp<int*, global_memory_selector>();
            testp<const int*, global_memory_selector>();
        )
    )

  return 0;
}
