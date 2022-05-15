/* Copyright to be added
 * Description to be added
 * Authores to be added
 */

#ifndef CONST_H
#define CONST_H

// We strive for constexpr correctness, but we sometimes still need a macro
namespace cloyster {

#define PRODUCT_NAME "CloysterHPC"
constexpr const char* productName = PRODUCT_NAME;

#define INSTALL_PATH "/opt/cloysterhpc"
constexpr const char* installPath = INSTALL_PATH;

#if defined(_DUMMY_) || defined(__APPLE__)
#define CHROOT "chroot"
[[maybe_unused]] constexpr const char* chroot = CHROOT;
#else
#define CHROOT
#endif

} // namespace cloyster

using cloyster::productName;
using cloyster::installPath;

#endif /* CONST_H */
