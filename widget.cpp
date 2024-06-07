#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    setWindowIcon(QIcon(":/favicon.ico"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_selectFilesButton_clicked()
{
    selectedFiles = QFileDialog::getOpenFileNames(this, tr("Select PGN Files"), "", tr("PGN Files (*.pgn)"));
    if (selectedFiles.isEmpty()) {
        QMessageBox::information(this, tr("No Files Selected"), tr("Please select at least one PGN file."));
    }
}

void Widget::on_mergeFilesButton_clicked()
{
    if (selectedFiles.isEmpty()) {
        QMessageBox::warning(this, tr("No Files Selected"), tr("Please select PGN files first."));
        return;
    }

    QString mergedContent;
    for (const QString &filePath : selectedFiles) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            mergedContent += in.readAll();
            file.close();
        } else {
            QMessageBox::warning(this, tr("File Error"), tr("Cannot open file: ") + filePath);
        }
    }

    ui->textEdit->setPlainText(mergedContent);

    // Optional: Save merged content to a new file
    QString savePath = QFileDialog::getSaveFileName(this, tr("Save Merged File"), "", tr("PGN Files (*.pgn)"));
    if (!savePath.isEmpty()) {
        QFile outFile(savePath);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outFile);
            out << mergedContent;
            outFile.close();
        } else {
            QMessageBox::warning(this, tr("Save Error"), tr("Cannot save file: ") + savePath);
        }
    }
}
