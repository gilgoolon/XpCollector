import abc
import glob
import json
import os
import time
from pathlib import Path
from typing import Optional, Generator


class Reader(abc.ABC):
    """
    Interface
    """

    @abc.abstractmethod
    def read(self, pattern: Optional[str] = None) -> Generator[dict, None, None]:
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

    def read(self, pattern: Optional[str] = None) -> Generator[dict, None, None]:
        full_pattern = str(self._folder / pattern if pattern is not None else self._folder / "**")
        print(f"Searching for files in pattern: {full_pattern}")
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

    def read(self, pattern: Optional[str] = None) -> Generator[dict, None, None]:
        full_pattern = str(self._folder / pattern if pattern is not None else self._folder / "**")
        for path in glob.glob(full_pattern):
            text = Path(path).read_text()
            yield json.loads(text)


class FolderScannerConsumer(Reader):
    """
    Scan a folder for existing files and consume (delete) them after getting their content
    """
    def __init__(self, folder: Path) -> None:
        self._folder = folder

    def read(self, pattern: Optional[str] = None) -> Generator[dict, None, None]:
        full_pattern = str(self._folder / pattern if pattern is not None else self._folder / "**")
        for path in glob.glob(full_pattern):
            text = Path(path).read_text()
            os.remove(path)
            yield json.loads(text)
