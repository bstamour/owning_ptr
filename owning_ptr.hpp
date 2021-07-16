

#include <memory>

​template <class T> class owning_ptr;
template <class T> class owning_ptr_ref;

template <class T> class borrowed_ptr;

​

template <class T> class owning_ptr {

    std::shared_ptr<T> impl_;

    friend owning_ptr_ref<T>;

​

public:

    using value_type = T;

    using reference = T&;

    using const_reference = T const&;

    using pointer = T*;

    using const_pointer = T const*;

​

    explicit owning_ptr(T* p) : impl_(p) {}

​

    owning_ptr(owning_ptr const&) = delete;

​

    reference operator*() { return *impl_; }

    const_reference operator*() const { return *impl_; }

​

    pointer operator->() { return impl_.get(); }

    const_pointer operator->() { return impl_.get(); }

};

​

template <class T> class owning_ptr_ref {

    std::weak_ptr<T> impl_;

    friend borrowed_ptr<T>;

​

public:

    using value_type = T;

    owning_ptr_ref(owning_ptr<T> const& owner) : impl_(owner.impl_) {}

    borrowed_ptr<T> lock();

};

​

template <class T> class borrowed_ptr {

    std::shared_ptr<T> impl_;

​

public:

    using value_type = T;

    using reference = T&;

    using const_reference = T const&;

    using pointer = T*;

    using const_pointer = T const*;

​

    borrowed_ptr(owning_ptr_ref<T> const& ref) : impl_(ref.impl_.lock()) {}

​

    borrowed_ptr(borrowed_ptr const&) = delete;

​

    reference operator*() { return *impl_; }

    const_reference operator*() const { return *impl_; }

​

    pointer operator->() { return impl_.get(); }

    const_pointer operator->() { return impl_.get(); }

​

    explicit operator bool() const { return !!impl_; }

};

​

template <class T> borrowed_ptr<T> owning_ptr_ref<T>::lock() {

    return borrowed_ptr<T>(*this);

}

​

// test code.

​

auto f() {

    owning_ptr<T> ptr(new int 42);

    return owning_ptr_ref<int>(ptr);

}

​

void g() {

    owning_ptr_ref<int> ref = f();

​

    if (borrowed_ptr<int> bptr(ref)) {

        // Safe to use.

    } else {

        // Not safe to use.

    }

​

    // later on...

    if (auto bptr2 = ref.lock()) {

        // Still safe to use.

    } else {

        // Not safe to use.

    }

}

​

// end test code.

