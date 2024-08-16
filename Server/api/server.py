import json
import uuid

from api.protocol.requests import BaseRequest
from protocol.responses import BasicResponse
from utils import readers, writers


class Server:
    def __init__(self, writer: writers.Writer, reader: readers.Reader) -> None:
        self._command_writer = writer
        self._product_reader = reader

    def send_request(self, request: BaseRequest) -> str:
        """
        Send a request to the communicator
        :param request: the request to send
        :return: the id of the request and response - to look for
        """
        identifier = uuid.uuid4().hex
        self._command_writer.write(identifier, json.dumps(request.model_dump()))
        return identifier

    def wait_for_response(self, identifier: str) -> BasicResponse:
        for item in self._product_reader.read(identifier):
            return BasicResponse(**item)
