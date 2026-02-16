from setuptools import setup, Extension

extra_compile_flags = ["-Iinclude", "-g"]

setup(
    name="resource_api_monitor",
    version="0.0.1",
    description="A simple API to check the cpu and memory usage of an instance",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="https://github.com/cubos-d/res_api_check",
    packages=["sys_monitor"],
    ext_modules=[
        Extension(
            "sys_monitor.monitor",
            sources=[
                "sys_monitor/src/main.c",
                "sys_monitor/src/hashmap.c",
                "sys_monitor/src/parser.c",
                "sys_monitor/src/cpustats.c",
            ],
            headers=["sys_monitor/include/metrics.h"],
            extra_compile_args=extra_compile_flags,
        )
    ],
)
