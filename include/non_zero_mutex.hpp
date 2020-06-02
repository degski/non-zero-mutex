
// Copyright (c) 2020 degski
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#if not( defined( __clang__ ) or defined( __GNUC__ ) )
#    include <emmintrin.h>
#endif

#include "non-zero-mutex/hedley/hedley.h"
#define NON_ZERO_MUTEX_ALWAYS_INLINE HEDLEY_ALWAYS_INLINE

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <atomic>
#include <type_traits>

namespace sax {
namespace detail {

NON_ZERO_MUTEX_ALWAYS_INLINE void yield ( ) noexcept {
#if ( defined( __clang__ ) or defined( __GNUC__ ) )
    asm( "pause" ); // no noop
#else
    _mm_pause ( );
#endif
}

template<typename FlagType, typename = std::enable_if_t<std::is_scalar<FlagType>::value>>
struct never_null_mutex {

    using value_type = FlagType;

    never_null_mutex ( ) noexcept                 = default;
    never_null_mutex ( never_null_mutex const & ) = delete;
    never_null_mutex ( never_null_mutex && )      = delete;
    ~never_null_mutex ( )                         = default;

    never_null_mutex & operator= ( never_null_mutex const & ) = delete;
    never_null_mutex & operator= ( never_null_mutex && ) = delete;

    static constexpr FlagType uninitialized               = 0;
    static constexpr FlagType unlocked                    = 1;
    static constexpr FlagType locked_reader               = 2;
    static constexpr FlagType unlocked_locked_reader_mask = 3;
    static constexpr FlagType locked_writer               = 4;

    NON_ZERO_MUTEX_ALWAYS_INLINE void lock ( ) noexcept {
        do {
            while ( unlocked != flag )
                yield ( );
        } while ( not try_lock ( ) );
    }

    [[nodiscard]] NON_ZERO_MUTEX_ALWAYS_INLINE bool try_lock ( ) noexcept {
        return unlocked == flag.exchange ( locked_writer, std::memory_order_acquire );
    }

    NON_ZERO_MUTEX_ALWAYS_INLINE void unlock ( ) noexcept { flag.store ( unlocked, std::memory_order_release ); }

    NON_ZERO_MUTEX_ALWAYS_INLINE void lock ( ) const noexcept {
        do {
            while ( not( unlocked_locked_reader_mask & flag ) )
                yield ( );
        } while ( not try_lock ( ) );
    }

    [[nodiscard]] NON_ZERO_MUTEX_ALWAYS_INLINE bool try_lock ( ) const noexcept {
        return unlocked_locked_reader_mask &
               const_cast<std::atomic<FlagType> *> ( &flag )->exchange ( locked_reader, std::memory_order_acquire );
    }

    NON_ZERO_MUTEX_ALWAYS_INLINE void unlock ( ) const noexcept {
        const_cast<std::atomic<FlagType> *> ( &flag )->store ( unlocked, std::memory_order_release );
    }

    private:
    std::atomic<FlagType> flag = { unlocked };
};

} // namespace detail

template<typename FlagType = char>
struct never_null_mutex final : public detail::never_null_mutex<FlagType> {};

struct alignas ( 64 ) cache_aligned_never_null_mutex final : public detail::never_null_mutex<std::uintptr_t> {};

} // namespace sax

#undef NON_ZERO_MUTEX_NON_ZERO_MUTEX_ALWAYS_INLINE
