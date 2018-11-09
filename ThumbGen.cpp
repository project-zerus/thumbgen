/**
 * @author huahang@me.com
 * @date 2015-10-15
 */

#include "CommonUtils.h"

#include "boost/algorithm/string/predicate.hpp"
#include "boost/filesystem.hpp"
#include "boost/filesystem/convenience.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "algorithm"
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "string"
#include "sys/wait.h"
#include "unistd.h"
#include "vector"

namespace {

DEFINE_bool(
  override,
  false,
  "Override existing thumbs?"
);

DEFINE_string(
  scanpaths,
  "",
  "Comma separated paths to scan. ex: ~/Pictures,~/Desktop"
);

DEFINE_string(
  thumbdir,
  "",
  "Thumb cache dir path."
);

DEFINE_string(
  convert,
  "/usr/bin/convert",
  "Path to scan."
);

void
forkAndConvert(const std::string& path) {
  using co::zerus::thumbgen::CommonUtils;
  std::string thumbCacheDir = FLAGS_thumbdir;
  if (thumbCacheDir.empty()) {
    CommonUtils::getHomeDir(thumbCacheDir);
    if (thumbCacheDir.empty()) {
      LOG(FATAL) << "getHomeDir() failed";
      exit(EXIT_FAILURE);
    }
    thumbCacheDir += "/.thumbgen/cache";
  }
  if (!CommonUtils::mkdirs(thumbCacheDir)) {
    LOG(ERROR) << "Failed to create thumb cache dir: " << thumbCacheDir;
    exit(EXIT_FAILURE);
  }
  std::string thumbFilename(path);
  for (size_t i = 0; i < thumbFilename.length(); ++i) {
    if ('\0' == thumbFilename[i]) {
      break;
    }
    char c = thumbFilename[i];
    if ('/' == c || ' ' == c || '~' == c) {
      thumbFilename[i] = '_';
    }
  }
  thumbFilename = thumbCacheDir + "/" + thumbFilename + ".jpg";
  if (!FLAGS_override && CommonUtils::fileExists(thumbFilename)) {
    return;
  }
  pid_t pid = fork();
  if (pid < 0) {
    LOG(ERROR) << "fork() failed with code: " << pid;
    exit(EXIT_FAILURE);
  }
  if (0 == pid) {
    execl(
      FLAGS_convert.c_str(),
      "thumbgen-convert",
      "-resize",
      "4000x4000>",
      "-quality",
      "90",
      path.c_str(),
      thumbFilename.c_str(),
      NULL
    );
  }
  int stat_loc;
  waitpid(pid, &stat_loc, 0);
}

void
detectScanPaths(std::vector<std::string>& paths, const std::string& scanpaths) {
  using co::zerus::thumbgen::CommonUtils;
  paths.clear();
  CommonUtils::split(paths, FLAGS_scanpaths, ',');
  auto end = paths.end();
  for (auto itr = paths.begin(); itr != end; ++itr) {
    std::string path = *itr;
    if (!CommonUtils::fileExists(path)) {
      LOG(FATAL)
        << "Scan path not exist. "
        << "path: " << path;
      exit(EXIT_FAILURE);
    }
    if (!CommonUtils::isDir(path)) {
      LOG(FATAL)
        << "Scan path not dir. "
        << "path: " << path;
      exit(EXIT_FAILURE);
    }
    LOG(INFO) << "Detected path to scan: " << path;
  }
}

void
processScanPath(const std::string& p) {
  using boost::filesystem::path;
  using boost::filesystem::recursive_directory_iterator;
  using boost::filesystem::system_complete;
  LOG(INFO) << "Processing path: " << p;
  path scanpath(p);
  std::vector<std::string> filenames;
  recursive_directory_iterator itr(scanpath), itrEnd;
  for (; itr != itrEnd; ++itr) {
    path completePath = system_complete(itr->path());
    std::string filenameLowerCase = completePath.generic_string();
    std::transform(
      filenameLowerCase.begin(),
      filenameLowerCase.end(),
      filenameLowerCase.begin(),
      tolower
    );
    if (boost::algorithm::ends_with(filenameLowerCase, ".jpg") ||
        boost::algorithm::ends_with(filenameLowerCase, ".jpeg") ||
        boost::algorithm::ends_with(filenameLowerCase, ".png") ||
        boost::algorithm::ends_with(filenameLowerCase, ".dng")) {
      filenames.push_back(completePath.generic_string());
    }
  }
  size_t size = filenames.size();
  for (size_t i = 0; i < size; ++i) {
    double progress = double(i) / double(size) * 100.0;
    fprintf(stdout, "Progress: %.02f%%\r", progress);
    fflush(stdout);
    forkAndConvert(filenames[i]);
  }
  LOG(INFO) << "Processing complete. Path : " << p;
}

void
processScanPaths(const std::vector<std::string>& paths) {
  auto itr = paths.begin();
  auto end = paths.end();
  for (; itr != end; ++itr) {
    processScanPath(*itr);
  }
}

} // namespace

int main(int argc, char** argv) {
  using boost::filesystem::path;
  using boost::filesystem::recursive_directory_iterator;
  using boost::filesystem::system_complete;
  using co::zerus::thumbgen::CommonUtils;
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::SetCommandLineOption("logtostderr", "true");
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  std::vector<std::string> paths;
  detectScanPaths(paths, FLAGS_scanpaths);
  processScanPaths(paths);
  return EXIT_SUCCESS;
}
