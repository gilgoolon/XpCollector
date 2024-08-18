import json
from pathlib import Path

from config import Config
from server import Server
from utils import readers, writers


def parse(path: Path) -> Server:
    config = Config(**json.loads(path.read_text()))
    return Server(
        writer=writers.FileWriter(config.products_path),
        reader=readers.FolderScannerConsumerMover(config.commands_path, config.done_commands_path)
    )
