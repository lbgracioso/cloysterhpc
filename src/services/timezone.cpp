//
// Created by Vinícius Ferrão on 11/01/22.
//

#include "log.h"
#include "timezone.h"
#include "../functions.h"
#include <fmt/format.h>

Timezone::Timezone() :
    m_availableTimezones {fetchAvailableTimezones()}
{}

// TODO: Check against m_availableTimezones and throw if not found
void Timezone::setTimezone(std::string_view tz)
{
    m_timezone = tz;
}

std::string_view Timezone::getTimezone() const
{
    return m_timezone;
}

void Timezone::setSystemTimezone()
{
    LOG_DEBUG("Setting system timezone to {}\n", m_timezone);
    cloyster::runCommand(
            fmt::format("timedatectl set timezone {}", m_timezone));
}

std::list<std::string> Timezone::fetchAvailableTimezones()
{
    LOG_TRACE("Fetching available system timezones");
    std::list<std::string> output;

// TODO: Remove this hack
#if __APPLE__
    output.insert(output.end(), {"UTC-3", "GMT", "America/Sao_Paolo"});
#else // Linux or others Unixes
    cloyster::runCommand(
            fmt::format("timedatectl list-timezones --no-pager"), output, true);
#endif

    return output;
}

std::list<std::string> Timezone::getAvailableTimezones() const
{
    return m_availableTimezones;
}

void Timezone::setTimeservers(const std::vector<std::string>& timeservers) {
    m_timeservers.reserve(timeservers.size());

    for (const auto& timeserver : timeservers)
        m_timeservers.emplace_back(timeserver);
}

// TODO: Check for correctness in timeservers (use hostname/IP check)
// TODO: Remove std::stringstream
// std::stringstream does not support string_view
void Timezone::setTimeservers(const std::string& timeservers) {
    std::stringstream stream{timeservers};

    while (stream.good()) {
        std::string substring;
        std::getline(stream, substring, ',');

        // Remove spaces from substring
        substring.erase(std::remove(
                substring.begin(), substring.end(), ' '), substring.end());

        m_timeservers.emplace_back(substring);
    }
}

std::vector<std::string> Timezone::getTimeservers() {
    return m_timeservers;
}
