#ifndef HDRVIEWERSETTINGS_H
#define HDRVIEWERSETTINGS_H

#include <QWidget>

namespace Ui {
class HdrViewerSettings;
}

class HdrViewerSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit HdrViewerSettings(QWidget *parent = 0);
    ~HdrViewerSettings();
    
    void setGamma(float gamma);
    void setExposure(float exposure);

    float gamma() const { return m_gamma; }
    float exposure() const { return m_exposure; }

    void onGammaSliderChanged(int value);
    void onExposureSliderChanged(int value);

signals:
    void gammaChanged(float gamma);
    void exposureChanged(float exposure);

private:
    int gammaToSliderValue(float gamma);
    float sliderValueToGamma(int value);

    int exposureToSliderValue(float exposure);
    float sliderValueToExposure(int value);

private:
    Ui::HdrViewerSettings *ui;

    float m_gamma;
    float m_exposure;
};

#endif // HDRVIEWERSETTINGS_H
