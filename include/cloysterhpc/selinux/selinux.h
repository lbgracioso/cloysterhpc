/*
 * Created by Lucas Gracioso <contact@lbgracioso.net>
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CLOYSTERHPC_SELINUX_H_
#define CLOYSTERHPC_SELINUX_H_

#include <cloysterhpc/cluster.h>
#include <cloysterhpc/selinux/xCAT/xcatpolicies.h>
#include <string>

class SELinux {
public:
    enum class Mode {
        Enforcing,
        Permissive,
        Disabled
    };
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
    void configureProvisioner(Cluster::Provisioner provisioner); // Não é possível, visto que não posso incluir cluster.h aqui ao mesmo tempo que incluo selinux.h no cluster.h
};

#endif // CLOYSTERHPC_SELINUX_H_
