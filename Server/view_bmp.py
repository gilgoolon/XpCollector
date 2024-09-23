import base64
import json
from pathlib import Path


path = Path(r"C:\Users\alper\OneDrive\Documents\XpCollector\products\8c7a23402c1f4731b48d80fe0b74502b\13d8a8e5d99846dab16d10666ec7704a")

data = path.read_text()
data = json.loads(data)

bitmap = data["bitmap_data"]

decoded = base64.b64decode(bitmap)

Path(path.parent / "screenshot.bmp").write_bytes(decoded)
