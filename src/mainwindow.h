#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/qwidget.h>

class MpvWidget;
class QSlider;
class QPushButton;

class MainWindow final : public QWidget {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

public Q_SLOTS:
	void openMedia();
	void seek(int pos);
	void pauseResume();

private Q_SLOTS:
	void setSliderRange(int duration);

private:
	MpvWidget *m_mpv;
	QSlider *m_slider;
	QPushButton *m_openBtn;
	QPushButton *m_playBtn;
};

#endif // MainWindow_H