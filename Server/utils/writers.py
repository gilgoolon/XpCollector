import abc
import os
from pathlib import Path
from typing import Union


class Writer(abc.ABC):
    """
    Interface
    """

    @abc.abstractmethod
    def write(self, name: str, data: str) -> None:
        """
        Write a model to some output method
        :param name: name of the product to write
        :param data: contents to write
        :return: None
        """


class FileWriter(Writer):

    def __init__(self, folder: Path) -> None:
        self._folder = folder

    def write(self, name: str, data: Union[str, bytes]) -> None:
        path = self._folder / name
        os.makedirs(path.parent, exist_ok=True)
        if isinstance(data, str):
            path.write_text(data)
        elif isinstance(data, bytes):
            path.write_bytes(data)
        else:
            raise ValueError
