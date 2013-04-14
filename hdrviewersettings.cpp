#include "hdrviewersettings.h"
#include "ui_hdrviewersettings.h"

#include <math.h>
#include <QDebug>

HdrViewerSettings::HdrViewerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HdrViewerSettings)
{
    ui->setupUi(this);

    connect(ui->gammaSlider, &QSlider::valueChanged, this, &HdrViewerSettings::onGammaSliderChanged);
    connect(ui->gammaSlider, &QSlider::valueChanged, this, &HdrViewerSettings::gammaChanged);
    connect(ui->exposureSlider, &QSlider::valueChanged, this, &HdrViewerSettings::onExposureSliderChanged);
    connect(ui->exposureSlider, &QSlider::valueChanged, this, &HdrViewerSettings::exposureChanged);
}

HdrViewerSettings::~HdrViewerSettings()
{
    delete ui;
}

void HdrViewerSettings::onExposureSliderChanged(int value)
{
    m_exposure = sliderValueToExposure(value);
    ui->exposureEdit->setText(QString::number(m_exposure));
}

void HdrViewerSettings::onGammaSliderChanged(int value)
{
    m_gamma = sliderValueToGamma(value);
    ui->gammaEdit->setText(QString::number(m_gamma));
}

void HdrViewerSettings::setGamma(float gamma)
{
    m_gamma = gamma;
    ui->gammaSlider->setValue(gammaToSliderValue(gamma));
    ui->gammaEdit->setText(QString::number(m_gamma));
}

void HdrViewerSettings::setExposure(float exposure)
{
    m_exposure = exposure;
    ui->exposureSlider->setValue(exposureToSliderValue(exposure));
    ui->exposureEdit->setText(QString::number(m_exposure));
}

int HdrViewerSettings::exposureToSliderValue(float exposure)
{
    float n = log(exposure) / 3.0;
    int sliderValue = n * 500 + 500;
    return sliderValue;
}

float HdrViewerSettings::sliderValueToExposure(int value)
{
    float n = 3.0 * (2.0 * value / 1000.0 - 1.0);
    return pow(M_E, n);
}

int HdrViewerSettings::gammaToSliderValue(float gamma)
{
    return gamma / 5.0 * 1000.0;
}

float HdrViewerSettings::sliderValueToGamma(int value)
{
    return value / 1000.0 * 5.0;
}


