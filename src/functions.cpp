#include "functions.h"

#include <cstdlib> /* getenv() */
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/process.hpp>
#include <fmt/format.h>

#include "services/log.h"

namespace cloyster {

CommandProxy runCommandIter(const std::string& command, bool overrideDryRun)
{
    if (!cloyster::dryRun || overrideDryRun) {
        LOG_TRACE("Running command: {}", command);
        boost::process::ipstream pipe_stream;
        boost::process::child child(command, boost::process::std_out > pipe_stream);

        return CommandProxy{.valid = true,
                            .child = std::move(child),
                            .pipe_stream = std::move(pipe_stream)};
    }

    return CommandProxy{};
}


// FIXME: Maybe std::optional here is irrelevant? Look at the next overload.
int runCommand(const std::string& command,
               //std::optional<std::list<std::string>>& output,
               std::list<std::string>& output,
               bool overrideDryRun) {

    if (!cloyster::dryRun || overrideDryRun) {
        LOG_TRACE("Running command: {}", command);
        boost::process::ipstream pipe_stream;
        boost::process::child child(command, boost::process::std_out > pipe_stream);

        std::string line;

        while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) {
            LOG_TRACE("{}", line);
            output.emplace_back(line);
        }

        child.wait();
        LOG_TRACE("Exit code: {}", child.exit_code());
        return child.exit_code();
    }
    else
    {
        LOG_WARN("Dry Run: {}", command);
        return 0;
    }
}

int runCommand(const std::string& command, bool overrideDryRun) {
    // FIXME: Why we can't pass std::nullopt instead?
    std::list<std::string> discard;
    //std::optional<std::list<std::string>> discard;
    return runCommand(command, discard, overrideDryRun);
}

/* Returns a specific environment variable when requested.
* If the variable is not set it will return as an empty string. That's by
* design and not considered a bug right now.
*/
std::string getEnvironmentVariable(const std::string &key) {
    char *value = getenv(key.c_str());
    return value == nullptr ? std::string("") : std::string(value);
}

/* Read .conf file */
std::string readConfig(const std::string &filename) {
    boost::property_tree::ptree tree;

    try {
        boost::property_tree::ini_parser::read_ini(filename, tree);
    }

    catch (boost::property_tree::ini_parser_error& ex) {
        LOG_ERROR("Error: {}", ex.what())
    }

    std::string value = tree.get<std::string>("headnode.LANG",
                                              "en_US.utf8");

    LOG_TRACE("Read configFile variables:")
    LOG_TRACE("LANG: {}", value)

    return value;
}

/* Write .conf file function */
void writeConfig(const std::string &filename) {
    boost::property_tree::ptree tree;

    tree.put("headnode.LANG", getEnvironmentVariable("LANG"));

    boost::property_tree::write_ini(filename, tree);
}

void createDirectory(const std::filesystem::path& path) {
    std::filesystem::create_directories(path);
    LOG_TRACE("Created directory: {}", path.string());
}

/* Remove file */
void removeFile(std::string_view filename) {
    LOG_TRACE("Checking if file {} already exists on filesystem:", filename);
    if (std::filesystem::exists(filename)) {
        LOG_TRACE("Already exists");
        std::filesystem::remove(filename);
        LOG_TRACE("File {} deleted", filename)
    } else {
        LOG_TRACE("File does not exist");
    }
}

/* Backup file */
void backupFile(const std::string_view &filename) {
    const auto& backupFile = fmt::format("{}/backup{}", installPath, filename);

    // Create the directory structure
    createDirectory(std::filesystem::absolute(backupFile).parent_path());

    std::fstream file(backupFile);
    if (!file.is_open()) {
        // Backup the file
        std::filesystem::copy_file(filename, backupFile);
        LOG_TRACE("Created a backup copy of {} on {}", filename, backupFile);
    }
}

/* TODO:
 *  - Do not treat .conf files as .ini files!
 *  - Discover why boost removes comments on files while parsing INI config
 *  - Replace boost with glib's GKeyFile?
 *  - http://www.gtkbook.com/gtkbook/keyfile.html
 */
void changeValueInConfigurationFile(const std::string& filename,
                                    const std::string& key,
                                    std::string_view value)
{
    boost::property_tree::ptree tree;

    try {
        boost::property_tree::ini_parser::read_ini(filename, tree);
    } catch(boost::property_tree::ini_parser_error& ex) {
        LOG_ERROR("Error: {}", ex.what());
    }

    tree.put(key, value);
    boost::property_tree::write_ini(filename, tree);
}

void addStringToFile(std::string_view filename, std::string_view string) {
#ifdef _LIBCPP_VERSION
    std::ofstream file(filename, std::ios_base::app);
#else
    std::ofstream file(std::string{filename}, std::ios_base::app);
#endif

    if (!file.is_open())
        throw std::runtime_error(
                fmt::format("Error opening file: {}", filename));

    file << string;
    LOG_TRACE("Added line(s):\n{}\n => to file: {}", string, filename);
}

} // namespace cloyster
