#ifndef LyricEditorBox_H
#define LyricEditorBox_H

#include <BesShadowWidget.h>
#include <BesFramelessDialog.h>
#include <QMessageBox>
#include <BesButton.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTextEdit>

#include <table/FinishLrcTableView.h>

class LyricEditorBox :public BesFramelessDialog
{
    Q_OBJECT
public:

    LyricEditorBox(QWidget *parent);

    void initLayout();
    void initConnection();

    bool isResultOK();

    void setLyricData(const QVector<QString>& _rawLines, const QVector<QPair<quint64, QString>>& _lrcLines);
    void getLyricData(QVector<QString>& _rawLines, QVector<QPair<quint64, QString>>& _lrcLines);
public slots:

    void onCopySelected();
    void onPastSelected();

private:
    QVector<QString> lineSelected;
    QVector<QString> rawLines;                          //原始歌词
    QVector<QPair<quint64, QString>> lrcLines;          //lrc歌词

private:
    QWidget* widgetMessageBoxContainer;
    QWidget* widgetBaseContainer;

    QWidget* widgetHeader;
    QWidget* widgetBody;
    QWidget* widgetFooter;

    QWidget* widgetContainer;
    QWidget* widgetContainerTop;
    QWidget* widgetContainerMain;

    QLabel *labelMessageTitle;
    BesButton *btnMessageClose;

    BesButton *btnOK;
    BesButton *btnCancel;

    QLabel *labelLyricFinishPart;
    QLabel *labelLyricUnfinishPart;
    BesButton *btnCopySelected;
    BesButton *btnPastSelected;
    BesButton *btnRemakeLastLine;

    FinishLrcTableView* tableLyric;
    QTextEdit* editUnfinishPart;

    QMessageBox::StandardButton btnResult;
};

#endif // LyricEditorBox_H
