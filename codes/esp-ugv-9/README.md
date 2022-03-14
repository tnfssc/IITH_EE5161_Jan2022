# UGV Bluetooth Controller using just your browser

## Prerequisites

- Assembled UGV

## Setup

- Enable this in your Chrome browser:
[chrome://flags/#enable-experimental-web-platform-features](chrome://flags/#enable-experimental-web-platform-features)
- Flash the [code](./) to your UGV.

## Usage

- Visit [https://ugv-bt.vercel.app/](https://ugv-bt.vercel.app/)
- Connect bluetooth to your UGV.
- Use the buttons to control the UGV.

## How it works

A bluetooth device has services that it can provide. Some have write and read access. Each service has its own UUID. The UUID is used to identify the service. The service has a characteristic that it can provide. The characteristic has a UUID. The UUID is used to identify the characteristic. The characteristic has a value. The value is used to provide the data. In the UGV code, we create a bluetooth server that provides a service with a characteristic that has a value. The value is a string. The string is the command to the UGV. We allow write access to the string to the connections. In the web app, we connect to the server and get the characteristic we specified in the UGV. We use this to send the command to the UGV.
