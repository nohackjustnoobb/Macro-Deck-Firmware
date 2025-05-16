## TODO instruction for installing TFT_eSPI

## SD Card Folder and File Structure

_Note that the path cannot contain any spaces._

### Root Directory (First Layer)

Each folder in the root directory represents a profile.
The default profile should be placed in a folder named `default`.

### Profile Directory (Second Layer)

Inside each profile folder:

- Button Icons:

  Use .jpg files named 0.jpg, 1.jpg, …, up to the number of buttons minus one.

  Each image corresponds to a button's icon.

- Folder Buttons:

  Addition to a .jpg file, a subfolder can be used for a button.

  If a folder is used, it indicates that the button is a "folder button" and will open another set of buttons (recursively).

### Subfolders (Third Layer and Beyond)

Within a subfolder:

- `0.jpg` — Always reserved for the Back button.
- 1.jpg, 2.jpg, etc. — Icons for other buttons in this nested menu.

### Special Image: `aio.jpg`

- An optional image named aio.jpg can be placed in any profile or subfolder directory.
- This image should contain all button graphics and spacing in a single layout, designed to display directly at position (0, 0).
- If aio.jpg exists, it overrides all individual button images (0.jpg, 1.jpg, etc.) in that folder.

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

### Setting Profile

To set a profile, follow these steps:

1. A message is sent with the type `sp` (set profile).
2. The data portion contains the profile name to be set.
3. The receiver responds with:
   - `ok` if the profile was set successfully.
   - `!ok` if the profile could not be set.

### Writing Icons

To write an icon, the following steps should be followed:

1. A message is sent with the type `wi` (write icon).
2. The data portion contains (splitted by spaces):
   - The path of the icon.
   - The byte size of the icon.
3. The receiver will respond with `rd` (ready) to indicate it is prepared to receive the icon.
4. The icon data is then transmitted.
5. Once writing is complete:
   - `ok` is returned if the write operation is successful.
   - `!ok` is returned if the write operation fails.

### Reading Icons

To read an icon, the following steps should be followed:

1. A message is sent with the type `ri` (read icon).
2. The data portion contains:
   - The path of the icon.
3. The receiver responds with either:
   - `!ok` if the request is invalid or the icon cannot be read.
   - `rd?` followed by the byte size of the icon if the request is valid.
4. Wait for a response:
   - `rd` (ready) indicates the receiver is prepared to accept the icon.
5. The receiver will then transmit the icon data.
6. Once transmission is complete, the process ends.

### Deleting Icons

To delete an icon, follow these steps:

1. A message is sent with the type `di` (delete icon).
2. The data portion contains:
   - The path of the icon to be deleted.
3. The receiver responds with:
   - `ok` if the deletion was successful.
   - `!ok` if the deletion failed or the index is invalid.

### Creating Folder

To create a folder, follow these steps:

1. A message is sent with the type `cf` (create folder).
2. The data portion contains:
   - The path or identifier of the folder.
3. The receiver responds with:
   - `ok` if the folder creation was successful.
   - `!ok` if the folder creation failed.

### Deleting Folder

To delete a folder, follow these steps:

1. A message is sent with the type `df` (delete folder).
2. The data portion contains:
   - The path or identifier of the folder to be deleted.
3. The receiver responds with:
   - `ok` if the folder deletion was successful.
   - `!ok` if the folder does not exist or deletion failed.

### List Directory

To get the directory, the following steps should be followed:

1. A message is sent with the type `ld` (list directory).
2. The receiver responds with either:
   - `!ok` if the request is invalid or the icon cannot be read.
   - `ld` with all the paths of files under the root directory split by space.

### Handling Button Clicks

When an button is clicked, a message will be sent with the type `bc` (button clicked). The data portion contains:

- The path (without extension) of the clicked button.

### Retrieving Device Information

To obtain device information, follow these steps:

1. Send a message with the type `li` (list info).
2. The receiver will respond with a message of type `li`.
3. The data portion contains infomations in the following order (splitted by spaces):
   - Width
   - Height
   - Number of Buttons Per Row
   - Number of Rows
   - Gap Size

### Setting Status

To set a status, follow these steps:

1. A message is sent with the type `ss` (Set Status).
2. The data portion contains:
   - The x coordinate.
   - The y coordinate.
   - The size of the image.
3. The receiver will respond with `rd` (ready) to indicate it is prepared to receive the image.
4. The image data is then transmitted.
5. Once writing is complete:
   - `ok` is returned if the set operation is successful.
   - `!ok` is returned if the set operation fails.

### Status Clicked

When a status is clicked, a message will be sent with the type `sc` (status clicked). The data portion contains:

- The x coordinate of the click position.

### Sync Time

To synchronize the time, follow these steps:

1. A message is sent with the type `st` (sync time).
2. The data portion contains:
   - The epoch time in seconds.
3. The receiver responds with:
   - `ok` if the time synchronization was successful.
   - `!ok` if the synchronization failed.
