#ifndef STENO_H
#define STENO_H

#include <QVector>
#include <QColor>

class QString;

class QImage;

class StenoWindow;

class Steno
{
public:
    Steno(StenoWindow *window);
    void put(QString text, QImage *targetImage);
    void extract(QImage *fromImage);
    void cancel();

private:
    StenoWindow *window;
    QVector<unsigned char> prepareBytes(QString string);
    QVector<bool> prepareBits(QString string);
    QImage *manipulateImage(QImage *image, QVector<bool> bits);
    QRgb generateNewPixelColor(QColor oldPixelColor, bool bit);
    void parseImage(QImage *image);
    bool canceled = false;
};

#endif // STENO_H
