import json
from pathlib import Path

from pydantic import BaseModel

from server import Server
from utils import readers, writers


class Config(BaseModel):
    products_path: Path
    commands_path: Path


def parse(path: Path) -> Server:
    config = Config(**json.loads(path.read_text()))
    return Server(
        writer=writers.FileWriter(config.commands_path),
        reader=readers.FolderWatcher(config.products_path)
    )
