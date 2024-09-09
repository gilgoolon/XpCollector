from enum import Enum
from typing import Union, Any

from pydantic import BaseModel


class ResponseType(Enum):
    Success = "Success"
    Error = "Error"


class ResponseHeader(BaseModel):
    status: ResponseType


class BasicResponse(BaseModel):
    header: ResponseHeader
    content: Union[dict, Any] = {}


class SendCommandContent(BaseModel):
    product: dict


class SendCommandResponse(BasicResponse):
    content: SendCommandContent
