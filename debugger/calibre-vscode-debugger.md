Calibre debugger setup

Calibre don't support python to run, it use it's interperter 

1. download debugypy
   > pip install debugpy  #python install  
   > install vscode plugin 'ext install ms-python.debugpy'

2. calibre support option with prefix of --, so need to add this debugypy code base
    >/home/{USER}/.local/lib/python3.11/site-packages/debugpy/server/api.py
    in my case {USER} is home

    in the above file find where arguments are generating 
    mostly inside def listen() function or find by  "--for-server"

    in my case it found here
    ```python
    adapter_args = [
            _config.get("python", sys.executable),
            os.path.dirname(adapter.__file__),
            "--for-server",
            str(endpoints_port),
            "--host",
            host,
            "--port",
            str(port),
            "--server-access-token",
            server_access_token,
        ]
    ```

    in this adapter_args need to add "--" before option start, since here 1st option is --for-server, add before that

    ```python
            adapter_args = [
            _config.get("python", sys.executable),
            os.path.dirname(adapter.__file__),
            "--",
            "--for-server",
            str(endpoints_port),
            "--host",
            host,
            "--port",
            str(port),
            "--server-access-token",
            server_access_token,
        ]
    ```
3. Calibre's working path is different so add the site_packages to calibre's main file
    ```python
    import sys
    sys.path.append('/home/home/.local/lib/python3.11/site-packages')
    import debugpy
    debugpy.log_to('/tmp/debugpy_log')  # Logs debugpy activity
    debugpy.listen(("127.0.0.1", 2712))  # Listen for VSCode on port 5678
    print("Waiting for debugger to attach...")
    debugpy.wait_for_client()
    ```
4. place break point
5. setup env path where source code is downloaded

  > export CALIBRE_DEVELOP_FROM=/home/home/Project/test/calibre/src

  /home/home/Project/test/ here i did git clone, cd calibre/src get pwd and use that path

6. Example to run custom script
    > cd ~/Project/test/calibre/src  

    > code .

    > touch .vscode/launch.json
    ```json
    {
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Python Debugger: Remote Attach",
            "type": "debugpy",
            "request": "attach",
            "connect": {
                "host": "localhost",
                "port": 2712
            },
            "pathMappings": [
                {
                    "localRoot": "/home/home/Project/test/calibre/src",
                    "remoteRoot": "."
                }
            ],
            "justMyCode": false
        }
    ]
    }
    ```

    > export CALIBRE_DEVELOP_FROM=/home/home/Project/test/calibre/src  

    > vi test.py
    ```python
    import sys
    sys.path.append('/home/home/.local/lib/python3.11/site-packages')

    import debugpy
    import time
    print("Debug is imported")
    debugpy.log_to('/tmp/debugpy_log')  # Logs debugpy activity
    debugpy.listen(("127.0.0.1", 2712))  # Listen for VSCode on port 5678

    print("Waiting for debugger to attach...")
    debugpy.wait_for_client() 

    a = 5
    print("a = ", a)
    ```
    > calibre-debug -e jitest.py

    > click debug icon in vscode

    To execute calibre keep the debugypy code snipte in `/home/home/Project/test/calibre/src/calibre/gui2/main.py`

    > calibre-debug -g #execute or calibre-debug -e /home/home/Project/test/calibre/src/calibre/gui2/main.py

    > click debug 

    