import argparse
from pathlib import Path

import uvicorn
from fastapi import FastAPI

import configurator
from commands import PopupCommand, PopupParameters
from protocol.requests import RequestHeader, RequestType, SendCommandRequest, SendCommandContent
from protocol.responses import BasicResponse

argument_parser = argparse.ArgumentParser()
argument_parser.add_argument("-c", "--config", default="../conf.json", type=Path, help="Path to config file")
args = argument_parser.parse_args()

app = FastAPI()
server = configurator.parse(args.config)


@app.get("/")
def make_command_request(request: SendCommandRequest) -> BasicResponse:
    request_id = server.send_command(request)
    return server.wait_for_response(request_id)


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


@app.get("/is-alive")
def is_alive(client_id: str) -> BasicResponse:
    return server.is_alive(client_id)


if __name__ == '__main__':
    uvicorn.run(app, host="localhost", port=8000)
