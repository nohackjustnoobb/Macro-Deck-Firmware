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

1. A message is sent with the type "wi" (write icon).
2. The data portion contains (splitted by spaces):
   - The index of the icon.
   - The byte size of the icon.
3. The receiver will respond with "rd" (ready) to indicate it is prepared to receive the icon.
4. The icon data is then transmitted.
5. Once writing is complete:
   - "ok" is returned if the write operation is successful.
   - "!ok" is returned if the write operation fails.

### Reading Icons

To read an icon, the following steps should be followed:

1. A message is sent with the type "ri" (read icon).
2. The data portion contains:
   - The index of the icon.
3. The receiver responds with either:
   - "!ok" if the request is invalid or the icon cannot be read.
   - "rd?" followed by the byte size of the icon if the request is valid.
4. Wait for a response:
   - "rd" (ready) indicates the receiver is prepared to accept the icon.
5. The receiver will then transmit the icon data.
6. Once transmission is complete, the process ends.

### Handling Icon Clicks

When an icon is clicked, a message will be sent with the type "ic" (icon clicked). The data portion contains:

- The index of the clicked icon.

### Retrieving Device Information

To obtain device information, follow these steps:

1. Send a message with the type "li" (list info).
2. The receiver will respond with a message of type "li".
3. The data portion contains infomations in the following order (splitted by spaces):
   - Width
   - Height
   - Number of Buttons Per Row
   - Number of Rows
   - Gap Size

### Setting Status

To set a status, follow these steps:

1. A message is sent with the type "ss" (Set Status).
2. The data portion contains:
   - The x coordinate.
   - The y coordinate.
   - The size of the image.
3. The receiver will respond with "rd" (ready) to indicate it is prepared to receive the image.
4. The image data is then transmitted.
5. Once writing is complete:
   - "ok" is returned if the set operation is successful.
   - "!ok" is returned if the set operation fails.

### Status Clicked

When a status is clicked, a message will be sent with the type "sc" (status clicked). The data portion contains:

- The x coordinate of the click position.
