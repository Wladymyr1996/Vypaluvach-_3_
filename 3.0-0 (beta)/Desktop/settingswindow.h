#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#endif // SETTINGSWINDOW_H

#include <QWidget>

class QGroupBox;
class QComboBox;
class QDoubleValidator;
class QCheckBox;
class QLineEdit;
class QLabel;
class QPushButton;

class TSettingsWindow : public QWidget{
    Q_OBJECT

    public:
        TSettingsWindow(QWidget *parent);

    signals:
        void opened(bool);

    public slots:
        void updateLabels();
        void doUpdateMeasureUnits();
        void doCancelButton();
        void doApplyButton();

    private:
        void restore();
        void doUpdate();

        QWidget *parent;

        QGroupBox *appSettingBox, *cncSettingBox;
        QComboBox *measureUnitComboBoxm, *measureUnitComboBox, *boudRateComboBox;
        QDoubleValidator *doubleValidator;
        QCheckBox *resizeImageCheckBox;
        QLineEdit *imageWidthLineEdit, *imageHeightLineEdit,
                  *coefStepsPerMMbyXLineEdit, *coefStepsPerMMbyYLineEdit,
                  *minPauseLineEdit, *maxPauseLineEdit, *skippedRowsLineEdit,
                  *skippedYStepsLineEdit, *heatingTimeLineEdit, *heatingPlaceWidthLineEdit;

        QLabel *imageWidthLabel, *imageHeightLabel, *coefStepsPerMMbyXLabel,
               *coefStepsPerMMbyYLabel, *minPauseLabel, *maxPauseLabel,
               *skippedRowsLabel, *skippedYStepsLabel, *heatingTimeLabel,
               *heatingPlaceWidthLabel, *measureUnitLabel, *boudRateLabel;

        QPushButton *cancelButton, *applyButton;

        // QWidget interface
protected:
        void closeEvent(QCloseEvent *event);
        void showEvent(QShowEvent *event);
};

