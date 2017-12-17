#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "steno.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    steno = new Steno(this);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete steno;
}

void MainWindow::showProgress(QString label, int maxValue)
{
    QString progressLabelText(label);
    QString cancelButtonText("Cancel");
    progressDialog = new QProgressDialog(progressLabelText, cancelButtonText, 0, maxValue, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->show();
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelSteno()));
}

void MainWindow::updateProgress(int value)
{
    progressDialog->setValue(value);
}

void MainWindow::showAlert(QString alertMessage)
{
    QMessageBox::warning(this, "Warning", alertMessage);
}

void MainWindow::showError(QString errorMessage)
{
    QMessageBox::critical(this, "Failed", errorMessage);
}

void MainWindow::showExtractResult(QString extractedText)
{
    ui->textboxUtf8->document()->setPlainText(extractedText);
}

void MainWindow::saveImageResult(QImage *image)
{
    QString outputFilename = QFileDialog::getSaveFileName(this, "Save To", QDir::homePath(), "Image Files (*.bmp)");
    if (!outputFilename.isEmpty()) {
        image->save(outputFilename);
    }
    delete image;
}

void MainWindow::on_buttonOpenImage_clicked()
{
    QString inputFilename = QFileDialog::getOpenFileName(this, "Choose File", QDir::homePath(), "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (!inputFilename.isEmpty()) {
        ui->textInputFilename->setText(inputFilename);
    }
    if (ui->optionExtract->isChecked()) {
        ui->textboxUtf8->clear();
    }
}

void MainWindow::on_buttonSteno_clicked()
{
    QImage *image = new QImage(ui->textInputFilename->text());
    if (ui->optionPut->isChecked()) {
        steno->put(ui->textboxUtf8->toPlainText(), image);
    }
    if (ui->optionExtract->isChecked()) {
        steno->extract(image);
    }
}

void MainWindow::on_optionPut_toggled(bool checked)
{
    if (checked) {
        clear();
    }
}


void MainWindow::on_optionExtract_toggled(bool checked)
{
    if (checked) {
        clear();
    }
}

void MainWindow::on_buttonAbout_clicked()
{
    QMessageBox::information(this, "Steno!", "The most simple stenography app created by Naufal Fachrian.");
}

void MainWindow::cancelSteno()
{
    steno->cancel();
}

void MainWindow::clear()
{
    ui->textboxUtf8->clear();
    ui->textInputFilename->clear();
}
