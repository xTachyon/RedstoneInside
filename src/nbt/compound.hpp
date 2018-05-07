#pragma once

#include <map>
#include "basic.hpp"
#include "value.hpp"
#include "prettyprinter.hpp"

namespace redi::nbt {

class tag_compound : public basic_tag<tag_compound> {
public:
  using map_type = std::map<std::string, tag_value>;

  using reference = map_type::reference;
  using const_reference = map_type::const_reference;
  using iterator = map_type::iterator;
  using const_iterator = map_type::const_iterator;
  using difference_type = map_type::difference_type;
  using size_type = map_type::size_type;
  using value_type = map_type::value_type;

  using reverse_iterator = map_type::reverse_iterator;
  using const_reverse_iterator = map_type::const_reverse_iterator;

  static constexpr tag_type type = tag_type::type_compound;

  tag_compound() = default;
  tag_compound(const tag_compound& other);
  tag_compound(tag_compound&&) noexcept = default;
  tag_compound(std::initializer_list<value_type> list);

  ~tag_compound() override = default;

  tag_compound& operator=(const tag_compound&);
  tag_compound& operator=(tag_compound&&) = default;

  tag_value& operator[](const std::string& key);
  tag_value& operator[](std::string&& key);
//
//  template <typename T>
//  void set(nbt_string_view key, const T& x) {
//    emplace(key, x);
//  }

  tag_value& at(const std::string& key) { return map.at(key); }
  const tag_value& at(const std::string& key) const {
#ifdef REDI_DEBUG
    if (!exists(key)) {
      throw std::runtime_error("map.at(" + key + ')');
    }
#endif
    return map.at(key);
  }

  void clear() { map.clear(); }
  std::size_t size() const override { return map.size(); }
  bool empty() const { return map.empty(); }
  void swap(tag_compound& other) { map.swap(other.map); }
  bool exists(const std::string& key) const { return map.count(key) == 1; }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    return map.emplace(std::forward<Args>(args)...);
  }

  iterator begin() { return map.begin(); }
  const_iterator begin() const { return map.begin(); }
  const_iterator cbegin() const { return map.cbegin(); }

  iterator end() { return map.end(); }
  const_iterator end() const { return map.end(); }
  const_iterator cend() const { return map.cend(); }

  reverse_iterator rbegin() { return map.rbegin(); }
  const_reverse_iterator rbegin() const { return map.rbegin(); }
  const_reverse_iterator crbegin() const { return map.crbegin(); }

  reverse_iterator rend() { return map.rend(); }
  const_reverse_iterator rend() const { return map.rend(); }
  const_reverse_iterator crend() const { return map.crend(); }

private:
  map_type map;
};

} // namespace redi::nbt