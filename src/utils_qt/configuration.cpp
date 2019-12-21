/*
 * Configuration's private implementation
 * Copyright (C) 2015  Michał Walenciak <MichalWalenciak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "OpenLibrary/utils_qt/configuration.hpp"
#include "configuration_p.hpp"

#include <QVariant>



ConfigurationPrivate::ConfigurationPrivate(IConfigStorage& configStorage):
    m_configStorage(configStorage),
    m_entries(),
    m_dumpTimer(),
    m_watchers()
{
    m_dumpTimer.setSingleShot(true);
    m_dumpTimer.setInterval(500);

    connect(&m_dumpTimer, &QTimer::timeout, this, &ConfigurationPrivate::saveData);

    loadData();
}


ConfigurationPrivate::~ConfigurationPrivate()
{
    if (m_dumpTimer.isActive())
    {
        m_dumpTimer.stop();
        saveData();
    }
}


QVariant ConfigurationPrivate::getEntry(const QString& entry)
{
    const QStringList entry_processed = entry.split("::");

    auto config = m_entries.lock();
    auto it = config->find(entry_processed);

    const QVariant value = it != config->end()? it->second: QVariant();

    return value;
}


QStringList ConfigurationPrivate::getSubEntries(const QString& entry)
{
    std::set<QString> subentries;

    const QStringList entry_processed = entry.split("::");

    auto config = m_entries.lock();
    auto locked_config = *config;

    for(const auto &[config_entry, value]: locked_config)
    {
        if (config_entry.size() <= entry_processed.size())
            continue;

        bool equal = true;
        auto ce_it = config_entry.cbegin();
        for(auto en_it = entry_processed.cbegin(); en_it != entry_processed.cend(); ++en_it, ++ce_it)
        {
            if (*en_it != *ce_it)
            {
                equal = false;
                break;
            }
        }

        if (equal)
            subentries.insert(*ce_it);
    }

    /// @todo: since Qt 5.14: QStringList(subentries.cbegin(), subentries.cend());

    QStringList list_of_subentries;
    std::copy(subentries.cbegin(), subentries.cend(), std::back_inserter(list_of_subentries));

    return list_of_subentries;
}


void ConfigurationPrivate::setEntry(const QString& entry, const QVariant& entry_value)
{
    const QStringList entry_processed = entry.split("::");

    auto config = m_entries.lock();
    (*config)[entry_processed] = entry_value;

    const auto w_it = m_watchers.find(entry);

    if (w_it != m_watchers.end())
        for (const IConfiguration::Watcher& watcher: w_it->second)
            watcher(entry, entry_value);

    markDataDirty();
}


void ConfigurationPrivate::watchFor(const QString& key, const IConfiguration::Watcher& watcher)
{
    m_watchers[key].push_back(watcher);
}


void ConfigurationPrivate::loadData()
{
    auto locked_config = m_entries.lock();
    *locked_config = m_configStorage.load();
}


void ConfigurationPrivate::markDataDirty()
{
    m_dumpTimer.start();
}


void ConfigurationPrivate::saveData()
{
    auto locked_config = m_entries.lock();
    m_configStorage.save(*locked_config);
}


////////////////////////////////////////////////////////////////////////////////


Configuration::Configuration(IConfigStorage& configStorage):
    d(new ConfigurationPrivate(configStorage))
{

}


Configuration::~Configuration()
{
    delete d;
}


QVariant Configuration::getEntry(const QString& entry)
{
    return d->getEntry(entry);
}


QStringList Configuration::getSubEntries(const QString& entry)
{
    return d->getSubEntries(entry);
}


void Configuration::setEntry(const QString& entry, const QVariant& value)
{
    d->setEntry(entry, value);
}


void Configuration::setDefaultValue(const QString& entry, const QVariant& value)
{
    const QVariant curren_value = getEntry(entry);
    if (curren_value.isNull())
        setEntry(entry, value);
}


void Configuration::watchFor(const QString& key, const Watcher& watcher)
{
    d->watchFor(key, watcher);
}
