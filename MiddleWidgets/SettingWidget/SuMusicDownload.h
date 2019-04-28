#ifndef SuMusicDownload_H
#define SuMusicDownload_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuMusicDownload: public ISettingUnit
{
    Q_OBJECT

public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

private:
    virtual QString getTipAfterAgreeForTimes(int);

    virtual QString getTipForClickCount(int count, qint64 currentTime);

private slots:
    void OnNameFormatClicked(int radioId);

public:
    QWidget* SettingUnitContainer;

    QLabel*  labelSettingTitle;

    BesButton*  btnSelectMusicDownloadPath;
    QLabel*  labelMusicDownloadPathTip;
    QLabel*  labelMusicDownloadPath;

    QCheckBox*  checkboxMusicDownload;

    QLabel* labelNameFormat;
    QRadioButton* radioFormatArtistSong;
    QRadioButton* radioFormatSongArtist;
    QButtonGroup* groupNameFormat;
};

#endif // SuMusicDownload_H
