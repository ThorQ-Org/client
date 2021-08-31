#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

ThorQ::UI::MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_vlayout(new QVBoxLayout())
    , m_hlayout(new QHBoxLayout())
{/*
    m_vlayout->setParent(this);
    m_hlayout->setParent(this);
	setLayout(m_vlayout);
*/}
