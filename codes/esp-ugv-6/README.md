# Send UGV's `MPU6050` data to a server in realtime

- This code has two parts. The server and the client.
- Server is a simple UDP server that receives data from the UGV and append it to a file (`data.txt`).
- Client is the UGV that sends the data to the server using the server's IP address.
- You need `NodeJS` to run the server. Use the command `node ./server` to run the server.
- In `main.cpp`, you can change the IP address of the server and the wifi settings.
- Start the UGV and move it around to get data to the server.
