import json

import requests


class DBHandler:
    """
    Wrapper that communicates with clients db microservice
    """

    def __init__(self, db_url: str) -> None:
        self._db_url = db_url

    def try_resolve_name(self, name: str) -> str:
        response = requests.get(f"{self._db_url}/get-client-id/{name}")
        data = json.loads(response.text)
        success = data.get("exists", False)
        return data["client_id"] if success else name
