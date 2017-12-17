#ifndef STENOWINDOW_H
#define STENOWINDOW_H

class QString;

class QImage;

class QProgressDialog;

class StenoWindow
{
public:
    StenoWindow() {}
    virtual ~StenoWindow() {}
    virtual void showProgress(QString label, int maxValue) {}
    virtual void updateProgress(int value) {}
    virtual void showAlert(QString alertMessage) {}
    virtual void showError(QString errorMessage) {}
    virtual void showExtractResult(QString extractedText) {}
    virtual void saveImageResult(QImage *image) {}
};

#endif // STENOWINDOW_H
