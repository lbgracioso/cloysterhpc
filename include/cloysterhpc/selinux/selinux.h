/*
 * Created by Lucas Gracioso <contact@lbgracioso.net>
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CLOYSTERHPC_SELINUX_H_
#define CLOYSTERHPC_SELINUX_H_

#include <cloysterhpc/selinux/xCAT/xcatpolicies.h>
#include <cloysterhpc/services/provisioner.h>
#include <string>

class SELinux {
public:
    enum class Mode { Enforcing, Permissive, Disabled };

private:
    SELinux::Mode m_mode = SELinux::Mode::Enforcing;
    void configurexCATPolicies();
    void configurexCATPolicyFile();
    void configurexCATgenimagePolicyFile();
    void configurexCATrsyncPolicyFile();

public:
    void setMode(Mode mode);
    void setMode(const std::string& mode);
    SELinux::Mode getMode();
    void configureProvisioner(ProvisionerType provisioner);
};

#endif // CLOYSTERHPC_SELINUX_H_
