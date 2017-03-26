#pragma once
#include <algorithm>
#include <stddef.h>
#include <unordered_map>
#include <vector>
namespace ry {
using hashT = size_t;
template <typename KeyT>
hashT hash(KeyT key);

namespace detail {
template <typename KeyT, typename ValT>
struct MapEntry {
  MapEntry() : key(), val(), isNull(true) {}

  MapEntry(KeyT aKey, ValT aVal) : key(aKey), val(aVal), isNull(false) {}
  operator bool() const { return !isNull; }
  KeyT key;
  ValT val;
  bool isNull;
};
}

template <typename KeyT, typename ValT>
class map {
 private:
  using ValsT = std::vector<detail::MapEntry<KeyT, ValT>>;
  using iterator = typename ValsT::iterator;
  using const_iterator = typename ValsT::const_iterator;

  size_t nElem;
  ValsT vals;

  iterator findElem(KeyT key) {
    auto sz = vals.size();
    auto h = hash(key);
    if (h >= sz) {
      h = h % sz;
    }
    auto v = vals.begin() + h;
    auto matcher = [&key](typename ValsT::const_reference entry) {
      return entry.isNull || entry.key == key;
    };

    auto it = std::find_if(v, vals.end(), matcher);
    if (it == vals.end()) {
      it = std::find_if(vals.begin(), v, matcher);
      return it == v ? vals.end() : it;
    }
    return it;
  }

 public:
  map() : nElem(0), vals(3) {}

  void insert(KeyT key, ValT val) {
    auto it = findElem(key);
    if (it == vals.end()) {
      // TODO: Is this possible?
      return;
    }
    bool checkAlloc = it->isNull;
    *it = detail::MapEntry<KeyT, ValT>(key, val);
    if (checkAlloc) {
      ++nElem;
      // Realloc and rehash
      if (4 * nElem > 3 * vals.size()) {
        nElem = 0;
        ValsT oldVals(std::move(vals));
        vals.resize(2 * oldVals.size());
        std::for_each(oldVals.begin(), oldVals.end(),
                      [this](typename ValsT::const_reference entry) {
                        if (!entry.isNull) {
                          insert(entry.key, entry.val);
                        }
                      });
      }
    }
  }

  const detail::MapEntry<KeyT, ValT> &lookup(KeyT key) {
    static detail::MapEntry<KeyT, ValT> bogus;
    auto it = findElem(key);
    if (it == vals.end()) {
      return bogus;
    }
    return *it;
  }
};

template <>
hashT hash(int key) {
  return std::hash<int>()(key);
}

template <>
hashT hash(size_t key) {
  return std::hash<size_t>()(key);
}
}
