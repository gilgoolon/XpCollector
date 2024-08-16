import abc
import glob
import json
import time
from pathlib import Path
from typing import Optional


class Reader(abc.ABC):
    """
    Interface
    """

    @abc.abstractmethod
    def read(self, pattern: Optional[str] = None) -> dict:
        """
        Read data from the input method
        Should be a generator function returning all matching items
        :param pattern: filter item names by this pattern
        :return: the json data stored in the file
        """


class FolderWatcher(Reader):
    """
    Watch a folder for incoming files
    """

    TIMEOUT = 5  # seconds

    def __init__(self, folder: Path) -> None:
        self._folder = folder

    def read(self, pattern: Optional[str] = None) -> dict:
        full_pattern = str(self._folder / pattern if pattern is not None else self._folder / "**")
        while True:
            for path in glob.glob(full_pattern):
                text = Path(path).read_text()
                yield json.loads(text)
            time.sleep(self.TIMEOUT)


class FolderScanner(Reader):
    """
    Scan a folder for existing files
    """
    def __init__(self, folder: Path) -> None:
        self._folder = folder

    def read(self, pattern: Optional[str] = None) -> dict:
        full_pattern = str(self._folder / pattern if pattern is not None else self._folder / "**")
        for path in glob.glob(full_pattern):
            text = Path(path).read_text()
            yield json.loads(text)

