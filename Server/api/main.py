import argparse
from pathlib import Path

import uvicorn
from fastapi import FastAPI

import configurator
from protocol.requests import BasicRequest, RequestHeader, RequestType, PopupContent, PopupRequest, BaseRequest
from protocol.responses import BasicResponse


argument_parser = argparse.ArgumentParser()
argument_parser.add_argument("-c", "--config", default="../conf.json", type=Path, help="Path to config file")
args = argument_parser.parse_args()

app = FastAPI()
server = configurator.parse(args.config)


@app.get("/")
def make_request(request: BaseRequest) -> BasicResponse:
    request_id = server.send_request(request)
    return server.wait_for_response(request_id)


@app.get("/popup")
def make_popup_request(client_id: str, message: str) -> BasicResponse:
    return make_request(
        PopupRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.Popup
            ),
            content=PopupContent(message=message)
        )
    )


if __name__ == '__main__':
    uvicorn.run(app, host="localhost", port=8000)
