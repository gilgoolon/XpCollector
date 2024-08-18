from typing import Optional

from pydantic import BaseModel


class GetClientIdResponse(BaseModel):
    name_exists: bool
    client_id: Optional[str]
