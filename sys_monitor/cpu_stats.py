import asyncio
from typing import Tuple, Dict

from . import monitor


def get_individual_cpu_usage(deltas) -> float:
    total = sum(deltas)
    idle_wait = deltas[3] + deltas[4]
    cpu_usage = (total - idle_wait) / total
    return cpu_usage


def diff(before: Tuple[int], after: Tuple[int]):
    return tuple(map(lambda x, y: y - x, before, after))


async def task(sleep_time: float, index_order: int):
    await asyncio.sleep(index_order * 0.05)
    tkey = "total_details"
    ckey = "cpu_details"
    dic_bf = monitor.get_cpu_info()  # type: ignore
    await asyncio.sleep(sleep_time)
    dic_af = monitor.get_cpu_info()  # type: ignore
    total_delta = diff(dic_bf[tkey], dic_af[tkey])
    deltas = tuple(map(lambda x, y: diff(x, y), dic_bf[ckey], dic_af[ckey]))
    total_cpu_usage = get_individual_cpu_usage(total_delta)
    each_cpu_usage = tuple(map(get_individual_cpu_usage, deltas))
    return {"total_cpu_usage": total_cpu_usage, "usage_per_cpu": each_cpu_usage}


async def get_cpu_usage(
    n_samples: int = 5, sleep_time: float = 0.2
) -> Dict[str, float | Tuple[float]]:
    results = await asyncio.gather(*[task(sleep_time, i) for i in range(n_samples)])
    total_avg = sum(map(lambda x: x["total_cpu_usage"], results)) / n_samples
    tup_cpus = (cpu_us["usage_per_cpu"] for cpu_us in results)
    mat_avg = zip(*tup_cpus)
    cpus_avg = tuple(map(lambda x: sum(x) / n_samples, mat_avg))
    return {"total_cpu_usage": total_avg, "usage_per_cpu": cpus_avg}
