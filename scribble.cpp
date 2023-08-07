#include <QtWidgets>

#include "scribble.h"
#include "ui_scribble.h"
#include "scribblearea.h"

Scribble::Scribble(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Scribble)
{
    ui->setupUi(this);

    p_ScribbleArea = std::make_unique<ScribbleArea>();
    setCentralWidget(p_ScribbleArea.get());
    createActions();
    createMenus();
    setWindowTitle(tr("Scribble"));
    resize(600, 600);
}

void Scribble::closeEvent(QCloseEvent *event)
{
    if(checkSave()) event->accept();
    else event->ignore();
}

void Scribble::open()
{
    if(checkSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty())
        {
            p_ScribbleArea->openImage(fileName);
        }
    }
}

void Scribble::save()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void Scribble::penColor()
{
    QColor newColor = QColorDialog::getColor(p_ScribbleArea->penColor());
    if(newColor.isValid())
    {
        p_ScribbleArea->setPenColor(newColor);
    }
}

void Scribble::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen size : "), p_ScribbleArea->penWidth(), 1, 50, 1, &ok);
    if(ok)
    {
        p_ScribbleArea->setPenWidth(newWidth);
    }
}

void Scribble::about()
{
    QMessageBox::about(this, tr("About Scribble"), tr("<p><b>Scribble</b> is a very simple paint application.</p>"));
}

void Scribble::createActions()
{
    p_OpenAction = new QAction(tr("&Open"), this);
    p_OpenAction->setShortcuts(QKeySequence::Open);
    connect(p_OpenAction, SIGNAL(triggered()), this, SLOT(open()));
    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        m_SaveAsActs.append(action);
    }
    p_ExitAct = new QAction(tr("E&xit"), this);
    p_ExitAct->setShortcuts(QKeySequence::Quit);
    connect(p_ExitAct, SIGNAL(triggered()), this, SLOT(close()));

    p_PenColorAct = new QAction(tr("&Pen Color..."), this);
    connect(p_PenColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    p_PenWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(p_PenWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    p_ClearScreenAct = new QAction(tr("&Clear Screen..."), this);
    p_ClearScreenAct->setShortcut(tr("Ctrl+W"));
    connect(p_ClearScreenAct, SIGNAL(triggered()), p_ScribbleArea.get(), SLOT(clearImage()));

    p_AboutAct = new QAction(tr("&About..."), this);
    connect(p_AboutAct, SIGNAL(triggered()), SLOT(about()));

    p_AboutQtAct = new QAction(tr("About &Qt..."), this);
    connect(p_AboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Scribble::createMenus()
{
    p_SaveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, m_SaveAsActs)
    {
        p_SaveAsMenu->addAction(action);
    }

    p_FileMenu = new QMenu(tr("&File"), this);
    p_FileMenu->addAction(p_OpenAction);
    p_FileMenu->addMenu(p_SaveAsMenu);
    p_FileMenu->addSeparator();
    p_FileMenu->addAction(p_ExitAct);

    p_OptionMenu = new QMenu(tr("&Options"), this);
    p_OptionMenu->addAction(p_PenColorAct);
    p_OptionMenu->addAction(p_PenWidthAct);
    p_OptionMenu->addSeparator();
    p_OptionMenu->addAction(p_ClearScreenAct);

    p_HelpMenu = new QMenu(tr("&Help"), this);
    p_HelpMenu->addAction(p_AboutAct);
    p_HelpMenu->addAction(p_AboutQtAct);

    menuBar()->addMenu(p_FileMenu);
    menuBar()->addMenu(p_OptionMenu);
    menuBar()->addMenu(p_HelpMenu);
}

bool Scribble::checkSave()
{
    if(p_ScribbleArea->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"), tr("The image has been modified.\nDo you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
            return saveFile("png");
        else if(ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}

bool Scribble::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
                                                    tr("%1 Files(*.%2);; All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper()), QString::fromLatin1(fileFormat)));

    if(fileName.isEmpty())
        return false;
    else
        return p_ScribbleArea->saveImage(fileName, fileFormat);
}

Scribble::~Scribble()
{
    delete ui;
}
