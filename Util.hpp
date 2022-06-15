//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _UTIL_
#define _UTIL_
#include <string>

namespace Util
{
  void getFilenameFromPath(const std::string& path, std::string& filename)
  {
    const auto it = path.find_last_of('/');
    filename = path.substr(it+1, path.size()-it);
  }
}

#endif //_UTIL_
