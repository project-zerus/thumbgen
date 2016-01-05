/**
 * @author huahang@me.com
 * @date 2015-10-15
 */

#include "string"
#include "vector"

namespace co {
namespace zerus {
namespace thumbgen {

class CommonUtils {
public:
  static bool fileExists(const std::string& path);

  static void getHomeDir(std::string& path);

  static bool isDir(const std::string& path);

  static bool mkdirs(const std::string& path);

  static void split(
    std::vector<std::string>& splits,
    const std::string& input,
    const char separator
  );

  static void split(
    std::vector<std::string>& splits,
    const char* input,
    const char separator
  );
};

} // namespace thumbgen
} // namespace zerus
} // namespace co
