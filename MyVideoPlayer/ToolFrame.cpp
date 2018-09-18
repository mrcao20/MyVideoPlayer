#include "ToolFrame.h"
#include <qapplication.h>
#include <QTime>
#include <qfile.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include "Slider.h"

struct ToolFrameData{
	QSlider *m_positionSlider;
	QPushButton *m_volumeButton;
	QSlider *m_volumeSlider;
	QLabel *m_positionLabel;
	QLabel *m_durationLabel;
	QComboBox *m_searchComboBox;
	QPushButton *m_searchButton;
};

ToolFrame::ToolFrame(QWidget *parent)
	: QFrame(parent)
	, d(new ToolFrameData)
{
	setObjectName("ToolFrame");

	init();

	QString appPath = qApp->applicationDirPath();
#ifdef _DEBUG
	appPath += "/../Release/data/qss/";
#else
	appPath += "/data/qss/";
#endif // _DEBUG
	QString toolFrameQSS = appPath + "ToolFrame.qss";
	QString searchButonQSS = appPath + "SearchButton.qss";
	loadStyleSheet(this, toolFrameQSS);
	loadStyleSheet(d->m_searchButton, searchButonQSS);
}

void ToolFrame::init() {
	d->m_positionSlider = new Slider(this);
	d->m_volumeButton = new QPushButton(this);
	d->m_volumeSlider = new Slider(this);
	d->m_positionLabel = new QLabel(this);
	d->m_durationLabel = new QLabel(this);
	d->m_searchComboBox = new QComboBox(this);
	d->m_searchButton = new QPushButton(this);

	d->m_positionSlider->setOrientation(Qt::Horizontal);
	d->m_positionSlider->setFixedHeight(10);
	d->m_volumeSlider->setRange(0, 100);
	d->m_volumeSlider->setValue(80);
	d->m_volumeButton->setFixedSize(20, 10);
	d->m_volumeSlider->setOrientation(Qt::Horizontal);
	d->m_volumeSlider->setFixedSize(80, 10);

	d->m_positionLabel->setFixedSize(51, 16);
	d->m_positionLabel->setText("00:00:00");
	d->m_positionLabel->setStyleSheet("color:rgb(204, 204, 204);");
	d->m_durationLabel->setFixedSize(61, 16);
	d->m_durationLabel->setText("00:00:00");
	d->m_durationLabel->setStyleSheet("color:rgb(111, 111, 111);");

	d->m_searchComboBox->addItem("ÌÚÑ¶ÊÓÆµ");
	d->m_searchComboBox->setFixedSize(75, 20);
	d->m_searchButton->setObjectName("searchButton");
	d->m_searchButton->setText("search");
	d->m_searchButton->setFixedSize(45, 22);

	QGridLayout *layout = new QGridLayout(this);

	layout->addItem(new QSpacerItem(80, 0, QSizePolicy::Fixed), 1, 0);
	layout->addWidget(d->m_positionLabel, 1, 1);
	layout->addWidget(d->m_durationLabel, 1, 2);
	layout->addItem(new QSpacerItem(60, 0, QSizePolicy::Maximum), 1, 3);
	layout->addWidget(d->m_searchComboBox, 1, 4);
	layout->addWidget(d->m_searchButton, 1, 5);
	layout->addItem(new QSpacerItem(0, 0), 1, 6);
	layout->addItem(new QSpacerItem(0, 0), 1, 7);
	layout->addItem(new QSpacerItem(0, 0), 1, 8);
	int count = layout->columnCount();
	layout->addWidget(d->m_positionSlider, 0, 0, 1, count - 2);
	layout->addWidget(d->m_volumeButton, 0, count - 2);
	layout->addWidget(d->m_volumeSlider, 0, count - 1);

	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	
	setLayout(layout);

	connect(d->m_positionSlider, &QSlider::valueChanged, this, &ToolFrame::positionValueChanged);
	connect(d->m_volumeSlider, &QSlider::valueChanged, this, &ToolFrame::volumeValueChanged);
	connect(d->m_searchButton, &QPushButton::clicked, [this]() {
		QString name = d->m_searchComboBox->currentText();
		emit search(name);
	});
}

void ToolFrame::loadStyleSheet(QWidget *widget, QString fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray byteArray = file.readAll();
	widget->setStyleSheet(byteArray);
}

void ToolFrame::positionUpdateDuration(qint64 duration) {
	QTime allDuration(duration / 3600000, (duration / 60000) % 60, (duration / 1000) % 60);
	d->m_durationLabel->setText(allDuration.toString("/hh:mm:ss"));
	d->m_positionSlider->setRange(0, duration);
	d->m_positionSlider->setEnabled(duration > 0);
	d->m_positionSlider->setPageStep(6000);
	d->m_positionSlider->setSingleStep(3000);
}

void ToolFrame::positionUpdatePosition(qint64 position) {
	d->m_positionSlider->setValue(position);
	QTime duration(position / 3600000, (position / 60000) % 60, (position / 1000) % 60);
	d->m_positionLabel->setText(duration.toString("hh:mm:ss"));
}

void ToolFrame::positionSeekForward() {
	d->m_positionSlider->triggerAction(QSlider::SliderPageStepAdd);
}

void ToolFrame::positionSeekBackward() {
	d->m_positionSlider->triggerAction(QSlider::SliderSingleStepSub);
}

ToolFrame::~ToolFrame(){
}
