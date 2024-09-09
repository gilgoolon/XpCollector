import json
import uuid

from api.db_handler import DBHandler
from api.protocol.requests import SendCommandRequest
from protocol.responses import BasicResponse, ResponseHeader, ResponseType, SendCommandContent, SendCommandResponse
from utils import readers, writers


class Server:
    def __init__(self, writer: writers.Writer, reader: readers.Reader, db_handler: DBHandler) -> None:
        self._command_writer = writer
        self._product_reader = reader
        self._db_handler = db_handler

    def handle_send_command(self, request: SendCommandRequest) -> SendCommandResponse:
        """
        Send a request to the communicator
        :param request: the request to send
        :return: the id of the request and response - to look for
        """
        # before beginning handling the request, try to resolve name to client id with the db
        resolved_client_id = self._db_handler.try_resolve_name(request.header.client_id)
        request.header.client_id = resolved_client_id
        request_id = self.send_command(request)
        return self.wait_for_product(request.header.client_id, request_id)

    def send_command(self, request: SendCommandRequest) -> str:
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

    def wait_for_product(self, client_id: str, identifier: str) -> SendCommandResponse:
        for content in self._product_reader.read(client_id + "/" + identifier):
            return SendCommandResponse(
                header=ResponseHeader(status=ResponseType.Success),
                content=SendCommandContent(product=content)
            )
