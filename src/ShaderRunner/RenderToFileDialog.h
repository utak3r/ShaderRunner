#ifndef SHADERRUNNER_RENDERTOFILE_DLG_H
#define SHADERRUNNER_RENDERTOFILE_DLG_H

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

#include <QDialog>

namespace Ui
	{
	class RenderToFileDialog;
	}

class RenderToFileDialog : public QDialog
	{
	Q_OBJECT

	Q_PROPERTY(QString filename READ filename WRITE setFilename)
	Q_PROPERTY(QSize size READ size WRITE setSize)
	Q_PROPERTY(double aspectRatio READ aspectRatio WRITE setAspectRatio)

public:
	explicit RenderToFileDialog(QWidget *aParent = Q_NULLPTR, Qt::WindowFlags aFlags = Qt::WindowFlags());
	~RenderToFileDialog();

	void setTitle(const QString& aTitle);
	QString filename() const;
	void setFilename(const QString& aFilename);
	QSize size() const;
	void setSize(const QSize& aSize);
	double aspectRatio() const { return theAspectRatio; }
	void setAspectRatio(const double anAspect);
	void setVideoOptionsVisible(bool aVisible);
	double framerate() const;
	void setFramerate(double aFramerate);
	double duration() const;
	void setDuration(double aDuration);
	QString ffmpegPath() const;
	void setFfmpegPath(const QString& aPath);
	double oversampling() const;
	void setOversampling(double aValue);

protected slots:
	void sizeValueChanged(int aValue);
	void keepAspectRatioToggled(bool aChecked);
	void browseForFile();
	void browseForFfmpeg();

private:
	Ui::RenderToFileDialog *ui;
	double theAspectRatio;
	};

#endif // SHADERRUNNER_RENDERTOFILE_DLG_H
