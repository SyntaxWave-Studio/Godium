#ifndef WINDOWING_CONFIG_H
#define WINDOWING_CONFIG_H

#include "config.h"

class WindowingConfig : public Config
{
public:
    static WindowingConfig& instance();
    
    int spacing() const;
    bool roundedCorners() const;
    
    void setSpacing(int value);
    void setRoundedCorners(bool value);
    
protected:
    WindowingConfig();
};

#endif
