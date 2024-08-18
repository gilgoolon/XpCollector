import argparse

import uvicorn
from fastapi import FastAPI

from database.db_handler import DBHandler
from database.responses import GetClientIdResponse

argument_parser = argparse.ArgumentParser()
argument_parser.add_argument("-p", "--port", default="../conf.json", type=int, help="Port to listen for clients on")
args = argument_parser.parse_args()

app = FastAPI()
db_handler = DBHandler()


@app.get("/get-client-id/{name}")
def get_client_id(name: str) -> GetClientIdResponse:
    try:
        return GetClientIdResponse(
            exists=True,
            client_id=db_handler.get_client_id(name)
        )
    except ValueError:
        return GetClientIdResponse(exists=False)


if __name__ == '__main__':
    uvicorn.run(app, host="0.0.0.0", port=args.port)
