#ifndef THORQ_ERRORLABEL_H
#define THORQ_ERRORLABEL_H

#include <QLabel>

class QWidget;
class QString;

namespace ThorQ::UI {

class ErrorLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ErrorLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit ErrorLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());;
    ~ErrorLabel();
};

}

#endif // THORQ_ERRORLABEL_H
