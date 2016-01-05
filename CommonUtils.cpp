/**
 * @author huahang@me.com
 * @date 2015-10-15
 */

#include "CommonUtils.h"

#include "boost/filesystem.hpp"
#include "glog/logging.h"

#include "pwd.h"
#include "string"
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "vector"

namespace co {
namespace zerus {
namespace thumbgen {

bool
CommonUtils::fileExists(const std::string& path) {
  boost::filesystem::path p(path);
  return boost::filesystem::exists(p);
}

void
CommonUtils::getHomeDir(std::string& path) {
  uid_t uid = getuid();
  passwd pw;
  passwd* pwResult = nullptr;
  std::vector<char> buffer;
  buffer.resize(sysconf(_SC_GETPW_R_SIZE_MAX));
  int rtn = getpwuid_r(uid, &pw, buffer.data(), buffer.size(), &pwResult);
  if (0 != rtn) {
    LOG(ERROR) << "getpwuid_r() failed with code: " << rtn;
    path.clear();
    return;
  }
  if (nullptr == pwResult) {
    LOG(ERROR) << "getpwuid_r() failed: no matching record found";
    path.clear();
    return;
  }
  path = std::string(pw.pw_dir);
}

bool
CommonUtils::isDir(const std::string& path) {
  boost::filesystem::path p(path);
  return boost::filesystem::is_directory(p);
}

bool
CommonUtils::mkdirs(const std::string& path) {
  boost::filesystem::path p(path);
  if (!boost::filesystem::exists(p)) {
    return boost::filesystem::create_directories(path);
  }
  if (!boost::filesystem::is_directory(p)) {
    LOG(ERROR) << "Path: " << p << " not a directory";
    return false;
  }
  return true;
}

void
CommonUtils::split(
  std::vector<std::string>& splits,
  const std::string& input,
  const char separator) {
  split(splits, input.c_str(), separator);
}

void
CommonUtils::split(
  std::vector<std::string>& splits,
  const char* input,
  const char separator) {
  splits.clear();
  if (nullptr == input) {
    return;
  }
  size_t begin = 0;
  size_t inputLenght = strlen(input);
  for (size_t i = 0; i <= inputLenght; ++i) {
    if ((i == inputLenght) || (separator == input[i])) {
      splits.push_back(std::string(input + begin, i - begin));
      begin = i + 1;
    }
  }
}

} // namespace thumbgen
} // namespace zerus
} // namespace co
