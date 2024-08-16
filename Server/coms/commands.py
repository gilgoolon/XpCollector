import enum

from pydantic import BaseModel


class CommandType(str, enum.Enum):
    Popup = "Popup"


class BasicCommand(BaseModel):
    command_type: CommandType
    parameters: dict
