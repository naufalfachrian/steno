#include "steno.h"
#include <QString>
#include <QImage>
#include <QProgressDialog>
#include "stenowindow.h"

Steno::Steno(StenoWindow *window)
{
    this->window = window;
}

void Steno::put(QString text, QImage *targetImage)
{
    canceled = false;

    if (text.isEmpty()) {
        window->showAlert("Text is empty.");
        delete targetImage;
        return;
    }

    QVector<bool> bits = this->prepareBits(text);

    if (bits.size() > (targetImage->width() * targetImage->height())) {
        window->showError("Image size is too small.");
        delete targetImage;
        return;
    }

    QImage *manipulatedImage = this->manipulateImage(targetImage, bits);

    if (!canceled) {
        window->saveImageResult(manipulatedImage);
    }
}

void Steno::extract(QImage *fromImage)
{
    canceled = false;

    if ((fromImage->width() * fromImage->height()) < 64) {
        window->showError("Image size is too small.");
        delete fromImage;
        return;
    }

    this->parseImage(fromImage);
}

void Steno::cancel()
{
    canceled = true;
}

QVector<unsigned char> Steno::prepareBytes(QString string)
{
    std::string stdString = string.toStdString();

    window->showProgress("Step 1/3 : Extracting Characters to Bytes ...", stdString.size());

    QVector<unsigned char> bytes;

    bytes.push_back('\x02'); // U+0002 = START OF TEXT

    uint progressCounter = 0;

    for (unsigned char byte : stdString) {
        bytes.push_back(byte);

        ++progressCounter;
        window->updateProgress(progressCounter);

        if (canceled) { return bytes; }
    }

    bytes.push_back('\x03'); // U+0003 = END OF TEXT

    return bytes;
}

QVector<bool> Steno::prepareBits(QString string)
{
    QVector<unsigned char> bytes = this->prepareBytes(string);

    window->showProgress("Step 2/3 : Extracting Bytes to Bits ...", bytes.size());

    QVector<bool> bits;

    uint progressCounter = 0;

    for (unsigned char byte: bytes) {

        QVector<bool> innerBits;

        while (byte != 0) {
            innerBits.push_front(byte % 2);
            byte >>= 1;
        }

        while (innerBits.size() < 8) {
            innerBits.push_front(0);
        }

        bits += innerBits;

        ++progressCounter;
        window->updateProgress(progressCounter);

        if (canceled) { return bits; }
    }

    return bits;
}

QImage *Steno::manipulateImage(QImage *image, QVector<bool> bits)
{   
    window->showProgress("Step 3/3 : Manipulating Pixels ...", bits.size());

    int bitCounter = 0;

    for (int col = 0; col < image->width() && bitCounter < bits.size(); ++col) {

        for (int row = 0; row < image->height() && bitCounter < bits.size(); ++row) {

            QColor oldPixelColor(image->pixelColor(col, row));
            image->setPixel(col, row, generateNewPixelColor(oldPixelColor, bits.at(bitCounter)));

            ++bitCounter;
            window->updateProgress(bitCounter);

            if (canceled) { return image; }
        }
    }

    return image;
}

QRgb Steno::generateNewPixelColor(QColor oldPixelColor, bool bit)
{
    unsigned char blueColor = oldPixelColor.blue();

    if (bit != blueColor % 2) {
        if (blueColor % 2) {
            blueColor += 1;
        } else {
            blueColor -= 1;
        }
    }

    return qRgba(oldPixelColor.red(), oldPixelColor.green(), blueColor, oldPixelColor.alpha());
}

void Steno::parseImage(QImage *image)
{
    std::string stdString;

    this->window->showProgress("Parsing image ...", image->width() - 1);

    uchar comparator = (UCHAR_MAX >> 1) + 1;
    uchar cache = 0;

    bool valid = false;

    for (int col = 0; col < image->width(); ++col) {

        for (int row = 0; row < image->height(); ++row) {

            QColor pixelColor(image->pixelColor(col, row));

            bool bit = pixelColor.blue() % 2;

            cache += bit * comparator;
            comparator >>= 1;
            if (comparator == 0) {

                if (cache == '\x03') {
                    this->window->updateProgress(image->width() - 1);
                    this->window->showExtractResult(QString::fromStdString(stdString));
                    delete image;
                    return;
                }

                if (stdString.size() == 0 && cache != '\x02' && !valid) {
                    this->window->updateProgress(image->width() - 1);
                    this->window->showError("The image doesn't contain hidden message.");
                    delete image;
                    return;
                } else {
                    valid = true;
                }

                if (valid) {
                    if (stdString.size() == 0 && cache == '\x02') {} else
                    stdString += cache;
                }

                comparator = (UCHAR_MAX >> 1) + 1;
                cache = 0;

                if (canceled) { return; }
            }
        }
        this->window->updateProgress(col);
    }
    delete image;
}
