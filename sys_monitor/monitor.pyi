from typing import Any, Dict

class _Monitor:
    def get_memory_info(self) -> Dict[str, Any]: ...
    def get_cpu_info(self) -> Dict[str, Any]: ...

monitor: _Monitor
