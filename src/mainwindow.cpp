#include "mainwindow.h"
#include "mpvwidget.h"
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qfiledialog.h>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {

	m_mpv = new MpvWidget(this);
	m_slider = new QSlider();
	m_slider->setOrientation(Qt::Horizontal);
	m_openBtn = new QPushButton("Open");
	m_playBtn = new QPushButton("Pause");
	QHBoxLayout *hb = new QHBoxLayout();
	hb->addWidget(m_openBtn);
	hb->addWidget(m_playBtn);
	QVBoxLayout *vl = new QVBoxLayout();
	vl->addWidget(m_mpv);
	vl->addWidget(m_slider);
	vl->addLayout(hb);
	setLayout(vl);
	connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
	connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
	connect(m_playBtn, SIGNAL(clicked()), SLOT(pauseResume()));
	connect(m_mpv, SIGNAL(positionChanged(int)), m_slider, SLOT(setValue(int)));
	connect(m_mpv, SIGNAL(durationChanged(int)), this, SLOT(setSliderRange(int)));

	// Create a video child window. Force Qt to create a native window, and
	// pass the window ID to the mpv wid option. Works on: X11, win32, Cocoa
	m_mpv->setAttribute(Qt::WA_DontCreateNativeAncestors);
	m_mpv->setAttribute(Qt::WA_NativeWindow);
	WId raw_wid = m_mpv->winId();
#ifdef _WIN32
	// Truncate to 32-bit, as all Windows handles are. This also ensures
	// it doesn't go negative.
	int64_t wid = static_cast<uint32_t>(raw_wid);
#else
	int64_t wid = raw_wid;
#endif
	mpv_set_option(m_mpv->mpv, "wid", MPV_FORMAT_INT64, &wid);
}

void MainWindow::openMedia() {

	QString file = QFileDialog::getOpenFileName(0, "Open a video");
	if (file.isEmpty())
		return;
	m_mpv->command(QStringList() << "loadfile" << file);
	const QFileInfo fileInfo = QFileInfo(file);
	QString baseName = fileInfo.baseName();
	if (baseName.back().isDigit()) {
		baseName.chop(1);
	}
	const QString suffix = fileInfo.suffix();
	QString filePath = fileInfo.absolutePath();
	if (filePath.back() == "/") {
		filePath.chop(1);
	}
	for (int i = 1; i < 11; i++) {
		const QString newFile = filePath + QStringLiteral("/") + baseName + QString::number(i) + QStringLiteral(".") + suffix;
		if (QFileInfo::exists(newFile)) {
			m_mpv->command(QStringList() << "loadfile" << newFile << "append");
		}
	}
}

void MainWindow::seek(int pos) {

	m_mpv->command(QVariantList() << "seek" << pos << "absolute");
}

void MainWindow::pauseResume() {

	const bool paused = m_mpv->getProperty("pause").toBool();
	m_mpv->setProperty("pause", !paused);
}

void MainWindow::setSliderRange(int duration) {

	m_slider->setRange(0, duration);
}
