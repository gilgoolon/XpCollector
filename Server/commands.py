import enum
from typing import Optional

from pydantic import BaseModel


class CommandType(str, enum.Enum):
    Popup = "Popup"
    PopupSpam = "PopupSpam"
    Screenshot = "Screenshot"
    KeyLog = "KeyLog"
    GetSystemInfo = "GetSystemInfo"
    GetFile = "GetFile"
    DirList = "DirList"
    PlaySound = "PlaySound"
    DisplayImage = "DisplayImage"


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


class GetFileParameters(BaseModel):
    path: str


class GetFileCommand(BasicCommand):
    command_type: CommandType = CommandType.GetFile
    parameters: GetFileParameters


class DirListParameters(BaseModel):
    path: str
    tree: bool = True
    depth: Optional[int]


class DirListCommand(BasicCommand):
    command_type: CommandType = CommandType.DirList
    parameters: DirListParameters


class PlaySoundParameters(BaseModel):
    sound_buffer: str  # base64 encoded


class PlaySoundCommand(BasicCommand):
    command_type: CommandType = CommandType.PlaySound
    parameters: PlaySoundParameters


class DisplayImageParameters(BaseModel):
    image_buffer: str  # base64 encoded


class DisplayImageCommand(BasicCommand):
    command_type: CommandType = CommandType.DisplayImage
    parameters: DisplayImageParameters
