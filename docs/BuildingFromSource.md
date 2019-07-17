<details>
    <summary>Example VSCode tasks.json:</summary><br>
    ```json
    {
        // See https://go.microsoft.com/fwlink/?LinkId=733558
        // for the documentation about the tasks.json format
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Build (Debug)",
                "type": "shell",
                "command": "./build/build.sh debug",
                "group": "build"
            },
            {
                "label": "Build (Debug Optimized)",
                "type": "shell",
                "command": "./build/build.sh debugoptimized",
                "group": {
                    "kind": "build",
                    "isDefault": true
                }
            },
            {
                "label": "Build (Release)",
                "type": "shell",
                "command": "./build/build.sh release"
            }
        ]
    }
    ```
</details>

<details>
    <summary>Example VSCode launch.json:</summary><br>
    ```json
    {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Launch (Debug)",
                "type": "cppdbg",
                "request": "launch",
                "preLaunchTask": "Build (Debug)",
                "program": "${workspaceFolder}/build/bin/Debug-linux-x86_64/testing/testing",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "miDebuggerPath": "/usr/bin/gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ]
            },
            {
                "name": "Launch (Debug Optimized)",
                "type": "cppdbg",
                "request": "launch",
                "preLaunchTask": "Build (Debug Optimized)",
                "program": "${workspaceFolder}/build/bin/DebugOptimized-linux-x86_64/testing/testing",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "miDebuggerPath": "/usr/bin/gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ]
            },
            {
                "name": "Launch (Release)",
                "type": "cppdbg",
                "request": "launch",
                "preLaunchTask": "Build (Release)",
                "program": "${workspaceFolder}/build/bin/Release-linux-x86_64/testing/testing",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "miDebuggerPath": "/usr/bin/gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ]
            }
        ]
    }
    ```
</details>
