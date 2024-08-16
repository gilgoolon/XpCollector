import argparse
from pathlib import Path

import uvicorn
from fastapi import FastAPI

import configurator
from coms.protocol.requests import BasicRequest, RequestHeader, RequestType, ReturnProductRequest, \
    ReturnProductContent
from coms.protocol.responses import BasicResponse, GetCommandResponse, InstallClientResponse

argument_parser = argparse.ArgumentParser()
argument_parser.add_argument("-c", "--config", default="../conf.json", type=Path, help="Path to config file")
argument_parser.add_argument("-p", "--port", default="../conf.json", type=int, help="Port to listen for clients on")
args = argument_parser.parse_args()

app = FastAPI()
server = configurator.parse(args.config)


@app.get("/")
def make_request(request: BasicRequest) -> BasicResponse:
    return server.handle_request(request)


@app.get("/get-command")
def get_command(client_id: str) -> GetCommandResponse:
    return GetCommandResponse(
        **make_request(
            BasicRequest(
                header=RequestHeader(
                    client_id=client_id,
                    request_type=RequestType.GetCommand
                ),
                content={}
            )
        ).model_dump()
    )


@app.get("/return-product")
def return_product(client_id: str, product_id: str, product_data: str) -> BasicResponse:
    return make_request(
        ReturnProductRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.ReturnProduct
            ),
            content=ReturnProductContent(
                product_id=product_id,
                data=product_data,
            )
        )
    )


@app.get("/install-client")
def install_client() -> InstallClientResponse:
    return InstallClientResponse(
        **make_request(
            BasicRequest(
                header=RequestHeader(
                    client_id="",
                    request_type=RequestType.InstallClient
                ),
            )
        ).model_dump()
    )


if __name__ == '__main__':
    uvicorn.run(app, host="localhost", port=args.port)
