import enum

from pydantic import BaseModel


class CommandType(str, enum.Enum):
    Popup = "Popup"


class BasicCommand(BaseModel):
    command_id: str = ""
    command_type: CommandType
    parameters: dict


class PopupParameters(BaseModel):
    message: str


class PopupCommand(BasicCommand):
    command_type: CommandType = CommandType.Popup
    parameters: PopupParameters
