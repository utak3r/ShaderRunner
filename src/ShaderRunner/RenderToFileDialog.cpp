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

#include "RenderToFileDialog.h"
#include "ui_RenderToFileDialog.h"
#include <QFileDialog>

RenderToFileDialog::RenderToFileDialog(QWidget *aParent, Qt::WindowFlags aFlags)
	:
	QDialog(aParent, aFlags),
	ui(new Ui::RenderToFileDialog),
	theAspectRatio(16.f/9.f)
	{
	ui->setupUi(this);
	setTitle(QLatin1Literal("Render to file..."));
	connect(ui->txtWidth, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RenderToFileDialog::sizeValueChanged);
	connect(ui->txtHeight, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RenderToFileDialog::sizeValueChanged);
	connect(ui->cbxKeepAspect, &QCheckBox::toggled, this, &RenderToFileDialog::keepAspectRatioToggled);
	connect(ui->btnBrowse, &QPushButton::clicked, this, &RenderToFileDialog::browseForFile);
	connect(ui->btnFindFfmpeg, &QPushButton::clicked, this, &RenderToFileDialog::browseForFfmpeg);
	}

RenderToFileDialog::~RenderToFileDialog()
	{
	delete ui;
	}

void RenderToFileDialog::setTitle(const QString& aTitle)
	{
	setWindowTitle(aTitle);
	}

QString RenderToFileDialog::filename() const
	{
	return ui->txtFilename->text();
	}

void RenderToFileDialog::setFilename(const QString& aFilename)
	{
	ui->txtFilename->setText(aFilename);
	}

QSize RenderToFileDialog::size() const
	{
	return QSize(ui->txtWidth->value(), ui->txtHeight->value());
	}

void RenderToFileDialog::setSize(const QSize& aSize)
	{
	ui->txtWidth->setValue(aSize.width());
	ui->txtHeight->setValue(aSize.height());
	}

void RenderToFileDialog::sizeValueChanged(int aValue)
	{
	if (ui->cbxKeepAspect->isChecked())
		{
		QSpinBox *another = nullptr;
		if (qobject_cast<QSpinBox *>(sender()) == ui->txtHeight)
			{
			another = ui->txtWidth;
			another->blockSignals(true);
			another->setValue(aValue * theAspectRatio);
			another->blockSignals(false);
			}
		if (qobject_cast<QSpinBox *>(sender()) == ui->txtWidth)
			{
			another = ui->txtHeight;
			another->blockSignals(true);
			another->setValue(aValue / theAspectRatio);
			another->blockSignals(false);
			}		
		}
	}

void RenderToFileDialog::setAspectRatio(const double anAspect)
	{
	theAspectRatio = anAspect;
	ui->txtHeight->setValue(ui->txtHeight->value() + 1);
	ui->txtHeight->setValue(ui->txtHeight->value() - 1);
	}

void RenderToFileDialog::keepAspectRatioToggled(bool aChecked)
	{
	if (aChecked)
		setAspectRatio(theAspectRatio);
	}

void RenderToFileDialog::setVideoOptionsVisible(bool aVisible)
	{
	if (aVisible)
		ui->grpVideoOptions->show();
	else
		ui->grpVideoOptions->hide();
	}

double RenderToFileDialog::framerate() const
	{
	return ui->txtFramerate->value();
	}

void RenderToFileDialog::setFramerate(double aFramerate)
	{
	if (aFramerate > 0)
		ui->txtFramerate->setValue(aFramerate);
	}

double RenderToFileDialog::duration() const
	{
	return ui->txtDuration->value();
	}

void RenderToFileDialog::setDuration(double aDuration)
	{
	if (aDuration > 0)
		ui->txtDuration->setValue(aDuration);
	}

QString RenderToFileDialog::ffmpegPath() const
	{
	return ui->txtFfmpeg->text();
	}

void RenderToFileDialog::setFfmpegPath(const QString& aPath)
	{
	ui->txtFfmpeg->setText(aPath);
	}

double RenderToFileDialog::oversampling() const
	{
	return ui->txtOversampling->value();
	}

void RenderToFileDialog::setOversampling(double aValue)
	{
	ui->txtOversampling->setValue(aValue);
	}

void RenderToFileDialog::browseForFile()
	{
	bool video = false;
	QString title = tr("Save image to...");
	QString filter = tr("Image files (*.jpg)");
	if (ui->grpVideoOptions->isVisible())
		{
		video = true;
		title = tr("Save video to...");
		filter = tr("Video files (*.mp4)");
		}
#ifdef Q_OS_WIN
	setFilename(QFileDialog::getSaveFileName(this, title, QLatin1Literal("."), filter));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	setFilename(QFileDialog::getSaveFileName(this, title, QLatin1Literal("."), filter, Q_NULLPTR, QFileDialog::DontUseNativeDialog));
#endif // Q_OS_LINUX
	}

void RenderToFileDialog::browseForFfmpeg()
	{
	QString title = tr("Find the ffmpeg executable");
#ifdef Q_OS_WIN
	QString filter = tr("Executable files (*.exe)");
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	QString filter = tr("Executable files (*)");
#endif // Q_OS_LINUX
#ifdef Q_OS_WIN
	setFfmpegPath(QFileDialog::getOpenFileName(this, title, QLatin1Literal("."), filter));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	setFfmpegPath(QFileDialog::getOpenFileName(this, title, QLatin1Literal("."), filter, Q_NULLPTR, QFileDialog::DontUseNativeDialog));
#endif // Q_OS_LINUX
	}

