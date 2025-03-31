## Serial Communication Protocol

### Message Format

Each message is structured as follows:

| Section     | Description                                            |
| ----------- | ------------------------------------------------------ |
| Type Length | The first byte indicates the length of the type field. |
| Type        | The next `length` bytes specify the message type.      |
| Data        | The remaining bytes contain the actual data payload.   |

#### Structure

```
[ Type Length (1 byte) ] [ Type (N bytes) ] [ Data (remaining bytes) ]
```

### Writing Icons

To write an icon, the following steps should be followed:

1. A message is sent with the type "wi" (write icon) followed by the index (`idx`) of the icon.
2. The data portion contains the byte size of the icon.
3. The receiver will respond with "rd" (ready) to indicate it is prepared to receive the icon.
4. The icon data is then transmitted.
5. Once writing is complete:
   - "ok" is returned if the write operation is successful.
   - "!ok" is returned if the write operation fails.

### Reading Icons

To read an icon, the following steps should be followed:

1. A message is sent with the type "ri" (read icon) followed by the index (`idx`) of the icon.
2. The receiver responds with either:
   - "!ok" if the request is invalid or the icon cannot be read.
   - "rd?" followed by the byte size of the icon if the request is valid.
3. If "rd?" is received, a message with the type "rd" (ready) must be sent to start the transmission.
4. The receiver will then transmit the icon data.
5. Once transmission is complete, the process ends.
