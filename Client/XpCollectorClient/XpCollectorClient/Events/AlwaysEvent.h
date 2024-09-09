#pragma once
#include "IEvent.h"

namespace xp_collector {

class AlwaysEvent :
    public IEvent
{
    virtual EventType is_detected() override;
};

}
