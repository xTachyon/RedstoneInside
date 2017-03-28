#ifndef REDI_EXCEPTIONS_HPP
#define REDI_EXCEPTIONS_HPP

#include <stdexcept>

namespace redi {

struct StopServer : public std::exception {
  virtual const char* what() const throw() override { return "Server stop"; }
};

} // namespace redi

#endif // REDI_EXCEPTIONS_HPP
