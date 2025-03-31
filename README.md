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
