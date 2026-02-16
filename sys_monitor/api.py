from fastapi import FastAPI
import uvicorn

from . import monitor
from . import cpu_stats

app = FastAPI(title="System Monitor API")


@app.get("/memory")
async def get_memory():
    return monitor.get_memory_info()


@app.get("/cpu")
async def get_cpu(samples: int = 1, sleep: float = 0.3):
    results = await cpu_stats.get_cpu_usage(n_samples=samples, sleep_time=sleep)
    return results


def start_server():
    uvicorn.run("sys_monitor.api:app", host="0.0.0.0", port=8000)
