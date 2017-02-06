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

#include "GLViewWidget.h"
#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QMouseEvent>

GLViewWidget::GLViewWidget(QWidget *aParent, const QColor &aBackground)
	:
	theBackgroundColor(aBackground),
	theShaderVertex(nullptr),
	theShaderFragment(nullptr),
	theShaderFragmentTemplate(""),
	theShaderFragmentFailsafe(""),
	theShaderProgram(nullptr),
	theCanvasBuffer(nullptr),
	theCanvasTexture(nullptr),
	theAnimationTimer(nullptr),
	theGlobalTime(0.f),
	theAspectRatio(16.f/9.f),
	theMousePressPos(0, 0)
	{
	setParent(aParent);
	setMinimumSize(352, 198);
	theAnimationTimer = new QTimer(this);
	theAnimationTimer->setInterval(1000 / 50);
	connect(theAnimationTimer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
	}

GLViewWidget::~GLViewWidget()
	{
	}

void GLViewWidget::setAspectRatio(const double anAspectRatio)
	{
	if (anAspectRatio > 0)
		{
		theAspectRatio = anAspectRatio;
		resizeGL(size().width(), size().height());
		}
	}

void GLViewWidget::resizeGL(int aWidth, int aHeight)
	{
	int cw = aWidth;
	int ch = aHeight;

	QWidget *container = nullptr;
	if (parent() != nullptr)
		{
		container = qobject_cast<QWidget *>(parent());
		cw = container->size().width();
		ch = container->size().height();
		}

	int w = cw;
	int h = int(cw / theAspectRatio);
	double s = qMin(cw / double(w), ch / double(h));

	if (container != nullptr)
		{
		setGeometry((cw - int(w*s)) / 2, (ch - int(h*s)) / 2, int(w*s), int(h*s));
		}
	else
		resize(int(w*s), int(h*s));
	}

static const GLfloat vertices[4 * 2] =
	{
	-1.f, -1.f,
	1.f, -1.f,
	-1.f, 1.f,
	1.f, 1.f
	};

void GLViewWidget::initializeGL()
	{
	initializeOpenGLFunctions();

	theShaderVertex = new QOpenGLShader(QOpenGLShader::Vertex);
	theShaderVertex->compileSourceCode(QLatin1Literal("#version 130\n" \
														"attribute vec3 position;\n" \
														"void main()\n" \
														"{\n" \
														"gl_Position = vec4(position, 1.0);\n" \
														"}\n")
										);

	theShaderFragmentTemplate = QLatin1Literal("#version 130\n" \
												"uniform vec3 iResolution;				// viewport resolution (in pixels)\n" \
												"uniform float iGlobalTime;				// shader playback time (in seconds)\n" \
												"uniform vec4 iMouse;					// mouse pixel coords\n" \
												"uniform vec4 iDate;					// (year, month, day, time in seconds)\n" \
												"uniform float iSampleRate;				// sound sample rate (i.e., 44100)\n" \
												"uniform sampler2D iChannel0;			// input channel. XX = 2D/Cube\n" \
												"uniform sampler2D iChannel1;			// input channel. XX = 2D/Cube\n" \
												"uniform sampler2D iChannel2;			// input channel. XX = 2D/Cube\n" \
												"uniform sampler2D iChannel3;			// input channel. XX = 2D/Cube\n" \
												"uniform vec3 iChannelResolution[4];	// channel resolution (in pixels)\n" \
												"uniform float iChannelTime[4];			// channel playback time (in sec)\n" \
												"uniform vec2 iOffset;					// pixel offset for tiled rendering\n" \
												"\n" \
												"%1" \
												"\n" \
												"void main()\n" \
												"{\n" \
											    "mainImage(gl_FragColor, gl_FragCoord.xy);\n" \
												"}\n");

	theShaderFragmentFailsafe = QLatin1Literal("void mainImage( out vec4 fragColor, in vec2 fragCoord )\n" \
												"{\n" \
												"vec2 uv = fragCoord.xy / iResolution.xy;\n" \
												"fragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);\n" \
												"}\n"
												);

	theShaderFragment = new QOpenGLShader(QOpenGLShader::Fragment);
	theShaderFragment->compileSourceCode(QString(theShaderFragmentTemplate).arg(theShaderFragmentFailsafe));

	theShaderProgram = new QOpenGLShaderProgram();
	theShaderProgram->addShader(theShaderVertex);
	theShaderProgram->addShader(theShaderFragment);
	theShaderProgram->link();

	int positionLocation = theShaderProgram->attributeLocation("position");
	theShaderProgram->enableAttributeArray(positionLocation);
	theShaderProgram->setAttributeArray(positionLocation, vertices, 2, 0);

	theShaderProgram->setUniformValue("iResolution", QVector3D(size().width(), size().height(), 0));
	theShaderProgram->setUniformValue("iMouse", QVector4D(0.0, 0.0, 0.0, 0.0));
	theShaderProgram->setUniformValue("iSampleRate", GLfloat(44100.0));
	for (int i = 0; i < 4; i++)
		{
		theShaderProgram->setUniformValue(QStringLiteral("iChannelTime[%1]").arg(i).toLatin1().constData(), GLfloat(0.0));
		theShaderProgram->setUniformValue(QStringLiteral("iChannel%1").arg(i).toLatin1().constData(), GLint(0));
		}
	theShaderProgram->setUniformValue("iGlobalTime", GLfloat(1.0));
	theShaderProgram->setUniformValue("iOffset", QVector2D(0.0, 0.0));

	//theCanvasTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	//theCanvasTexture = new QOpenGLTexture(QImage(800, 450, QImage::Format_ARGB32));
	theCanvasTexture = new QOpenGLTexture(QImage(QLatin1Literal("metal-scratched-brushed-512.jpg")).mirrored());
	theCanvasTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	theCanvasTexture->setMagnificationFilter(QOpenGLTexture::Linear);
	theCanvasTexture->setWrapMode(QOpenGLTexture::Repeat);

	/*theCanvasBuffer = new QOpenGLBuffer();
	theCanvasBuffer->create();
	theCanvasBuffer->bind();
	//...
	theCanvasBuffer->release();*/

	theAnimationTimer->start();
	}

void GLViewWidget::paintGL()
	{
	QPainter painter;
	painter.begin(this);

	painter.beginNativePainting();

	glClearColor(theBackgroundColor.redF(), theBackgroundColor.greenF(), theBackgroundColor.blueF(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// paint the canvas
	theShaderProgram->bind();
	//theCanvasTexture->bind();
	int positionLocation = theShaderProgram->attributeLocation("position");
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	theShaderProgram->setUniformValue("iResolution", QVector3D(size().width(), size().height(), 0));
	theShaderProgram->setUniformValue("iGlobalTime", GLfloat(theGlobalTime));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	theShaderProgram->release();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	painter.endNativePainting();

	painter.end();
	update();
	}

void GLViewWidget::setFragmentShaderSource(const QString& aFragmentShaderSource)
	{
	theAnimationTimer->stop();
	theGlobalTime = 0.f;
	theShaderFragment->compileSourceCode(QString(theShaderFragmentTemplate).arg(aFragmentShaderSource));

	theShaderProgram->removeAllShaders();
	theShaderProgram->addShader(theShaderVertex);
	theShaderProgram->addShader(theShaderFragment);
	theShaderProgram->link();

	int positionLocation = theShaderProgram->attributeLocation("position");
	theShaderProgram->enableAttributeArray(positionLocation);
	theShaderProgram->setAttributeArray(positionLocation, vertices, 2, 0);
	theShaderProgram->setAttributeValue("iMouse", QVector4D(0.0, 0.0, 0.0, 0.0));
	theShaderProgram->setAttributeValue("iSampleRate", 44100.0);
	for (int i = 0; i < 4; i++)
		theShaderProgram->setAttributeValue(QStringLiteral("iChannelTime[%1]").arg(i).toLatin1().constData(), 0.0);
	theShaderProgram->setAttributeValue("iGlobalTime", 0.0);
	theShaderProgram->setAttributeValue("iOffset", QVector2D(0.0, 0.0));
	theAnimationTimer->start();
	}

void GLViewWidget::setFragmentShaderFile(const QString& aFragmentShaderFilename)
	{
	//
	}

void GLViewWidget::timerUpdate()
	{
	theGlobalTime += 0.1;
	}

bool GLViewWidget::isPlaying()
	{
	return theAnimationTimer->isActive();
	}

void GLViewWidget::play()
	{
	theAnimationTimer->start();
	}

void GLViewWidget::pause()
	{
	theAnimationTimer->stop();
	}

void GLViewWidget::rewind()
	{
	theGlobalTime = 0.f;
	}

void GLViewWidget::mousePressEvent(QMouseEvent *event)
	{
	QPoint pos = event->pos();
	theShaderProgram->bind();
	theShaderProgram->setUniformValue("iMouse", QVector4D(pos.x(), size().height()-pos.y(), 0.0, 0.0));
	theShaderProgram->release();
	theMousePressPos = pos;
	}

void GLViewWidget::mouseMoveEvent(QMouseEvent *event)
	{
	QPoint pos = event->pos();
	if (event->buttons() & Qt::LeftButton)
		{
		theShaderProgram->bind();
		theShaderProgram->setUniformValue("iMouse", 
										  QVector4D(pos.x(), size().height() - pos.y(), 
													 theMousePressPos.x(), size().height() - theMousePressPos.y()
													));
		theShaderProgram->release();
		}
	}
