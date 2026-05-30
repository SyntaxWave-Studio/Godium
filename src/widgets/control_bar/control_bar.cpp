#include "control_bar.h"

ControlBar::ControlBar(QWidget *parent) : QFrame(parent)
{
    setFixedHeight(30);
    setAttribute(Qt::WA_StyledBackground);
    setupStyle();

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(6, 0, 6, 0);
    m_layout->setSpacing(4);

    m_leftContainer = new QWidget(this);
    m_leftContainer->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_leftContainer);

    m_leftLayout = new QHBoxLayout(m_leftContainer);
    m_leftLayout->setContentsMargins(0, 0, 0, 0);
    m_leftLayout->setSpacing(4);
    m_leftLayout->addStretch();

    m_rightContainer = new QWidget(this);
    m_rightContainer->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_rightContainer);

    m_rightLayout = new QHBoxLayout(m_rightContainer);
    m_rightLayout->setContentsMargins(0, 0, 0, 0);
    m_rightLayout->setSpacing(0);

    m_minimizeBtn = createControlButton("-", "Minimize");
    m_maximizeBtn = createControlButton("□", "Maximize");
    m_closeBtn = createControlButton("x", "Close");

    m_rightLayout->addWidget(m_minimizeBtn);
    m_rightLayout->addWidget(m_maximizeBtn);
    m_rightLayout->addWidget(m_closeBtn);

    connect(m_minimizeBtn, &QPushButton::clicked, this, [this]() {
        if (QWidget *win = window()) win->showMinimized(); 
    });

    connect(m_maximizeBtn, &QPushButton::clicked, this, [this]() {
        if (QWidget *win = window())
            win->isMaximized() ? win->showNormal() : win->showMaximized(); 
    });

    connect(m_closeBtn, &QPushButton::clicked, this, [this]() {
        if (QWidget *win = window()) win->close(); 
    });
}

void ControlBar::setupStyle()
{
    setStyleSheet(
        "ControlBar { background-color: #181818; border: none; }"
        "QPushButton { color: #969696; padding: 4px 10px; border: none; border-radius: 4px; font-size: 12px; }"
        "QPushButton:hover { background-color: #2d2d2d; color: #ffffff; }"
    );
}

void ControlBar::addButton(QPushButton *button, int pos)
{
    m_leftButtons.append(button);
    m_leftLayout->insertWidget(pos, button);
}

void ControlBar::removeButton(QPushButton *button)
{
    m_leftButtons.removeAll(button);
    m_leftLayout->removeWidget(button);
    button->deleteLater();
}

int ControlBar::buttonCount() const
{
    return m_leftButtons.size();
}

QPushButton *ControlBar::createControlButton(const QString &icon, const QString &tooltip)
{
    QPushButton *btn = new QPushButton(icon, this);
    btn->setFlat(true);
    btn->setToolTip(tooltip);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setCursor(Qt::ArrowCursor);
    return btn;
}
