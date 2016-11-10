#ifndef REDI_COMPRESSOR_HPP
# define REDI_COMPRESSOR_HPP

namespace redi
{
namespace compressor
{

enum : int
{
  /*
  * Not a very good idea
  * but I don't want to include
  * boost here.
  */

  NoCompression = 0,
  Default = -1,
  BestSpeed = 1,
  BestCompression = 9
};

//BinaryData compresszlib(const BinaryData& data, int level = BestCompression);
//BinaryData decompresszlib(const BinaryData& data);
//
//BinaryData compressgzip(const BinaryData& data, int level = BestCompression);
//BinaryData decompresszgzip(const BinaryData& data);

} // namespace compressor
} // namespace redi

#endif // REDI_COMPRESSOR_HPP