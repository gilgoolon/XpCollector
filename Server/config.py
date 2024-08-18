from pathlib import Path

from pydantic import BaseModel


class Config(BaseModel):
    products_path: Path
    commands_path: Path
    done_commands_path: Path
    clients_db_url: str
