//
// Created by Vinícius Ferrão on 24/11/21.
//

#include "presenter.h"

Presenter::Presenter(std::unique_ptr<Newt>& view,
                     std::unique_ptr<Cluster>& model)
                     : m_model(model), m_view(view) {

    welcomeMessage();
    installInstructions();
    m_model->setTimezone(timezoneSelection({"UTC", "GMT-3"}));
    m_model->setLocale(localeSelection({"en_US.UTF-8", "pt_BR.UTF-8", "C"}));
    networkHostnameSelection({"Hostname", "Domain name"});
    networkInterfaceSelection({"eth0", "eth1", "enp4s0f0"});
    networkAddress({"Headnode IP", "Management network"});

    // Destroy the view since we don't need it anymore
    m_view.reset();
}

void Presenter::welcomeMessage() {
    m_view->message(MSG_WELCOME);
}

void Presenter::installInstructions() {
    m_view->okCancelMessage(MSG_GUIDED_INSTALL);
}

std::string Presenter::timezoneSelection(const std::vector<std::string>& timezones) {
    return m_view->listMenu(MSG_TITLE_TIME_SETTINGS,
                            MSG_TIME_SETTINGS_TIMEZONE, timezones,
                            MSG_TIME_SETTINGS_TIMEZONE_HELP);
}

std::string Presenter::localeSelection(const std::vector<std::string>& locales)
{
    return m_view->listMenu(MSG_TITLE_LOCALE_SETTINGS,
                            MSG_LOCALE_SETTINGS_LOCALE, locales,
                            MSG_LOCALE_SETTINGS_LOCALE_HELP);
}

std::vector<std::string>
Presenter::networkHostnameSelection(const std::vector<std::string>& entries)
{
    return m_view->fieldMenu(MSG_TITLE_NETWORK_SETTINGS,
                             MSG_NETWORK_SETTINGS_HOSTID, entries,
                             MSG_NETWORK_SETTINGS_HOSTID_HELP);
}

std::string
Presenter::networkInterfaceSelection(const std::vector<std::string>& interface)
{
    return m_view->listMenu(MSG_TITLE_NETWORK_SETTINGS,
                            MSG_NETWORK_SETTINGS_EXTERNAL_IF, interface,
                            MSG_NETWORK_SETTINGS_EXTERNAL_IF_HELP);
}

std::vector<std::string>
Presenter::networkAddress(const std::vector<std::string>& addresses) {
    return m_view->fieldMenu(MSG_TITLE_NETWORK_SETTINGS,
                            MSG_NETWORK_SETTINGS_INTERNAL_IPV4,
                            addresses,
                            MSG_NETWORK_SETTINGS_INTERNAL_IPV4_HELP);
}