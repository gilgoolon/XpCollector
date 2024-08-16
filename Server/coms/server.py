import uuid

from coms.commands import BasicCommand
from coms.protocol.requests import BasicRequest, RequestType, ReturnProductRequest
from protocol.responses import BasicResponse, InstallClientResponse, GetCommandResponse, InstallClientContent, \
    ResponseHeader, ResponseType, GetCommandContent
from utils import readers, writers


class Server:
    def __init__(self, writer: writers.Writer, reader: readers.Reader) -> None:
        self._product_writer = writer
        self._command_reader = reader

    def handle_request(self, request: BasicRequest) -> BasicResponse:
        """
        Send a request to the communicator
        :param request: the request to send
        :return: the id of the request and response - to look for
        """
        tp = request.header.request_type
        if tp is RequestType.GetCommand:
            return self._handle_get_command(request)
        elif tp is RequestType.ReturnProduct:
            return self._handle_return_product(ReturnProductRequest(**request.model_dump()))
        elif tp is RequestType.InstallClient:
            return self._handle_install_client()

    def _handle_get_command(self, request: BasicRequest) -> GetCommandResponse:
        found = next(self._command_reader.read(pattern=request.header.client_id + "/**"))
        exists = found is not None
        return GetCommandResponse(
            header=ResponseHeader(status=ResponseType.Success),
            content=GetCommandContent(
                exists=exists,
                command=None if not exists else BasicCommand(**found)
            )
        )

    def _handle_return_product(self, request: ReturnProductRequest) -> BasicResponse:
        self._product_writer.write(request.header.client_id + "/" + request.content.product_id, request.content.data)
        return BasicResponse(
            header=ResponseHeader(status=ResponseType.Success),
            content={}
        )

    @staticmethod
    def _handle_install_client() -> InstallClientResponse:
        return InstallClientResponse(
            header=ResponseHeader(status=ResponseType.Success),
            content=InstallClientContent(client_id=uuid.uuid4().hex)
        )
