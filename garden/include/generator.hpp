#pragma once

#include <concepts>
#include <coroutine>
#include <exception>

template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T _value;
        std::exception_ptr _exception;

        Generator get_return_object ()
        {
            return Generator(handle_type::from_promise(*this));
        }

        std::suspend_always initial_suspend ()
        {
            return {};
        }

        std::suspend_always final_suspend () noexcept
        {
            return {};
        }

        void unhandled_exception ()
        {
            _exception = std::current_exception();
        }

        template<std::convertible_to<T> From>
        std::suspend_always yield_value (From &&from)
        {
            _value = std::forward<From>(from);
            return {};
        }

        void return_void () { }
    };

    handle_type _h;

    Generator(handle_type h) : _h(h) { }

    Generator(const Generator &) = delete;

    ~Generator()
    {
        _h.destroy();
    }

    explicit operator bool()
    {
        fill();
        return ! _h.done();
    }

    T operator()()
    {
        fill();
        _full = false;
        return std::move(_h.promise()._value);
    }

private:
    bool _full = false;

    void fill ()
    {
        if ( ! _full ) {
            _h();
            if ( _h.promise()._exception ) {
                std::rethrow_exception(_h.promise()._exception);
            }
            _full = true;
        }
    }
};