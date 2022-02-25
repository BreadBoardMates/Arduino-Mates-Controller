# Communication Errors

The table below lists the possible errors after running a function from the library.

| Value | Error                         | Description                                           |
|:-----:|:-----------------------------:| ----------------------------------------------------- |
| 0     | MATES_ERROR_NONE              | Last command was completed successfully               |
| 1     | MATES_ERROR_COMMAND_FAILED    | The display failed to perform the last command        |
| 2     | MATES_ERROR_COMMAND_TIMEOUT   | The display didn't acknowledge the command in time    |
| 3     | MATES_ERROR_RESPONSE_TIMEOUT  | The display didn't send a return value in time        |
| 4     | MATES_ERROR_NOT_INITIALIZED   | The library hasn't been initialized yet               |
| 5     | MATES_ERROR_SYNC_TIMEOUT      | The synchronization attempt reached specified timeout |
| 6     | MATES_ERROR_SYNC_ERROR        | Invalid active page or no reply received after sync   |