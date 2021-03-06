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

#include "srMainWindow.h"
#include "ui_srMainWindow.h"
#include <GLViewWidget.h>
#include <QFileDialog>
#include <QSplitter>
#include <QProcess>
#include <QBuffer>
#include <QByteArray>
#include <QTemporaryDir>
#include <QTextCodec>
#include "settings.h"
#include "RenderToFileDialog.h"
#include "ProgressDialog.h"

srMainWindow::srMainWindow(QWidget* aParent) :
	QMainWindow(aParent),
	ui(new Ui::srMainWindow),
	theAnimStepOnSlider(0.f)
	{
	ui->setupUi(this);
	decorateSplitter(ui->splitter);
	connectButtons();
	ui->cbxShowFPS->setChecked(false);
	loadConfig();
	}

srMainWindow::~srMainWindow()
	{
	storeConfig();
	delete ui;
	}

void srMainWindow::exitApp()
	{
	close();
	}

void srMainWindow::connectButtons()
	{
	// Editor buttons
	connect(ui->btnSetShader, &QPushButton::clicked, [=]()
		{
		ui->GLWidget->setFragmentShaderSource(ui->textEdit->toPlainText());
		});
	connect(ui->btnOpen, &QPushButton::clicked, this, &srMainWindow::openShaderSource);
	connect(ui->btnSave, &QPushButton::clicked, this, &srMainWindow::saveShaderSource);
	
	// Image buttons
	connect(ui->btnSaveImage, &QPushButton::clicked, this, &srMainWindow::saveBufferToImage);
	connect(ui->btnRenderToFile, &QPushButton::clicked, this, &srMainWindow::renderToImage);
	connect(ui->btnRenderToVideo, &QPushButton::clicked, this, &srMainWindow::renderToVideo);

	// Player buttons
	if (ui->GLWidget->isPlaying())
		ui->btnPlayPause->setIcon(QIcon(QStringLiteral(":/Player/resources/pause.png")));
	else
		ui->btnPlayPause->setIcon(QIcon(QStringLiteral(":/Player/resources/play.png")));
	connect(ui->btnPlayPause, &QPushButton::clicked, [=]()
		{
		if (ui->GLWidget->isPlaying())
			{
			ui->GLWidget->pause();
			ui->btnPlayPause->setIcon(QIcon(QStringLiteral(":/Player/resources/play.png")));
			}
		else
			{
			ui->GLWidget->play();
			ui->btnPlayPause->setIcon(QIcon(QStringLiteral(":/Player/resources/pause.png")));
			}
		});
	connect(ui->btnRewind, &QPushButton::clicked, [=]()
		{
		ui->GLWidget->rewind();
		});
	connect(ui->btnAspect169, &QPushButton::clicked, [=]()
		{
		ui->GLWidget->setAspectRatio(16.f / 9.f);
		});
	connect(ui->btnAspect43, &QPushButton::clicked, [=]()
		{
		ui->GLWidget->setAspectRatio(4.f / 3.f);
		});
	connect(ui->btnAspectSquare, &QPushButton::clicked, [=]()
		{
		ui->GLWidget->setAspectRatio(1.f);
		});
	connect(ui->cbxShowFPS, &QCheckBox::toggled, [=](bool aChecked)
		{
		ui->GLWidget->setShowFPS(aChecked);
		});

	// Animation time slider	
	connect(ui->sldAnimTime, &QSlider::sliderPressed, [=]()
		{
		theAnimStepOnSlider = ui->GLWidget->animationStep();
		});
	connect(ui->sldAnimTime, &QSlider::sliderMoved, [=](int aValue)
		{
		ui->GLWidget->setAnimationStep((aValue != 0) ? aValue / 5.f : theAnimStepOnSlider);
		});
	connect(ui->sldAnimTime, &QSlider::sliderReleased, [=]()
		{
		ui->sldAnimTime->setValue(0);
		ui->GLWidget->setAnimationStep(theAnimStepOnSlider);
		});
	}

void srMainWindow::decorateSplitter(QSplitter* splitter)
	{
	splitter->setHandleWidth(10);
	QSplitterHandle *handle = splitter->handle(1);

	QHBoxLayout *layout = new QHBoxLayout(handle);
	layout->setSpacing(0);
	layout->setMargin(0);

	for (int i = 0; i < 2; ++i)
		{

		QFrame *line = new QFrame(handle);
		line->setFrameShape(QFrame::VLine);
		line->setFrameShadow(QFrame::Sunken);
		line->setLineWidth(1);
		layout->addWidget(line);
		}
	}

void srMainWindow::loadConfig()
	{
	if (Settings::containsValue(QStringLiteral("Window"), QStringLiteral("Geometry")))
		restoreGeometry(Settings::readValue(QStringLiteral("Window"), QStringLiteral("Geometry")).toByteArray());
	if (Settings::containsValue(QStringLiteral("Window"), QStringLiteral("State")))
		restoreState(Settings::readValue(QStringLiteral("Window"), QStringLiteral("State")).toByteArray());

	if (Settings::containsValue(QStringLiteral("Window"), QStringLiteral("Splitter_geometry")))
		ui->splitter->restoreGeometry(Settings::readValue(QStringLiteral("Window"), QStringLiteral("Splitter_geometry")).toByteArray());
	if (Settings::containsValue(QStringLiteral("Window"), QStringLiteral("Splitter_state")))
		ui->splitter->restoreState(Settings::readValue(QStringLiteral("Window"), QStringLiteral("Splitter_state")).toByteArray());

	if (Settings::containsValue(QStringLiteral("Preview"), QStringLiteral("AspectRatio")))
		ui->GLWidget->setAspectRatio(Settings::readValue(QStringLiteral("Preview"), QStringLiteral("AspectRatio")).toDouble());
	if (Settings::containsValue(QStringLiteral("Preview"), QStringLiteral("ShowFPS")))
		ui->cbxShowFPS->setChecked(Settings::readValue(QStringLiteral("Preview"), QStringLiteral("ShowFPS")).toBool());
	}

void srMainWindow::storeConfig()
	{
	Settings::writeValue(QStringLiteral("Window"), QStringLiteral("Geometry"), saveGeometry());
	Settings::writeValue(QStringLiteral("Window"), QStringLiteral("State"), saveState());
	Settings::writeValue(QStringLiteral("Window"), QStringLiteral("Splitter_geometry"), ui->splitter->saveGeometry());
	Settings::writeValue(QStringLiteral("Window"), QStringLiteral("Splitter_state"), ui->splitter->saveState());
	
	Settings::writeValue(QStringLiteral("Preview"), QStringLiteral("AspectRatio"), ui->GLWidget->aspectRatio());
	Settings::writeValue(QStringLiteral("Preview"), QStringLiteral("ShowFPS"), ui->GLWidget->showFPS());
	}

void srMainWindow::openShaderSource()
	{
	QString fileName = QLatin1Literal("");
#ifdef Q_OS_WIN
	fileName = QFileDialog::getOpenFileName(this, tr("Open shader from file..."), QLatin1Literal("."), tr("Shader source (*.glsl)"));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	fileName = QFileDialog::getOpenFileName(this, tr("Open shader from file..."), QLatin1Literal("."), tr("Shader source (*.glsl)"), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
#endif // Q_OS_LINUX
	if (fileName != QLatin1Literal(""))
		{
		QFile file(fileName);
		if (file.open(QFile::ReadOnly | QFile::Text))
			{
			ui->textEdit->clear();
			QTextStream in(&file);
			while (!in.atEnd())
				{
				ui->textEdit->append(in.readLine());
				}
			file.close();
			}
		}
	}

void srMainWindow::saveShaderSource()
	{
	QString fileName = QLatin1Literal("");
#ifdef Q_OS_WIN
	fileName = QFileDialog::getSaveFileName(this, tr("Save shader to file..."), QLatin1Literal("."), tr("Shader source (*.glsl)"));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	fileName = QFileDialog::getSaveFileName(this, tr("Save shader to file..."), QLatin1Literal("."), tr("Shader source (*.glsl)"), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
#endif // Q_OS_LINUX
	if (fileName != QLatin1Literal(""))
		{
		QFile file(fileName);
		if (file.open(QFile::WriteOnly | QFile::Text))
			{
			QTextStream out(&file);
			out << ui->textEdit->toPlainText();
			out.flush();
			file.close();
			}
		}
	}

void srMainWindow::saveBufferToImage()
	{
	QString fileName = QLatin1Literal("");
#ifdef Q_OS_WIN
	fileName = QFileDialog::getSaveFileName(this, tr("Save image to file..."), QLatin1Literal("."), tr("Image Files (*.png *.jpg *.bmp)"));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	fileName = QFileDialog::getSaveFileName(this, tr("Save image to file..."), QLatin1Literal("."), tr("Image Files (*.png *.jpg *.bmp)"), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
#endif // Q_OS_LINUX
	if (fileName != QLatin1Literal(""))
		{
		QImage img = ui->GLWidget->grabFramebuffer();
		img.save(fileName);
		}
	}

void srMainWindow::renderToImage()
	{
	RenderToFileDialog dlg;
	dlg.setVideoOptionsVisible(false);
	dlg.setAspectRatio(ui->GLWidget->aspectRatio());
	dlg.setFilename(QLatin1Literal("render.jpg"));
	if ((QDialog::DialogCode)dlg.exec() == QDialog::Accepted)
		{
		QString filename = dlg.filename();
		QSize size = dlg.size();
		double oversampling = dlg.oversampling();
		QImage img = ui->GLWidget->renderOffscreen(size*oversampling);
		if (oversampling > 1)
			img = img.scaled(size);
		img.save(dlg.filename());
		}
	}

void srMainWindow::renderToVideo()
	{
	RenderToFileDialog dlg;
	dlg.setVideoOptionsVisible(true);
	dlg.setAspectRatio(ui->GLWidget->aspectRatio());
	if (Settings::containsValue(QStringLiteral("Video"), QStringLiteral("Framerate")))
		dlg.setFramerate(Settings::readValue(QStringLiteral("Video"), QStringLiteral("Framerate")).toDouble());
	if (Settings::containsValue(QStringLiteral("Video"), QStringLiteral("ffmpeg")))
		dlg.setFfmpegPath(Settings::readValue(QStringLiteral("Video"), QStringLiteral("ffmpeg")).toString());
	dlg.setFilename(QLatin1Literal("render.mp4"));
	dlg.setSize(QSize(1280,720));

	if ((QDialog::DialogCode)dlg.exec() == QDialog::Accepted)
		{
		QString filename = dlg.filename();
		QSize size = dlg.size();
		double oversampling = dlg.oversampling();
		double framerate = dlg.framerate();
		Settings::writeValue(QStringLiteral("Video"), QStringLiteral("Framerate"), framerate);
		Settings::writeValue(QStringLiteral("Video"), QStringLiteral("ffmpeg"), dlg.ffmpegPath());
		int frames = dlg.duration() * framerate;
		QString resolution = QStringLiteral("%1x%2").arg(size.width()).arg(size.height());

		QTemporaryDir dir;
		if (dir.isValid())
			{
			ProgressDialog progress;
			progress.setTitle(QLatin1Literal("Rendering individual frames..."));
			progress.setRange(0, frames);
			progress.setValue(0);
			progress.show();

			QString dirpath = dir.path();
			qDebug() << "Temporary dir:" << dirpath;
			for (int i = 0; i < frames; ++i)
				{
				progress.setValue(i);
				progress.addLog(QStringLiteral("Rendering frame %1 out of %2...").arg(i+1).arg(frames));
				QImage img = ui->GLWidget->renderOffscreen(size*oversampling);
				if (oversampling > 1)
					img = img.scaled(size);
				QString filename = QStringLiteral("%1/frame%2.ppm").arg(dirpath).arg(i + 1, 6, 10, QLatin1Char('0'));
				qDebug() << "Filename:" << filename;
				img.save(filename, "PPM");
				ui->GLWidget->jogAnimation(1.0 / framerate);
				qDebug() << "Image" << i + 1 << "saved.";				
				}

			QString encodingexe = dlg.ffmpegPath();
			QStringList encodingArgs;
			encodingArgs << QStringLiteral("-r") << QStringLiteral("%1").arg(framerate, 0, 'g', 2);
			encodingArgs << QStringLiteral("-f") << QStringLiteral("image2");
			encodingArgs << QStringLiteral("-s") << resolution;
			encodingArgs << QStringLiteral("-i") << QStringLiteral("%1/frame%06d.ppm").arg(dirpath);
			encodingArgs << QStringLiteral("-c:v") << QStringLiteral("libx264");
			encodingArgs << QStringLiteral("-preset") << QStringLiteral("medium");
			encodingArgs << QStringLiteral("-tune") << QStringLiteral("animation");
			encodingArgs << QStringLiteral("-y");
			encodingArgs << filename;
			qDebug() << "ffmpeg args:" << encodingArgs;

			progress.setTitle(QLatin1Literal("Encoding video..."));
			progress.setRange(0, 0);
			progress.setValue(0);
			QProcess *encoder = new QProcess(this);
			encoder->setProcessChannelMode(QProcess::MergedChannels);
			connect(encoder, &QProcess::readyReadStandardOutput, &progress, &ProgressDialog::readStandardOutput);
			connect(encoder, &QProcess::readyReadStandardError, &progress, &ProgressDialog::readStandardError);
			encoder->start(encodingexe, encodingArgs);
			encoder->waitForStarted(2000);
			if (encoder->waitForFinished())
				qDebug() << encoder->readAll();

			dir.remove();
			qDebug() << "Video encoding done.";
			progress.close();
			}

		}
	}
