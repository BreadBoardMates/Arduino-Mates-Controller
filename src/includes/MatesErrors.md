# Communication Errors

The table below lists the possible errors after running a function from the library.

| Value | Error                         | Description                                        |
|:-----:|:-----------------------------:| -------------------------------------------------- |
| 0     | MATES_ERROR_NONE              | No error has occurred                              |
| 1     | MATES_ERROR_COMMAND_FAILED    | The display failed to perform the command          |
| 2     | MATES_ERROR_COMMAND_TIMEOUT   | The display didn't acknowledge the command in time |
| 3     | MATES_ERROR_RESPONSE_TIMEOUT  | The display didn't send a return value in time     |
| 4     | MATES_ERROR_NOT_INITIALIZED   | The library hasn't been initialized yet            |