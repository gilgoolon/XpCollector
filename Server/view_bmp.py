import base64
import json
from pathlib import Path


path = Path(r"C:\Users\alper\OneDrive\Documents\TheFriendlyTroll\products\b6f1a68c8e79462f89d179f86ec8e033\a935bf2d2b704641be98648ed325b419")

data = path.read_text()
data = json.loads(data)

bitmap = data["bitmap_data"]

decoded = base64.b64decode(bitmap)

Path(path.parent / "screenshot.bmp").write_bytes(decoded)
