from enum import Enum

from pydantic import BaseModel


class ResponseType(Enum):
    Success = "Success"
    Error = "Error"


class ResponseHeader(BaseModel):
    status: ResponseType


class BasicResponse(BaseModel):
    header: ResponseHeader
    content: dict


class SendCommandContent(BaseModel):
    product: dict


class SendCommandResponse(BasicResponse):
    content: SendCommandContent
