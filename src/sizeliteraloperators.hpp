#ifndef REDI_LITERALOPERATORS
#define REDI_LITERALOPERATORS

namespace redi
{

struct SizeConstants
{
  static constexpr std::size_t KB = 1 << 10;
  static constexpr std::size_t MB = KB << 10;
  static constexpr std::size_t GB = MB << 10;
  static constexpr std::size_t TB = GB << 10;
  static constexpr std::size_t PB = TB << 10;
  static constexpr std::size_t EB = PB << 10;
};

constexpr std::size_t operator ""_KB(unsigned long long int data)
{
  return data * SizeConstants::KB;
}

constexpr std::size_t operator ""_MB(unsigned long long int data)
{
  return data * SizeConstants::MB;
}

constexpr std::size_t operator ""_GB(unsigned long long int data)
{
  return data * SizeConstants::GB;
}

constexpr std::size_t operator ""_TB(unsigned long long int data)
{
  return data * SizeConstants::TB;
}

constexpr std::size_t operator ""_PB(unsigned long long int data)
{
  return data * SizeConstants::PB;
}

constexpr std::size_t operator ""_EB(unsigned long long int data)
{
  return data * SizeConstants::EB;
}

} // namespace redi

#endif // REDI_LITERALOPERATORS