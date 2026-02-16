# Resource Checker API

## This is a simple application that checks the resource usage of your server via API

## Requires

+ python = ">=3.13"

## Installation

```bash
pip3 install resource-api-monitor
```

You can also download the .whl package and install from there.

## Usage

Run:

```bash
monitor-server
```

Then just curl to endpoint:8000/cpu or endpoint:8000/memory.

## API Reference

| Endpoint | Method | Description |
| :--- | :--- | :--- |
| `/memory` | GET | Returns RAM usage and swap statistics. |
| `/cpu` | GET | Returns average CPU usage (0.3s duration between samples). |

### Example CPU Output

{
  "total_cpu_usage": 0.15,
  "usage_per_cpu": [0.12, 0.18, 0.14, 0.16]
}
