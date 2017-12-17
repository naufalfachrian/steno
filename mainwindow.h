#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stenowindow.h"

namespace Ui {
class MainWindow;
}

class Steno;

class QProgressDialog;

class MainWindow : public QMainWindow, public StenoWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Steno *steno;
    QProgressDialog *progressDialog;

    // StenoWindow interface
public:
    void showProgress(QString label, int maxValue);
    void updateProgress(int value);
    void showAlert(QString alertMessage);
    void showError(QString errorMessage);
    void showExtractResult(QString extractedText);
    void saveImageResult(QImage *image);

private slots:
    void on_buttonOpenImage_clicked();
    void on_buttonSteno_clicked();
    void on_optionPut_toggled(bool checked);
    void on_optionExtract_toggled(bool checked);
    void on_buttonAbout_clicked();
    void cancelSteno();

private:
    void clear();

};

#endif // MAINWINDOW_H
