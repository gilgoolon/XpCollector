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
        writer=writers.FileWriter(config.products_path),
        reader=readers.FolderScanner(config.commands_path)
    )
