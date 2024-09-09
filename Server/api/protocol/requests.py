from enum import Enum

from pydantic import BaseModel

from commands import BasicCommand


class RequestType(str, Enum):
    IsAlive = "IsAlive"
    SendCommand = "SendCommand"


class RequestHeader(BaseModel):
    client_id: str
    request_type: RequestType


class BasicRequest(BaseModel):
    header: RequestHeader
    content: dict


class SendCommandContent(BaseModel):
    command: BasicCommand


class SendCommandRequest(BasicRequest):
    content: SendCommandContent
