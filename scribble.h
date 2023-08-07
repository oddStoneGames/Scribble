#ifndef SCRIBBLE_H
#define SCRIBBLE_H

#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Scribble; }
QT_END_NAMESPACE

class ScribbleArea;

class Scribble : public QMainWindow
{
    Q_OBJECT

public:
    Scribble(QWidget *parent = nullptr);
    ~Scribble();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    void createActions();
    void createMenus();
    bool checkSave();
    bool saveFile(const QByteArray &fileFormat);

    Ui::Scribble *ui;
    std::unique_ptr<ScribbleArea> p_ScribbleArea;
    QMenu *p_SaveAsMenu;
    QMenu *p_FileMenu;
    QMenu *p_OptionMenu;
    QMenu *p_HelpMenu;
    QAction *p_OpenAction;
    QList<QAction*> m_SaveAsActs;
    QAction *p_PenColorAct;
    QAction *p_PenWidthAct;
    QAction *p_ClearScreenAct;
    QAction *p_AboutAct;
    QAction *p_AboutQtAct;
    QAction *p_ExitAct;

};
#endif // SCRIBBLE_H
