from sys_monitor import monitor
def main():
    dic_mem = monitor.get_memory_info()
    print(dic_mem)


if __name__ == "__main__":
    main()
