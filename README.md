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

## Message Types

### Writing Icons

To write an icon, the following steps should be followed:

1. A message is sent with the type "wi" (write icon) followed by the index of the icon.
2. The data portion contains the byte size of the icon.
3. The receiver will respond with "rd" (ready) to indicate it is prepared to receive the icon.
4. The icon data is then transmitted.
5. Once writing is complete:
   - "ok" is returned if the write operation is successful.
   - "!ok" is returned if the write operation fails.

### Reading Icons

To read an icon, the following steps should be followed:

1. A message is sent with the type "ri" (read icon) followed by the index of the icon.
2. The receiver responds with either:
   - "!ok" if the request is invalid or the icon cannot be read.
   - "rd?" followed by the byte size of the icon if the request is valid.
3. Wait for a response:
   - "rd" (ready) indicates the receiver is prepared to accept the icon.
4. The receiver will then transmit the icon data.
5. Once transmission is complete, the process ends.

### Handling Icon Clicks

When an icon is clicked, a message will be sent with the type "ic" (icon clicked) followed by the index of the icon.

### Retrieving Device Information

To obtain device information, follow these steps:

1. Send a message with the type "li" (list info).
2. The receiver will respond with a message of type "li", where the data contains the device information.
3. The data is parsed by splitting on spaces.
4. The response contains five pieces of information in the following order:
   - Width
   - Height
   - Number of Buttons Per Row
   - Number of Rows
   - Gap Size
