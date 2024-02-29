/*
 * Created by Lucas Gracioso <contact@lbgracioso.net>
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cloysterhpc/functions.h>
#include <cloysterhpc/services/log.h>
#include <cloysterhpc/spack.h>

void Spack::configure()
{
    LOG_INFO("Configuring Spack")

    cloyster::runCommand("git clone -c feature.manyFiles=true "
                         "https://github.com/spack/spack.git /opt/spack");
    cloyster::runCommand("sudo /opt/spack/share/spack/setup-env.sh");
    cloyster::runCommand("spack config add 'modules:default:enable:[tcl]'");
}

void Spack::remove()
{
    LOG_INFO("Removing Spack")

    cloyster::runCommand("rm -rf /opt/spack");
}
