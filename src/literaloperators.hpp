#ifndef REDI_LITERALOPERATORS
# define REDI_LITERALOPERATORS

namespace redi
{
namespace nbt
{

struct SizeConstants
{
  static constexpr std::size_t kb = 1 << 10;
  static constexpr std::size_t mb = kb << 10;
  static constexpr std::size_t gb = mb << 10;
  static constexpr std::size_t tb = gb << 10;
  static constexpr std::size_t pb = tb << 10;
  static constexpr std::size_t eb = pb << 10;
};

std::size_t operator ""_kb(unsigned long long int data)
{
  return data * SizeConstants::kb;
}

std::size_t operator ""_mb(unsigned long long int data)
{
  return data * SizeConstants::mb;
}

std::size_t operator ""_gb(unsigned long long int data)
{
  return data * SizeConstants::gb;
}

std::size_t operator ""_tb(unsigned long long int data)
{
  return data * SizeConstants::tb;
}

std::size_t operator ""_pb(unsigned long long int data)
{
  return data * SizeConstants::pb;
}

std::size_t operator ""_eb(unsigned long long int data)
{
  return data * SizeConstants::eb;
}

} // namespace nbt
} // namespace redi


#endif // REDI_LITERALOPERATORS