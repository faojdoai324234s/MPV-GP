#include <QtWidgets/qapplication.h>
#include <clocale>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

	QApplication a(argc, argv);

	// Qt sets the locale in the QApplication constructor, but libmpv requires
	// the LC_NUMERIC category to be set to "C", so change it back.
	char* currentLocale = std::setlocale(LC_NUMERIC, nullptr);
	std::setlocale(LC_NUMERIC, "C");

	MainWindow w;
	w.show();

	QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
		// Change the locale back to its original value
		std::setlocale(LC_NUMERIC, currentLocale);
	});

	return a.exec();
}
