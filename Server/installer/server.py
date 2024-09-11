import argparse
from pathlib import Path

from fastapi import FastAPI, HTTPException
from fastapi.responses import PlainTextResponse
import base64

parser = argparse.ArgumentParser()
parser.add_argument("-f ", "--path", required=True, type=Path, help="Path to the file to serve")
parser.add_argument("-p", "--port", required=True, type=int, help="Port to start the server on")
args = parser.parse_args()

app = FastAPI()


@app.get("/install", response_class=PlainTextResponse)
async def serve_file():
    # Ensure file exists
    if not args.path.exists() or not args.path.is_file():
        raise HTTPException(status_code=500, detail="Installation not found")

    # Read the file and encode in Base64
    file_content = args.path.read_bytes()
    encoded_content = base64.b64encode(file_content).decode("utf-8")

    return encoded_content


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=args.port)
