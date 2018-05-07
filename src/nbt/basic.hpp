#pragma once

#include "tag.hpp"
#include "visitor.hpp"

namespace redi::nbt {

template <typename T>
class basic_tag : public tag {
public:
  ~basic_tag() override = default;

  tag& assign(tag&& tag) override;
  tag& assign(const tag& tag) override;

  tag_type get_type() const override;

  tag_unique_ptr clone() const& override;
  tag_unique_ptr move() && override;

  void visit(nbt_visitor& visitor) override;
  void visit(const_nbt_visitor& visitor) const override;

private:
  T& der();
  const T& der() const;
};

template<typename T>
inline void basic_tag<T>::visit(nbt_visitor& visitor) {
  visitor.visit(der());
}

template<typename T>
inline void basic_tag<T>::visit(const_nbt_visitor& visitor) const {
  visitor.visit(der());
}

template<typename T>
inline tag& basic_tag<T>::assign(tag&& tag) {
  return der() = dynamic_cast<T&&>(tag);
}

template<typename T>
inline tag& basic_tag<T>::assign(const tag& tag) {
  return der() = dynamic_cast<const T&>(tag);
}

template<typename T>
inline tag_type basic_tag<T>::get_type() const {
  return T::type;
}

template<typename T>
inline tag_unique_ptr basic_tag<T>::clone() const& {
  return std::make_unique<T>(der());
}

template<typename T>
inline tag_unique_ptr basic_tag<T>::move()&& {
  return std::make_unique<T>(std::move(der()));
}

template<typename T>
inline T& basic_tag<T>::der() {
  return static_cast<T&>(*this);
}

template<typename T>
inline const T& basic_tag<T>::der() const {
  return static_cast<const T&>(*this);
}

} // namespace redi::nbt