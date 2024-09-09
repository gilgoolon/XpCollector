import base64
import json
from pathlib import Path


path = Path(r"C:\Users\alper\OneDrive\Documents\TheFriendlyTroll\products\4b085825a5ba44d9a9afed02ce26525c\9213925d4ffc4043baa9061027443de0")

data = path.read_text()
data = json.loads(data)

bitmap = data["bitmap_data"]

decoded = base64.b64decode(bitmap)

Path(path.parent / "screenshot.bmp").write_bytes(decoded)
