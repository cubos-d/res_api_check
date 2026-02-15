import asyncio

from sys_monitor import monitor, cpu_stats


def main():
    # Hello!
    dic_mem = monitor.get_memory_info()
    dic_cpu = monitor.get_cpu_info()
    cpu_results = asyncio.run(cpu_stats.get_cpu_usage())
    print(dic_mem)
    print(dic_cpu)
    print(cpu_results)


if __name__ == "__main__":
    main()
