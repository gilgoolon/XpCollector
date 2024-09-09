#pragma once
#include "IEvent.h"

class AlwaysEvent :
    public IEvent
{
    virtual EventType is_detected() override;
};

