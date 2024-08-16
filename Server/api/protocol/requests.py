import abc
from enum import Enum

from pydantic import BaseModel


class RequestType(str, Enum):
    IsAlive = "IsAlive"
    Popup = "Popup"


class RequestHeader(BaseModel):
    client_id: str
    request_type: RequestType


class BaseRequest(BaseModel, abc.ABC):
    pass


class BasicRequest(BaseRequest):
    header: RequestHeader
    content: dict


class PopupContent(BaseModel):
    message: str


class PopupRequest(BaseRequest):
    header: RequestHeader
    content: PopupContent
