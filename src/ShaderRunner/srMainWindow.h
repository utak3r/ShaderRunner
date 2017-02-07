#ifndef SHADERRUNNER_MAINWINDOW_H
#define SHADERRUNNER_MAINWINDOW_H

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

#include <QMainWindow>
#include <QEvent>
#include <QDebug>

namespace Ui
	{
	class srMainWindow;
	}

QT_FORWARD_DECLARE_CLASS(GLViewWidget)
QT_FORWARD_DECLARE_CLASS(QSplitter)

class srMainWindow : public QMainWindow
	{
	Q_OBJECT

public:
	explicit srMainWindow(QWidget* aParent = 0);
	~srMainWindow();

public slots:
	void exitApp();
	void OpenShaderSource();
	void SaveShaderSource();
	void SaveBufferToImage();
	void RenderToImage();

protected:
	void connectButtons();
	void decorateSplitter(QSplitter* splitter);
	void loadConfig();
	void storeConfig();

private:
	Ui::srMainWindow *ui;
	double theAnimStepOnSlider;
	};


#endif // SHADERRUNNER_MAINWINDOW_H
