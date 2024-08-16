from enum import Enum

from pydantic import BaseModel


class RequestType(str, Enum):
    GetCommand = "GetCommand"
    ReturnProduct = "ReturnProduct"
    InstallClient = "InstallClient"


class RequestHeader(BaseModel):
    client_id: str
    request_type: RequestType


class BasicRequest(BaseModel):
    header: RequestHeader
    content: dict = {}


class ReturnProductContent(BaseModel):
    product_id: str
    data: str


class ReturnProductRequest(BasicRequest):
    content: ReturnProductContent
