#include <iostream>
#include <fstream>
#include <type_traits>
#include <boost/filesystem.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "literaloperators.hpp"
#include "block.hpp"
#include "binarydata.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  
//  RootTag root, nested;
//  root.name = "root abc";
//  root[1] = TagInt(6);
//  root[2] = TagDouble(87.3426);
//  nested[5] = 6;
//  nested[0] = "7654";
//  root[90] = nested;
//  root[-1] = TagList();
//  root[-5] = *root.clone();
//
//  TagList& lst = root[-1].get().get<NBTType::List>();
//  lst.push(TagFloat(-1));
//  lst.push(TagFloat(-67));
//
//  std::string str = Printer(root);
//
//  std::cout << std::is_same<std::size_t, typename std::vector<int>::size_type>::value;

  while (true)
  {
    std::string name;
    std::getline(std::cin, name);

    BinaryData d;
    d.resize(fs::file_size(name));

    std::ifstream f(name);
    f.read(reinterpret_cast<char*>(&d[0]), fs::file_size(name));

    std::cout << Printer(BytesReader(d)) << "\n\n";
  }

  return 0;
}
