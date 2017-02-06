#ifndef SHADERRUNNER_SETTINGS_H
#define SHADERRUNNER_SETTINGS_H

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

#include <QVariant>

class Settings
	{
public:
	static void setSettingsFile(const QString& aFile);
	static const QString& settingsFile();

	static void writeValue(const QString& aSection, const QString& aName, const QVariant& aValue);
	static QVariant readValue(const QString& aSection, const QString& aName, const QVariant& aDefaultValue = QVariant());

	static bool containsSection(const QString& aSection);
	static bool containsValue(const QString& aSection, const QString& aName);

	static void removeSection(const QString& aSection);
	static void removeValue(const QString& aSection, const QString& aName);

	static QStringList sectionItems(const QString& aSection);

protected:
	Settings();

private:
	static QString theSettingsFile;

	};

#endif // SHADERRUNNER_SETTINGS_H
