# Wwise PubSub Wamp - Python Sample
## Overview

This sample demonstrates how to connect, subscribe, and react to Events coming from Wwise through WAAPI using Python with Autobahn and Asyncio.

## Requirements

Make sure you have pip installed (it should be provided with the Python installation and the command might not be available until you restart your computer).


Install Autobahn and `six` using the following command in the terminal:

Windows:
``` py -3 -m pip install autobahn six```

macOS, Linux
``` python3 -m pip install autobahn six```

## Execution

Run the following commands from the "wwise-pubsub-wamp" directory:

Windows:
``` py -3 get_ancestors_py3.py ```

macOS, Linux:
``` python3 get_ancestors_py3.py ```

Create an object in Wwise's Project Explorer and see that its ancestors are printed in the output.
The output should look like:

```
The object was created in the category: Actor-Mixer Hierarchy
Ancestor of {2001B305-5900-4A64-B6C1-81A626FAA0EA}:
        Default Work Unit
        Actor-Mixer Hierarchy
2022-11-09T09:50:18 session closed with reason wamp.error.goodbye_and_out [None]
The client was disconnected.
```
