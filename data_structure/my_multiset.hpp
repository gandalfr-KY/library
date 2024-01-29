#pragma once

#include <iostream>
#include <set>

#include "gandalfr/types.hpp"

namespace gandalfr {

template <typename _Key, typename _Compare = std ::less<_Key>,
          typename _Alloc = std ::allocator<_Key>>
class my_multiset : public std::multiset<_Key, _Compare, _Alloc> {
  public:
    using std::multiset<_Key, _Compare, _Alloc>::multiset;
    bool erase_one(const _Key &__x) {
        auto it = this->find(__x);
        if (it == this->end()) {
            return false;
        } else {
            this->erase(it);
            return true;
        }
    }
    i32 erase_all(const _Key &__x) {
        i32 ret = this->count(__x);
        if (ret == 0) {
            return 0;
        } else {
            this->erase(__x);
            return ret;
        }
    }
    const _Key &front() const { return *this->begin(); }
    const _Key &back() const { return *--this->end(); }
    friend std::ostream &operator<<(std::ostream &os, const my_multiset &ms) {
        if (ms.empty())
            return os;
        auto it = ms.begin();
        os << *it;
        for (++it; it != ms.end(); ++it) {
            os << ' ' << *it;
        }
        return os;
    }
};

template <typename _Key, typename _Compare = std ::less<_Key>,
          typename _Alloc = std ::allocator<_Key>>
class my_set : public std::set<_Key, _Compare, _Alloc> {
  public:
    using std::set<_Key, _Compare, _Alloc>::set;
    const _Key &front() const { return *this->begin(); }
    const _Key &back() const { return *--this->end(); }
    friend std::ostream &operator<<(std::ostream &os, const my_set &ms) {
        if (ms.empty())
            return os;
        auto it = ms.begin();
        os << *it;
        for (++it; it != ms.end(); ++it) {
            os << ' ' << *it;
        }
        return os;
    }
};
} // namespace gandalfr
