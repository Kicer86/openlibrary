/*
 * Base configuration class
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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <map>

#include <QStringList>
#include <QVariant>

#include <OpenLibrary/utils_qt/iconfiguration.hpp>
#include <OpenLibrary/utils_qt/utils_qt_export.h>



/**
 * @brief Configuration load/save
 */
struct IConfigStorage
{
    typedef std::map<QStringList, QVariant> Content;

    virtual ~IConfigStorage() = default;

    /**
     * @brief Load configuration
     * @return object containing loaded configuration
     */
    virtual Content load() = 0;

    /**
     * @brief Save configuration
     * @param configuration entries to be stored
     */
    virtual void save(const Content& configuration) = 0;
};


class UTILS_QT_EXPORT Configuration: public IConfiguration
{
    public:
        /**
         * @brief Constructor
         * @param configStorage object responsible to data storage and load
         */
        explicit Configuration(IConfigStorage& configStorage);
        Configuration(const Configuration &) = delete;
        ~Configuration();

        Configuration& operator=(const Configuration &) = delete;

        QVariant getEntry(const QString &) override;
        QStringList getSubEntries(const QString &) override;

        void setEntry(const QString &, const QVariant &) override;
        void setDefaultValue(const QString &, const QVariant &) override;
        void watchFor(const QString & key, const Watcher &) override;

    private:
        class ConfigurationPrivate* const d;
};

#endif // CONFIGURATION_HPP
