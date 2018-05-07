#pragma once

#include <vector>
#include "basic.hpp"

namespace redi::nbt {

class tag_list : public basic_tag<tag_list> {
public:
  static constexpr tag_type type = tag_type::type_list;

  tag_list() = default;
  tag_list(const tag_list& other);
  tag_list(tag_list&& other) = default;

  tag_list& operator=(const tag_list& other);
  tag_list& operator=(tag_list&& other) = default;

  tag_value& operator[](std::size_t pos) { return data[pos]; }
  const tag_value& operator[](std::size_t pos) const { return data[pos]; }

  void push_back(const tag_value& value) { data.push_back(value); }
  void push_back(tag_value&& value) { data.push_back(std::move(value)); }
  void push_back(tag&& tag) { push_back(tag_value(std::move(tag).move())); }

  void pop_back() { data.pop_back(); }

  std::size_t size() const override { return data.size(); }
  bool empty() const { return data.empty(); }

  tag_type getListType() const;

  const std::vector<tag_value>& get() const { return data; }
  std::vector<tag_value>& getData() { return data; } // Use at own risk
  const std::vector<tag_value>& getData() const { return data; }

private:
  std::vector<tag_value> data;
};

} // namespace redi::nbt