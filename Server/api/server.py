import json
import uuid

from api.protocol.requests import SendCommandRequest
from protocol.responses import BasicResponse, ResponseHeader, ResponseType
from utils import readers, writers


class Server:
    def __init__(self, writer: writers.Writer, reader: readers.Reader) -> None:
        self._command_writer = writer
        self._product_reader = reader

    def send_command(self, request: SendCommandRequest) -> str:
        """
        Send a request to the communicator
        :param request: the request to send
        :return: the id of the request and response - to look for
        """
        identifier = uuid.uuid4().hex
        request.content.command.command_id = identifier
        self._command_writer.write(request.header.client_id + "/" + identifier,
                                   json.dumps(request.content.command.model_dump()))
        return identifier

    @staticmethod
    def is_alive(client_id: str) -> BasicResponse:
        return BasicResponse(
            header=ResponseHeader(status=ResponseType.Success),
            content={}
        )

    def wait_for_response(self, identifier: str) -> BasicResponse:
        for item in self._product_reader.read(identifier):
            return BasicResponse(**item)
