from typing import Optional

from pydantic import BaseModel


class GetClientIdResponse(BaseModel):
    exists: bool
    client_id: Optional[str] = None
