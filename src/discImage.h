//
// Created by Vinícius Ferrão on 22/05/22.
//

#ifndef CLOYSTERHPC_DISCIMAGE_H
#define CLOYSTERHPC_DISCIMAGE_H

#include <filesystem>
#include <array>

class DiscImage {
private:
    std::filesystem::path m_path;
    static constexpr auto m_knownImageFilename{std::to_array<const char*>(
            {
                "rhel-8.5-x86_64-dvd.iso",
                "OracleLinux-R8-U5-x86_64-dvd.iso",
                "Rocky-8.5-x86_64-dvd1.iso"
            })};
    bool isKnownImage();

public:
    const std::filesystem::path& getPath() const;
    void setPath(const std::filesystem::path& path);
};

#endif //CLOYSTERHPC_DISCIMAGE_H
