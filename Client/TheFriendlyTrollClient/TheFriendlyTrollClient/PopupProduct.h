#pragma once
#include "BasicProduct.h"

class PopupProduct :
    public BasicProduct
{
public:
    explicit PopupProduct(std::string product_id, bool is_success);

    json serialize() override;

private:
    bool m_is_success;
};

