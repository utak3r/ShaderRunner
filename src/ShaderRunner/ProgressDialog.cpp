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

#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include <QProcess>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>

ProgressDialog::ProgressDialog(QWidget *aParent, Qt::WindowFlags aFlags)
	:
	QDialog(aParent, aFlags),
	ui(new Ui::ProgressDialog)
	{
	ui->setupUi(this);
	setTitle(QLatin1Literal("Progress"));
	ui->txtLog->clear();
	}

ProgressDialog::~ProgressDialog()
	{
	delete ui;
	}

void ProgressDialog::setTitle(const QString& aTitle)
	{
	setWindowTitle(aTitle);
	}

void ProgressDialog::setLogVisible(bool aVisible)
	{
	if (aVisible)
		ui->txtLog->show();
	else
		ui->txtLog->hide();
	}

void ProgressDialog::addLog(const QString& aMesg)
	{
	if (ui->txtLog->isVisible())
		{
		ui->txtLog->appendPlainText(aMesg);
		qApp->processEvents();
		}
	}

void ProgressDialog::setRange(int aMin, int aMax)
	{
	ui->progressBar->setRange(aMin, aMax);
	}

void ProgressDialog::setValue(int aValue)
	{
	ui->progressBar->setValue(aValue);
	qApp->processEvents();
	}

void ProgressDialog::readStandardOutput()
	{
	QProcess *process = qobject_cast<QProcess *>(sender());
	if (process)
		{
		QByteArray ba = process->readAllStandardOutput();
		qDebug() << ba;
		addLog(QString::fromLatin1(ba.data()));
		}
	}

void ProgressDialog::readStandardError()
	{
	QProcess *process = qobject_cast<QProcess *>(sender());
	if (process)
		{
		QByteArray ba = process->readAllStandardError();
		qDebug() << ba;
		addLog(QString::fromLatin1(ba.data()));
		}
	}
