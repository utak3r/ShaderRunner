#ifndef GLVIEW_WIDGET_H
#define GLVIEW_WIDGET_H

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

#include "GLViewWidget_global.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QTime>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLVIEWWIDGETSHARED_EXPORT GLViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
	{
	Q_OBJECT

public:
	GLViewWidget(QWidget *aParent = nullptr, const QColor &aBackground = QColor(0,0,0));
	~GLViewWidget();

	Q_PROPERTY(bool showFPS READ showFPS WRITE setShowFPS)
	Q_PROPERTY(double animationStep READ animationStep WRITE setAnimationStep)

public slots:
	void setFragmentShaderSource(const QString& aFragmentShaderSource);
	void setFragmentShaderFile(const QString& aFragmentShaderFilename);
	double aspectRatio() const { return theAspectRatio; }
	void setAspectRatio(const double anAspectRatio);
	bool showFPS() const { return theShowFPS; }
	void setShowFPS(bool aShow) { theShowFPS = aShow; }
	bool isPlaying();
	void play();
	void pause();
	void rewind();
	double animationStep() const { return theAnimationStep; }
	void setAnimationStep(double anAnimStep) { theAnimationStep = anAnimStep; }

protected slots:
	void timerUpdate();

protected:
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void initializeGL() Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
	QColor theBackgroundColor;
	QOpenGLShader *theShaderVertex;
	QOpenGLShader *theShaderFragment;
	QLatin1Literal theShaderFragmentTemplate;
	QLatin1Literal theShaderFragmentFailsafe;
	QOpenGLShaderProgram *theShaderProgram;
	QOpenGLBuffer *theCanvasBuffer;
	QOpenGLTexture *theCanvasTexture;
	double theAspectRatio;
	QTimer *theAnimationTimer;
	float theGlobalTime;
	QPoint theMousePressPos;
	bool theShowFPS;
	int theFPSFrames;
	QTime theFPSTime;
	double theAnimationStep;
	};

#endif // GLVIEW_WIDGET_H
