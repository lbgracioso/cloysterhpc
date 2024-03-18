#ifndef CLOYSTERHPC_FUNCTIONS_H_
#define CLOYSTERHPC_FUNCTIONS_H_

#include <boost/process/child.hpp>
#include <boost/process/pipe.hpp>
#include <filesystem>
#include <list>
#include <optional>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <fmt/format.h>

namespace cloyster {
// Globals
extern bool dryRun;

/**
 * A command proxy, to us to be able to get the
 * command output while the command is running
 *
 * We will use this in the progress dialog
 */
struct CommandProxy {
    bool valid = false;
    boost::process::child child;
    boost::process::ipstream pipe_stream;

    std::optional<std::string> getline()
    {
        if (!valid)
            return std::nullopt;

        std::string line;

        while (pipe_stream.good()) {
            if (std::getline(pipe_stream, line)) {
                return std::make_optional(line);
            }
        }

        if (!pipe_stream.good()) {
            this->valid = false;
        }

        return std::nullopt;
    }
};

/* shell execution */
int runCommand(const std::string& command,
    // std::optional<std::list<std::string>>& output,
    std::list<std::string>& output, bool overrideDryRun = false);
int runCommand(const std::string& command, bool overrideDryRun = false);

CommandProxy runCommandIter(
    const std::string& command, bool overrideDryRun = false);

/* environment variables helper functions */
std::string getEnvironmentVariable(const std::string&);

/* conf manipulation functions */
std::string readConfig(const std::string&);
void writeConfig(const std::string&);

/* helper function */
void createDirectory(const std::filesystem::path& path);
void removeFile(std::string_view filename);
void backupFile(std::string_view filename);
void changeValueInConfigurationFile(
    const std::string&, const std::string&, std::string_view);
void addStringToFile(std::string_view filename, std::string_view string);

/* convert string, string_view and others to filesystem::path easily */
template <typename FilePath>
std::filesystem::path handlePath(FilePath&& path, bool shouldExist = false)
{
    std::filesystem::path pathToFile;

    try {
        pathToFile = std::forward<FilePath>(path);
    } catch (...) {
        throw std::invalid_argument("Unsupported path object type");
    }

    if (shouldExist && !std::filesystem::exists(pathToFile)) {
        throw std::invalid_argument(fmt::format(
            "File '{}' doesn't exist", pathToFile.filename().c_str()));
    }

    return pathToFile;
}

} /* namespace cloyster */

#endif // CLOYSTERHPC_FUNCTIONS_H_
