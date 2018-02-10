# Illinois Tech Robotics SerialCtl 2

`SerialCtl 2` is ITR's fifth robot control software. The previous versions are (in chronological order): [ITR-Gumstix-Controller](https://github.com/illinoistechrobotics/ITR-Gumstix-Controller), [robots](https://github.com/illinoistechrobotics/robots), [robots v2](https://github.com/illinoistechrobotics/robots_v2), and the original [serialctl](https://github.com/illinoistechrobotics/serialctl)

`SerialCtl 2` is written in C++.  `ITR-Gumstix-Controller` and `serialctl` are also written in C, while `robots` and `robots_v2` are written in Java.

Unlike the previous serialctl, SerialCtl 2 is meant to be used as a separate library, and does not have a separate branch for each robot.

## Communications

One packet consists of a 2-byte big endian CRC-16 checksum, a 1-byte packet type, up to 252 bytes of either UTF-8 text or Base-64 encoded binary (depending on the packet type), and finally a 0xFF terminator.

Note: The CRC-16 can contain 0xFF bytes so don't start looking for the terminator until after the 2nd byte

### Packet Types:
(TODO: Add details on each packet type)
```
0x00: Identify - (Empty) Sent by the computer to the robot to request an Identify Response packet
0x01: Identify Response - (UTF-8) Sent by the robot to the computer to configure the console layout
0x02: Joystick Data - (Binary) Sent by the computer to the robot to update joystick data
0x03: Robot Status - (Binary) Sent by the robot to the computer in response to joystick data
0x04: Log Message - (UTF-8) Sent by the robot to the computer to log data
0x05: Error Message - (UTF-8) Sent by the robot to the computer to log errors
0x06 - 0xEF - Reserved for Future Packet Types
0xF0 - 0xFE - (UTF-8) Custom Robot Commands (sendable via the console)
```
