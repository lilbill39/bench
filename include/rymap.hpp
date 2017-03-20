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

  ValsT vals;
  size_t nElem;

  iterator findElem(KeyT key) {
    auto h = hash(key) % vals.size();
    auto v = vals.begin() + h;
    auto matcher = [&key](const detail::MapEntry<KeyT, ValT> &entry) {
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
  map() = default;

  void insert(KeyT key, ValT val) {
    if (vals.capacity() == 0) {
      vals.resize(2);
    }
    auto it = findElem(key);
    if (it != vals.end()) {
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
                        [this](const detail::MapEntry<KeyT, ValT> &entry) {
                          if (!entry.isNull) {
                            insert(entry.key, entry.val);
                          }
                        });
        }
      }
      return;
    }
  }

  detail::MapEntry<KeyT, ValT> lookup(KeyT key) {
    auto it = findElem(key);
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