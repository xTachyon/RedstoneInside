#ifndef REDI_NBT_EXCEPTIONS
#define REDI_NBT_EXCEPTIONS

#include <stdexcept>
#include <string>
#include <boost/format.hpp>
#include "type.hpp"

namespace redi
{
namespace nbt
{

struct InvalidRootTagException : public std::exception
{
  virtual const char* what() const throw() override
  {
    return "The root tag must be a Tag Compound";
  }
};

struct RangeErrorException : public std::range_error
{
  RangeErrorException(std::size_t index, std::size_t max)
        : std::range_error((boost::format("%1% is out of range. It must be at most %2%") % index % max).str()) {}
  
};

struct InvalidTagTypeException : public std::exception
{
  InvalidTagTypeException(NBTType type)
        : mErrorMessage(std::to_string(static_cast<int>(type)) + " is not a valid NBT type") {}
  
  InvalidTagTypeException(int type)
        : InvalidTagTypeException(static_cast<NBTType>(type)) {}
  
  virtual const char* what() const throw() override
  {
    return mErrorMessage.c_str();
  }

  private:
  
  const std::string mErrorMessage;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_EXCEPTIONS