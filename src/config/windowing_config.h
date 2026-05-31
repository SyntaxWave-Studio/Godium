#ifndef WINDOWING_CONFIG_H
#define WINDOWING_CONFIG_H

#include "config.h"

class WindowingConfig : public Config
{
public:
    static WindowingConfig& instance();

    bool spacing() const;
    bool roundedCorners() const;

    void setSpacing(bool value);
    void setRoundedCorners(bool value);
    
protected:
    WindowingConfig();
};

#endif
