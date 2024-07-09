/*
 * Created by Lucas Gracioso <contact@lbgracioso.net>
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cloysterhpc/selinux/selinux.h"
#include "cloysterhpc/functions.h"
#include "cloysterhpc/services/log.h"

void SELinux::configurexCATPolicies()
{
    LOG_TRACE("Configuring xCAT SELinux policies")
    configurexCATPolicyFile();
    configurexCATgenimagePolicyFile();
    configurexCATrsyncPolicyFile();
}

void SELinux::configurexCATPolicyFile()
{
    // Create SELinux policy file for xCAT
    const std::string file = "/opt/cloysterhpc/xCAT-httpd-read-tftpdir.te";
    cloyster::addStringToFile(file, xCATSELinuxPolicyFile);
    cloyster::setFilePermissions(file);

    // Install SELinux policy file for xCAT
    std::vector<std::string> commands
        = { "checkmodule -M -m -o /opt/cloysterhpc/xCAT-httpd-read-tftpdir.mod "
            "/opt/cloysterhpc/xCAT-httpd-read-tftpdir.te",
              "semodule_package -o /opt/cloysterhpc/xCAT-httpd-read-tftpdir.pp "
              "-m /opt/cloysterhpc/xCAT-httpd-read-tftpdir.mod",
              "semodule -i /opt/cloysterhpc/xCAT-httpd-read-tftpdir.pp",
              "semanage fcontext -a -t httpd_sys_content_t '/install(/.*)?'",
              "restorecon -R /install",
              "rm -rf /opt/cloysterhpc/xCAT-httpd-read-tftpdir*" };

    for (const auto& command : commands) {
        cloyster::runCommand(command);
    }
}

void SELinux::configurexCATgenimagePolicyFile()
{
    // Create SELinux genimage policy file for xCAT
    const std::string file
        = "/opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.te";
    cloyster::addStringToFile(file, xCATgenimageSELinuxPolicyFile);
    cloyster::setFilePermissions(file);

    // Install SELinux genimage policy file for xCAT
    std::vector<std::string> commands = {
        "checkmodule -M -m -o "
        "/opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.mod "
        "/opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.te",
        "semodule_package -o "
        "/opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.pp -m "
        "/opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.mod",
        "semodule -i /opt/cloysterhpc/xCAT-genimage-ldconfig-rw-tmpdir.pp",
        "rm -rf /opt/cloysterhpc/xCAT-genimage-ldconfig*"
    };

    for (const auto& command : commands) {
        cloyster::runCommand(command);
    }
}

void SELinux::configurexCATrsyncPolicyFile()
{

    // Create SELinux rsync policy file for xCAT
    const std::string file = "/opt/cloysterhpc/xCAT-rsync.te";
    cloyster::addStringToFile(file, xCATrsyncSELinuxPolicyFile);
    cloyster::setFilePermissions(file);

    // Install SELinux rsync policy file for xCAT
    std::vector<std::string> commands
        = { "checkmodule -M -m -o /opt/cloysterhpc/xCAT-rsync.mod "
            "/opt/cloysterhpc/xCAT-rsync.te",
              "semodule_package -o /opt/cloysterhpc/xCAT-rsync.pp -m "
              "/opt/cloysterhpc/xCAT-rsync.mod",
              "semodule -i /opt/cloysterhpc/xCAT-rsync.pp",
              "rm -rf /opt/cloysterhpc/xCAT-genimage-ldconfig*" };

    for (const auto& command : commands) {
        cloyster::runCommand(command);
    }
}

void SELinux::configureProvisioner(Cluster::Provisioner provisioner)
{
    switch (provisioner) {
        case Cluster::Provisioner::xCAT:
            configurexCATPolicies();
            break;
    }
}

void SELinux::setMode(SELinux::Mode mode) { m_mode = mode; }

void SELinux::setMode(const std::string& mode)
{
    if (const auto& formattedMode = magic_enum::enum_cast<SELinux::Mode>(
            mode, magic_enum::case_insensitive)) {
        m_mode = formattedMode.value();
    } else {
        throw std::runtime_error(fmt::format("Unsupported mode: {}", mode));
    }
}

SELinux::Mode SELinux::getMode() { return m_mode; }
