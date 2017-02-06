// MIT License
//
// Copyright(c) 2017 Piotr 'utak3r' Borys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "settings.h"
#include <QSettings>

QString Settings::theSettingsFile;

Settings::Settings()
	{
	}

void Settings::setSettingsFile(const QString& aFile)
	{
	theSettingsFile = aFile;
	}

const QString& Settings::settingsFile()
	{
	return theSettingsFile;
	}

void Settings::writeValue(const QString& aSection, const QString& aName, const QVariant& aValue)
	{
	if (theSettingsFile.isEmpty())
		return;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.beginGroup(aSection);
	ini.setValue(aName, aValue);
	ini.endGroup();
	ini.sync();
	}

QVariant Settings::readValue(const QString& aSection, const QString& aName, const QVariant& aDefaultValue)
	{
	if (theSettingsFile.isEmpty())
		return aDefaultValue;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.beginGroup(aSection);
	return ini.value(aName, aDefaultValue);
	}

bool Settings::containsSection(const QString& aSection)
	{
	if (theSettingsFile.isEmpty())
		return false;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	return ini.contains(aSection);
	}

bool Settings::containsValue(const QString& aSection, const QString& aName)
	{
	if (theSettingsFile.isEmpty())
		return false;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.beginGroup(aSection);
	return ini.contains(aName);
	}

void Settings::removeSection(const QString& aSection)
	{
	if (theSettingsFile.isEmpty())
		return;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.remove(aSection);
	ini.sync();
	}

void Settings::removeValue(const QString& aSection, const QString& aName)
	{
	if (theSettingsFile.isEmpty())
		return;

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.beginGroup(aSection);
	ini.remove(aName);
	ini.endGroup();
	ini.sync();
	}

QStringList Settings::sectionItems(const QString& aSection)
	{
	if (theSettingsFile.isEmpty())
		return QStringList();

	QSettings ini(theSettingsFile, QSettings::IniFormat);
	ini.beginGroup(aSection);
	return ini.childKeys();
	}
