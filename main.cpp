#include <QApplication>
#include <QGraphicsView>
#include <QScreen>
#include <QFile>
#include "scene.hpp"

int main(int argc, char ** argv){
	QApplication app(argc, argv);

         {
                  QFile file(":/styles/stylesheet.css");
                  file.open(QFile::ReadOnly);
                  app.setStyleSheet(file.readAll());
                  file.close();
         }

         auto mainRect = QApplication::primaryScreen()->availableSize();

         GraphicsScene scene(mainRect);
         QGraphicsView view(&scene);

         QObject::connect(&scene,&GraphicsScene::close,&app,&QApplication::quit);

         view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
         view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
         // view.setGeometry(0,0,mainRect.width(),mainRect.height());
         view.setFixedSize(mainRect);

         view.show();
	return app.exec();
}
