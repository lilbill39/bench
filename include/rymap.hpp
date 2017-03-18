#pragma once
#include <stddef.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
namespace ry {
using hashT = size_t;
template <typename KeyT>
hashT hash(KeyT key);

namespace detail {
template <typename KeyT, typename ValT>
struct MapEntry {
  MapEntry() : key(), val(), isNull(true) {}

  MapEntry(KeyT aKey, ValT aVal) : key(aKey), val(aVal), isNull(false) {}
  KeyT key;
  ValT val;
  bool isNull;
};
}

template <typename KeyT, typename ValT>
class map {
 public:
  class iterator {
   public:
    iterator() : valid(false) {}
    iterator(const KeyT &akey, const ValT &aval)
        : key(akey), val(aval), valid(true) {}
    const KeyT &key;
    const ValT &val;
    //operator bool() const { return valid; }

   private:
    bool valid;
  };
  map() = default;

  void insert(KeyT key, ValT val) {
    if (vals.size() == 0) {
      vals.reserve(2);
    }
    auto h = hash(key) % vals.size();
    auto v = vals.begin() + h;
    auto it = std::find_if(v, vals.end(),
                           [](const detail::MapEntry<KeyT, ValT> &entry) {
                             return entry.isNull;
                           });
    if (it != vals.end()) {}
    
  }

  iterator lookup(KeyT key);

 private:
  std::vector<detail::MapEntry<KeyT, ValT>> vals;
};

template <>
hashT hash(int key) {
  return static_cast<hashT>(key);
}
}
