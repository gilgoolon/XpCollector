from enum import Enum
from typing import Optional, Any, Union

from pydantic import BaseModel

from commands import BasicCommand


class ResponseType(str, Enum):
    Success = "Success"
    Error = "Error"


class ResponseHeader(BaseModel):
    status: ResponseType


class BasicResponse(BaseModel):
    header: ResponseHeader
    content: Union[dict, Any]


class InstallClientContent(BaseModel):
    client_id: str


class InstallClientResponse(BasicResponse):
    content: InstallClientContent


class GetCommandContent(BaseModel):
    exists: bool
    command: Optional[BasicCommand]


class GetCommandResponse(BasicResponse):
    content: GetCommandContent
