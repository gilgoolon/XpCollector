import json
from pathlib import Path

from api import db_handler
from config import Config
from server import Server
from utils import readers, writers


def parse(path: Path) -> Server:
    config = Config(**json.loads(path.read_text()))
    return Server(
        writer=writers.FileWriter(config.commands_path),
        reader=readers.FolderWatcher(config.products_path),
        db_handler=db_handler.DBHandler(config.clients_db_url)
    )
