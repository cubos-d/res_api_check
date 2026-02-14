from sys_monitor import monitor


def main():
    # Hello!
    dic_mem = monitor.get_memory_info()
    dic_cpu = monitor.get_cpu_info()
    print(dic_mem)
    print(dic_cpu)


if __name__ == "__main__":
    main()
